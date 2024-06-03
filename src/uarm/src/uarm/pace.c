#include "pace.h"

#include <stdlib.h>

#include "mem.h"
#include "memcpy.h"
#include "uae/UAE.h"
#include "uarm_endian.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

static struct ArmMem* mem = NULL;
static struct ArmMmu* mmu = NULL;

static uint_fast8_t fsr = 0;
static uint32_t lastAddr = 0;
static bool wasWrite = false;

static uint32_t pendingStatus = 0;
static uint32_t statePtr;
static bool priviledged = false;

#ifdef __EMSCRIPTEN__
static cpuop_func* cpufunctbl_base;
#else
static cpuop_func* cpufunctbl[65536];  // (normally in newcpu.c)
#endif

static uint32_t pace_get_le(uint32_t addr, uint8_t size) {
    if (fsr != 0) return 0;

    lastAddr = addr;
    wasWrite = false;

    MMUTranslateResult translateResult = mmuTranslate(mmu, addr, priviledged, false);

    if (!MMU_TRANSLATE_RESULT_OK(translateResult)) {
        fsr = MMU_TRANSLATE_RESULT_FSR(translateResult);
        return 0;
    }

    const uint32_t pa = MMU_TRANSLATE_RESULT_PA(translateResult);

    uint32_t result = 0;
    bool ok = memAccess(mem, pa, size, false, &result);

    if (!ok) {
        fsr = 10;  // external abort on non-linefetch
        return 0;
    }

    return result;
}

uint8_t uae_get8(uint32_t addr) { return pace_get_le(addr, 1); }

uint16_t uae_get16(uint32_t addr) {
    if (!fsr && addr & 0x01) {
        fsr = 10;
        return 0;
    }

    return htobe16(pace_get_le(addr, 2));
}

static uint32_t uae_get32_split(uint32_t addr) {
    if ((addr & 0x3ff) <= (0x3ff - 4)) {
        if (fsr != 0) return 0;

        lastAddr = addr;
        wasWrite = false;

        MMUTranslateResult translateResult = mmuTranslate(mmu, addr, priviledged, false);

        if (!MMU_TRANSLATE_RESULT_OK(translateResult)) {
            fsr = MMU_TRANSLATE_RESULT_FSR(translateResult);
            return 0;
        }

        const uint32_t pa = MMU_TRANSLATE_RESULT_PA(translateResult);
        uint32_t val_le;

        if (!memAccess(mem, pa, 2, false, &val_le)) {
            fsr = 10;
            return 0;
        }

        lastAddr += 2;

        if (!memAccess(mem, pa + 2, 2, false, (uint8_t*)(&val_le) + 2)) {
            fsr = 10;
            return 0;
        }

        return htobe32(val_le);
    } else {
        return htobe32(pace_get_le(addr, 2) | (pace_get_le(addr + 2, 2) << 16));
    }
}

uint32_t uae_get32(uint32_t addr) {
    switch (__builtin_ctz(addr)) {
        case 0:
            fsr = 1;
            lastAddr = addr;
            wasWrite = false;
            return 0;

        case 1:
            return uae_get32_split(addr);

        default:
            return htobe32(pace_get_le(addr, 4));
    }
}

static void pace_put_le(uint32_t addr, uint32_t value, uint8_t size) {
    if (fsr != 0) return;

    lastAddr = addr;
    wasWrite = true;

    // fprintf(stderr, "%u byte write %#010x to %#010x\n", (uint32_t)size, value, addr);

    MMUTranslateResult translateResult = mmuTranslate(mmu, addr, priviledged, true);

    if (!MMU_TRANSLATE_RESULT_OK(translateResult)) {
        fsr = MMU_TRANSLATE_RESULT_FSR(translateResult);
        return;
    }

    uint32_t pa = MMU_TRANSLATE_RESULT_PA(translateResult);

    bool ok = memAccess(mem, pa, size, true, &value);

    if (!ok) {
        fsr = 10;  // external abort on non-linefetch
    }
}

void uae_put8(uint32_t addr, uint8_t value) { pace_put_le(addr, value, 1); };

void uae_put16(uint32_t addr, uint16_t value) {
    if (!fsr && addr & 0x01) {
        fsr = 1;
        return;
    }

    pace_put_le(addr, be16toh(value), 2);
}

static void put_get32_split(uint32_t addr, uint32_t value) {
    value = htobe32(value);

    if ((addr & 0x3ff) <= (0x3ff - 4)) {
        if (fsr != 0) return;

        lastAddr = addr;
        wasWrite = true;

        MMUTranslateResult translateResult = mmuTranslate(mmu, addr, priviledged, false);

        if (!MMU_TRANSLATE_RESULT_OK(translateResult)) {
            fsr = MMU_TRANSLATE_RESULT_FSR(translateResult);
            return;
        }

        const uint32_t pa = MMU_TRANSLATE_RESULT_PA(translateResult);

        if (!memAccess(mem, pa, 2, true, &value)) {
            fsr = 10;
            return;
        }

        lastAddr += 2;

        if (!memAccess(mem, pa + 2, 2, true, (uint8_t*)(&value) + 2)) fsr = 10;
    } else {
        pace_put_le(addr, value, 2);
        pace_put_le(addr + 2, value >> 16, 2);
    }
}

void uae_put32(uint32_t addr, uint32_t value) {
    switch (__builtin_ctz(addr)) {
        case 0:
            fsr = 1;
            lastAddr = addr;
            wasWrite = true;
            break;

        case 1:
            put_get32_split(addr, value);
            break;

        default:
            pace_put_le(addr, be32toh(value), 4);
            break;
    }
}

void Exception(int exception, uaecptr lastPc) {
    pendingStatus = exception;
    if (exception == pace_status_syscall) regs.pc += 2;
}

unsigned long op_unimplemented(uint32_t opcode) REGPARAM {
    pendingStatus = pace_status_unimplemented_instr;
    regs.pc += 2;

    return 0;
}

unsigned long op_illg(uint32_t opcode) REGPARAM {
    pendingStatus = pace_status_illegal_instr;
    regs.pc += 2;

    return 0;
}

unsigned long op_line1111(uint32_t opcode) REGPARAM {
    pendingStatus = pace_status_line_1111;
    regs.pc += 2;

    return 0;
}

unsigned long op_line1010(uint32_t opcode) REGPARAM {
    pendingStatus = pace_status_line_1010;
    regs.pc += 2;

    return 0;
}

void notifiyReturn() { pendingStatus = pace_status_return; }

static void staticInit() {
    static bool initialized = false;
    if (initialized) return;

    int i, j;
    for (i = 0; i < 256; i++) {
        for (j = 0; j < 8; j++) {
            if (i & (1 << j)) {
                break;
            }
        }

        movem_index1[i] = j;
        movem_index2[i] = 7 - j;
        movem_next[i] = i & (~(1 << j));
    }

    read_table68k();
    do_merges();

    // The rest of this code is based on build_cpufunctbl in newcpu.c.

#ifdef __EMSCRIPTEN__
    cpuop_func** cpufunctbl =
        (cpuop_func**)malloc(0x10000 * sizeof(cpuop_func*));  // (normally in newcpu.c)
#endif

    unsigned long opcode;
    struct cputbl* tbl = op_smalltbl_3;

    for (opcode = 0; opcode < 65536; opcode++) {
        if (opcode >> 12 == 0x0f)
            cpufunctbl[opcode] = op_line1111;
        else if (opcode >> 12 == 0x0a)
            cpufunctbl[opcode] = op_line1010;
        else
            cpufunctbl[opcode] = op_illg;
    }

    for (i = 0; tbl[i].handler != NULL; i++)
        if (!tbl[i].specific) cpufunctbl[tbl[i].opcode] = tbl[i].handler;

    for (opcode = 0; opcode < 65536; opcode++) {
        cpuop_func* f;

        if (table68k[opcode].mnemo == i_ILLG || table68k[opcode].clev > 0) continue;

        if (table68k[opcode].handler != -1) {
            f = cpufunctbl[table68k[opcode].handler];
            if (f == op_illg) {
                abort();
            }

            cpufunctbl[opcode] = f;
        }
    }

    for (i = 0; tbl[i].handler != NULL; i++)
        if (tbl[i].specific) cpufunctbl[tbl[i].opcode] = tbl[i].handler;

#ifdef __EMSCRIPTEN__
    cpufunctbl_base = (cpuop_func*)EM_ASM_INT(
        {
            wasmTable.grow(0x10000);

            for (let i = 0; i <= 0xffff; i++)
                wasmTable.set(wasmTable.length - 0xffff - 1 + i,
                              wasmTable.get(HEAPU32[($0 >>> 2) + i]));

            return wasmTable.length - 0xffff - 1;
        },
        cpufunctbl);

    free(cpufunctbl);
#endif

    // (hey readcpu doesn't free this guy!)
    free(table68k);

    initialized = true;
}

void paceInit(struct ArmMem* _mem, struct ArmMmu* _mmu) {
    staticInit();

    mem = _mem;
    mmu = _mmu;
}

void paceSetStatePtr(uint32_t addr) { statePtr = addr; }

uint8_t paceGetFsr() { return fsr; }

uint16_t paceGetLastOpcode() { return regs.lastOpcode; }

bool paceLoad68kState() {
    static uint32_t stateScratchBuffer[19];

    uint8_t* state = (sizeof(struct regstruct) == sizeof(stateScratchBuffer))
                         ? (uint8_t*)&regs
                         : (uint8_t*)stateScratchBuffer;

    struct MemcpyResult result;
    memcpy_armToHost(state, statePtr, sizeof(stateScratchBuffer), priviledged, mem, mmu, &result);

    if (!result.ok) {
        lastAddr = result.faultAddr;
        fsr = result.fsr;
        wasWrite = result.wasWrite;

        return false;
    }

    if (sizeof(struct regstruct) != sizeof(stateScratchBuffer)) {
        for (size_t i = 1; i < 17; i++) regs.regs[i - 1] = stateScratchBuffer[i];

        regs.pc = stateScratchBuffer[17];
        regs.sr = stateScratchBuffer[18];
    }

    MakeFromSR();

    return true;
}

bool paceSave68kState() {
    static uint32_t stateScratchBuffer[19];
    uint8_t* state;

    MakeSR();

    if (sizeof(struct regstruct) != sizeof(stateScratchBuffer)) {
        stateScratchBuffer[0] = regs.lastOpcode;

        for (size_t i = 1; i < 17; i++) stateScratchBuffer[i] = regs.regs[i - 1];

        stateScratchBuffer[17] = regs.pc;
        stateScratchBuffer[18] = regs.sr;

        state = (uint8_t*)stateScratchBuffer;
    } else {
        state = (uint8_t*)&regs;
    }

    struct MemcpyResult result;
    memcpy_hostToArm(statePtr, state, sizeof(stateScratchBuffer), priviledged, mem, mmu, &result);

    if (!result.ok) {
        lastAddr = result.faultAddr;
        fsr = result.fsr;
        wasWrite = result.wasWrite;

        return false;
    }

    return true;
}

void paceGetMemeryFault(uint32_t* _addr, bool* _wasWrite, uint_fast8_t* _fsr) {
    *_addr = lastAddr;
    *_wasWrite = wasWrite;
    *_fsr = fsr;
}

uint16_t paceReadTrapWord() {
    fsr = 0;
    return uae_get16(regs.pc - 2);
}

void paceSetPriviledged(bool _priviledged) { priviledged = _priviledged; }

enum paceStatus paceExecute() {
    fsr = 0;
    pendingStatus = pace_status_ok;

    uint16_t opcode = uae_get16(regs.pc);
    regs.lastOpcode = opcode;

    if (fsr != 0) return pace_status_memory_fault;

        // fprintf(stderr, "execute m68k opcode %#06x at %#010x\n", opcode, regs.pc);

#ifdef __EMSCRIPTEN__
    ((cpuop_func*)((long)cpufunctbl_base + opcode))(opcode);
#else
    cpufunctbl[opcode](opcode);
#endif

    //    fprintf(stderr, "a7 now %#010x, top of stack is %#010x\n", m68k_areg(regs, 7),
    //            uae_get32(m68k_areg(regs, 7)));

    return fsr == 0 ? pendingStatus : pace_status_memory_fault;
}
