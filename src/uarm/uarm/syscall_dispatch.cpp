#include "syscall_dispatch.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "CPU.h"
#include "cputil.h"
#include "memcpy.h"
#include "nand.h"
#include "pace.h"
#include "savestate/ChunkTypeUarm.h"
#include "savestate/Savestate.h"
#include "savestate/SavestateLoader.h"
#include "syscall.h"
#include "syscall_68k.h"

#define MAX_NEST_LEVEL 4
#define INJECTED_CALL_IPS 100000000ull
#define INJECTED_CALL_TIMEOUT_SEC 4

#define PREPARE_INJECTED_CALL_IPS 50000000ull
#define PREPARE_INJECTED_CALL_TIMEOUT_SEC 1

#define INSTRUCTION_M68K_TRAP0 0x4E40

struct ScratchState {
    enum class Type { full, pace };

    void Load(SavestateLoader<ChunkType>& loader) {
        if (type == Type::full) {
            cpuFinishInjectedCall(cpu, loader);
        } else {
            paceLoad(loader);

            cpuSetReg(cpu, 14, lr);
            cpuSetReg(cpu, 15, pc);
            cpuClearSlowPath(cpu, SLOW_PATH_REASON_INJECTED_CALL_DONE);
            cpuSetSlowPath(cpu, slowPathInjectedCallDone);
            cpuSetModePace(cpu, modePace);
            cpuSetCurInstrPC(cpu, curInstrPC);
        }
    }

    template <typename T>
    void Save(T& savestate) {
        cpuPrepareInjectedCall(cpu, savestate);
    }

    void Save(Type type) {
        this->type = type;

        savestate.Save(*this);

        if (type == Type::pace) {
            const uint32_t* registers = cpuGetRegisters(cpu);
            lr = registers[14];
            pc = registers[15] + (cpuIsThumb(cpu) ? 1 : 0);

            slowPathInjectedCallDone =
                cpuGetSlowPathReason(cpu) & SLOW_PATH_REASON_INJECTED_CALL_DONE;

            modePace = cpuIsModePace(cpu);
            curInstrPC = cpuGetCurInstrPC(cpu);
        }
    }

    void Restore() { loader.Load(savestate.GetBuffer(), savestate.GetSize(), *this); }

    Type type{Type::full};
    uint32_t lr, pc;
    uint32_t curInstrPC;
    uint32_t slowPathInjectedCallDone;
    bool modePace;

    Savestate<ChunkType> savestate;
    SavestateLoader<ChunkType> loader;
    ArmCpu* cpu;
};

struct SyscallDispatch {
    struct SoC* soc;

    ScratchState scratchStates[MAX_NEST_LEVEL];
    size_t nestLevel{0};
    bool deadMansSwitch{false};
};

struct SyscallDispatch* initSyscallDispatch(struct SoC* soc) {
    struct SyscallDispatch* sd = new SyscallDispatch();

    sd->soc = soc;
    for (auto& scratchState : sd->scratchStates) scratchState.cpu = socGetCpu(soc);

    return sd;
}

bool syscallDispatchInProgress(struct SyscallDispatch* sd) { return sd->nestLevel > 0; }

bool syscallDispatchM68kSupport(struct SyscallDispatch* sd) { return socIsPacePatched(sd->soc); }

bool syscallDispatchPrepare(struct SyscallDispatch* sd) {
    if (!syscallDispatchM68kSupport(sd)) return false;
    if (sd->nestLevel > 0) ERR("syscallDispatchPrepare is only allowed on top level\n");

    return socRunToPaceSyscall(sd->soc, SYSCALL_68K_EVT_GET_EVENT,
                               PREPARE_INJECTED_CALL_TIMEOUT_SEC * PREPARE_INJECTED_CALL_IPS,
                               PREPARE_INJECTED_CALL_IPS);
}

bool syscallDispatch_strncpy_toHost(struct SyscallDispatch* sd, void* dest, uint32_t src,
                                    size_t maxLen) {
    struct ArmCpu* cpu = socGetCpu(sd->soc);
    memset(dest, 0, maxLen);

    for (size_t i = 0; i < maxLen; i++) {
        if (!cpuMemOpExternal(cpu, dest, src, 1, false)) return false;
        if (*(const char*)(dest) == '\0') break;

        dest = (char*)dest + 1;
        src++;
    }

    return true;
}

bool syscallDispatch_memcpy_fromHost(struct SyscallDispatch* sd, uint32_t dest, void* src,
                                     size_t size) {
    struct ArmCpu* cpu = socGetCpu(sd->soc);
    MemcpyResult memcpyResult;
    memcpy_hostToArm(dest, reinterpret_cast<uint8_t*>(src), size, true, cpuGetMem(cpu),
                     cpuGetMMU(cpu), &memcpyResult);

    return memcpyResult.ok;
}

bool syscallDispatch_memcpy_toHost(struct SyscallDispatch* sd, void* dest, uint32_t src,
                                   size_t size) {
    struct ArmCpu* cpu = socGetCpu(sd->soc);
    MemcpyResult memcpyResult;
    memcpy_armToHost(reinterpret_cast<uint8_t*>(dest), src, size, true, cpuGetMem(cpu),
                     cpuGetMMU(cpu), &memcpyResult);

    return memcpyResult.ok;
}

static size_t pushState(struct SyscallDispatch* sd, ScratchState::Type type) {
    if (sd->nestLevel >= MAX_NEST_LEVEL)
        ERR("unable to dispatch syscall: max nest level reached\n");

    const uint32_t nestLevel = sd->nestLevel++;
    sd->scratchStates[nestLevel].Save(type);

    return nestLevel;
}

static void popState(struct SyscallDispatch* sd, size_t nestLevel) {
    if (sd->nestLevel != nestLevel + 1) ERR("bad nest level\n");

    sd->scratchStates[nestLevel].Restore();
    sd->nestLevel--;
}

void syscallDispatchRegisterM68kStub(struct SyscallDispatch* sd, uint32_t trampoline,
                                     M68kStub stub) {
    M68kTrapHandler trapHandler = [=](struct ArmCpu* cpu, uint32_t address) -> bool {
        const uint32_t sp = paceGetAreg(7);
        const uint32_t returnAddress = paceGet32(sp);

        stub(cpu, sp + 4, [=]() { sd->deadMansSwitch = true; });

        paceSetAreg(7, sp + 4);
        paceSetPC(returnAddress);

        return true;
    };

    paceSet16(trampoline, INSTRUCTION_M68K_TRAP0);
    cpuAddM68kTrap0Handler(socGetCpu(sd->soc), trampoline, trapHandler);
}

void syscallDispatchUnregisterM68kStub(struct SyscallDispatch* sd, uint32_t trampoline) {
    cpuRemoveM68kTrap0Handler(socGetCpu(sd->soc), trampoline);
}

static bool executeInjectedCall(struct SoC* soc, uint32_t flags) {
    struct ArmCpu* cpu = socGetCpu(soc);

    if (flags & SC_EXECUTE_FULL) {
        if (socExecuteInjected(soc, cpu, INJECTED_CALL_TIMEOUT_SEC * INJECTED_CALL_IPS,
                               INJECTED_CALL_IPS)) {
            return true;
        }
    } else {
        uint64_t cycles = 0;

        do {
            cycles += cpuCyclePure(cpu);
            if (cpuGetSlowPathReason(cpu) & SLOW_PATH_REASON_INJECTED_CALL_DONE) return true;
        } while (cycles < INJECTED_CALL_TIMEOUT_SEC * INJECTED_CALL_IPS);
    }

    return false;
}

static void executeInjectedSyscall(struct SyscallDispatch* sd, uint32_t flags, uint16_t syscall) {
    cpuStartInjectedSyscall(socGetCpu(sd->soc), syscall);

    if (!executeInjectedCall(sd->soc, flags)) {
        ERR("failed to execute injected call within time limit\n");
    }
}

static void executeInjectedSyscall68k(struct SyscallDispatch* sd, uint32_t flags,
                                      uint16_t syscall) {
    struct ArmCpu* cpu = socGetCpu(sd->soc);
    struct NAND* nand = socGetNand(sd->soc);
    bool deadMansSwitchSaved = sd->deadMansSwitch;

    cpuStartInjectedSyscall68k(cpu, syscall);

    while (true) {
        const uint32_t nandWriteCnt = nandGetWriteCnt(nand);
        sd->deadMansSwitch = false;

        if (executeInjectedCall(sd->soc, flags)) break;
        if (sd->deadMansSwitch || nandGetWriteCnt(nand) != nandWriteCnt) continue;

        ERR("failed to execute injected call within time limit\n");
    }

    cpuFinishInjectedSyscall68k(cpu);

    sd->deadMansSwitch = deadMansSwitchSaved;
}

uint16_t syscall_SysSetAutoOffTime(struct SyscallDispatch* sd, uint32_t flags, uint32_t timeout) {
    const size_t nestLevel = pushState(sd, ScratchState::Type::full);
    uint32_t* registers = cpuGetRegisters(socGetCpu(sd->soc));

    registers[0] = timeout;
    executeInjectedSyscall(sd, flags, SYSCALL_SYS_SET_AUTO_OFF_TIME);

    uint16_t err = registers[0];

    popState(sd, nestLevel);

    return err;
}

static uint32_t syscall68k(struct SyscallDispatch* sd, uint32_t flags, uint16_t syscall,
                           bool returnPtr, std::function<void()> setupStack) {
    const size_t nestLevel = pushState(
        sd, (flags & SC_EXECUTE_FULL) ? ScratchState::Type::pace : ScratchState::Type::full);

    if (flags & SC_EXECUTE_FULL) socSuspendTimerInterrupts(sd->soc, true);

    setupStack();
    executeInjectedSyscall68k(sd, flags, syscall);
    const uint32_t result = returnPtr ? paceGetAreg(0) : paceGetDreg(0);

    if (flags & SC_EXECUTE_FULL) socSuspendTimerInterrupts(sd->soc, false);

    popState(sd, nestLevel);

    return result;
}

uint32_t syscall68k_SysGetOsVersionString(struct SyscallDispatch* sd, uint32_t flags) {
    return syscall68k(sd, flags, SYSCALL_68K_SYS_GET_OS_VERSION_STRING, true, []() {});
}

uint32_t syscall68k_MemPtrNew(struct SyscallDispatch* sd, uint32_t flags, uint32_t size) {
    return syscall68k(sd, flags, SYSCALL_68K_MEM_PTR_NEW, true, [=]() { pacePush32(size); });
}

uint16_t syscall68k_MemPtrFree(struct SyscallDispatch* sd, uint32_t flags, uint32_t memPtr) {
    return syscall68k(sd, flags, SYSCALL_68K_MEM_CHUNK_FREE, false, [=]() { pacePush32(memPtr); });
}

uint16_t syscall68k_MemNumCards(struct SyscallDispatch* sd, uint32_t flags) {
    return syscall68k(sd, flags, SYSCALL_68K_MEM_NUM_CARDS, false, []() {});
}

uint16_t syscall68k_ExgDBRead(struct SyscallDispatch* sd, uint32_t flags, uint32_t readProcP,
                              uint32_t deleteProcP, uint32_t userDataP, uint32_t dbIDP,
                              uint16_t cardNo, uint32_t needsResetP, bool keepDates) {
    return syscall68k(sd, flags, SYSCALL_68K_EXG_DB_READ, false, [=]() {
        pacePush8(keepDates);
        pacePush32(needsResetP);
        pacePush16(cardNo);
        pacePush32(dbIDP);
        pacePush32(userDataP);
        pacePush32(deleteProcP);
        pacePush32(readProcP);
    });
}

uint16_t syscall68k_ExgDBWrite(struct SyscallDispatch* sd, uint32_t flags, uint32_t writeProcP,
                               uint32_t userDataP, uint32_t nameP, uint32_t dbID, uint16_t cardNo) {
    return syscall68k(sd, flags, SYSCALL_68K_EXG_DB_WRITE, false, [=]() {
        pacePush16(cardNo);
        pacePush32(dbID);
        pacePush32(nameP);
        pacePush32(userDataP);
        pacePush32(writeProcP);
    });
}

uint16_t syscall68k_DmNumDatabases(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo) {
    return syscall68k(sd, flags, SYSCALL_68K_DM_NUM_DATABASES, false,
                      [=]() { pacePush16(cardNo); });
}

uint32_t syscall68k_DmGetDatabase(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                  uint16_t index) {
    return syscall68k(sd, flags, SYSCALL_68K_DM_GET_DATABASE, false, [=]() {
        pacePush16(index);
        pacePush16(cardNo);
    });
}

uint16_t syscall68k_DmDatabaseInfo(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                   uint32_t dbId, uint32_t nameP, uint32_t attributesP,
                                   uint32_t versionP, uint32_t crDateP, uint32_t modDateP,
                                   uint32_t bcpUpDateP, uint32_t modNumP, uint32_t appInfoIDP,
                                   uint32_t sortInfoIDP, uint32_t typeP, uint32_t creatorP) {
    return syscall68k(sd, flags, SYSCALL_68K_DM_DATABASE_INFO, false, [=]() {
        pacePush32(creatorP);
        pacePush32(typeP);
        pacePush32(sortInfoIDP);
        pacePush32(appInfoIDP);
        pacePush32(modNumP);
        pacePush32(bcpUpDateP);
        pacePush32(modDateP);
        pacePush32(crDateP);
        pacePush32(versionP);
        pacePush32(attributesP);
        pacePush32(nameP);
        pacePush32(dbId);
        pacePush16(cardNo);
    });
}

uint16_t syscall68k_DmDatabaseProtect(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                      uint32_t dbID, uint8_t protect) {
    return syscall68k(sd, flags, SYSCALL_68K_DM_DATABASE_PROTECT, false, [=]() {
        pacePush8(protect);
        pacePush32(dbID);
        pacePush16(cardNo);
    });
}

uint16_t syscall68k_DmDeleteDatabase(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                     uint32_t dbID) {
    return syscall68k(sd, flags, SYSCALL_68K_DM_DELETE_DATABASE, false, [=]() {
        pacePush32(dbID);
        pacePush16(cardNo);
    });
}
