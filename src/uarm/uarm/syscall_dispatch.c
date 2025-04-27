#include "syscall_dispatch.h"

#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "cputil.h"
#include "pace.h"
#include "syscall.h"
#include "syscall_68k.h"

#define MAX_NEST_LEVEL 4

struct SyscallDispatch {
    struct SoC* soc;

    struct ArmCpu* scratchStates[MAX_NEST_LEVEL];
    struct PaceScratchState* scratchStatesPace[MAX_NEST_LEVEL];
    size_t nestLevel;
};

struct SyscallDispatch* initSyscallDispatch(struct SoC* soc) {
    struct SyscallDispatch* sd = malloc(sizeof(*sd));
    memset(sd, 0, sizeof(*sd));

    sd->soc = soc;

    return sd;
}

bool syscallDispatchM68kSupport(struct SyscallDispatch* sd) { return socIsPacePatched(sd->soc); }

bool syscallDispatchPrepare(struct SyscallDispatch* sd) {
    if (!syscallDispatchM68kSupport(sd)) return false;

    return socRunToPaceSyscall(sd->soc, SYSCALL_68K_EVT_GET_EVENT, 50000000, 50000000);
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

static size_t allocateScratchState(struct SyscallDispatch* sd) {
    if (sd->nestLevel >= MAX_NEST_LEVEL)
        ERR("unable to dispatch syscall: max nest level reached\n");

    const uint32_t nestLevel = sd->nestLevel++;

    sd->scratchStates[nestLevel] =
        cpuPrepareInjectedCall(socGetCpu(sd->soc), sd->scratchStates[nestLevel]);
    sd->scratchStatesPace[nestLevel] = pacePrepareInjectedCall(sd->scratchStatesPace[nestLevel]);

    return nestLevel;
}

static void disposeScratchState(struct SyscallDispatch* sd, size_t nestLevel) {
    if (sd->nestLevel != nestLevel + 1) ERR("bad nest level");

    cpuFinishInjectedCall(socGetCpu(sd->soc), sd->scratchStates[nestLevel]);
    paceFinishInjectedCall(sd->scratchStatesPace[nestLevel]);
    sd->nestLevel--;
}

uint16_t syscall_SysSetAutoOffTime(struct SyscallDispatch* sd, uint32_t timeout) {
    const size_t nestLevel = allocateScratchState(sd);
    uint32_t* registers = cpuGetRegisters(sd->scratchStates[nestLevel]);

    registers[0] = timeout;
    cpuExecuteInjectedCall(sd->scratchStates[nestLevel], SYSCALL_SYS_SET_AUTO_OFF_TIME);

    uint16_t err = registers[0];

    disposeScratchState(sd, nestLevel);

    return err;
}

uint32_t syscall68k_SysGetOsVersionString(struct SyscallDispatch* sd) {
    const size_t nestLevel = allocateScratchState(sd);

    cpuExecuteSyscall68k(sd->scratchStates[nestLevel], SYSCALL_68K_SYS_GET_OS_VERSION_STRING);
    const uint32_t result = paceGetAreg(0);

    disposeScratchState(sd, nestLevel);

    return result;
}

uint16_t syscall68k_MemPtrFree(struct SyscallDispatch* sd, uint32_t memPtr) {
    const size_t nestLevel = allocateScratchState(sd);

    paceResetFsr();
    pacePush32(memPtr);
    if (paceGetFsr() > 0) ERR("memory fault during injected PACE call");

    cpuExecuteSyscall68k(sd->scratchStates[nestLevel], SYSCALL_68K_MEM_CHUNK_FREE);
    const uint16_t result = paceGetDreg(0);

    disposeScratchState(sd, nestLevel);

    return result;
}
