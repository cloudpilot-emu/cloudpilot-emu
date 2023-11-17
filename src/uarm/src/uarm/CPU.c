//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "CPU.h"

#include <stdlib.h>
#include <string.h>

#include "MMU.h"
#include "cp15.h"
#include "gdbstub.h"
#include "icache.h"
#include "mem.h"
#include "pace.h"
#include "util.h"

#define xstr(s) str(s)
#define str(s) #s

#define unlikely(x) __builtin_expect((x), 0)
#define likely(x) __builtin_expect((x), 1)

#define USE_ICACHE
#define NO_SUPPORT_FCSE

#define ARM_MODE_2_REG 0x0F
#define ARM_MODE_2_WORD 0x10
#define ARM_MODE_2_LOAD 0x20
#define ARM_MODE_2_T 0x40
#define ARM_MODE_2_INV 0x80

#define ARM_MODE_3_REG 0x0F   // flag for actual reg number used
#define ARM_MODE_3_TYPE 0x30  // flag for the below 4 types
#define ARM_MODE_3_H 0x00
#define ARM_MODE_3_SH 0x10
#define ARM_MODE_3_SB 0x20
#define ARM_MODE_3_D 0x30
#define ARM_MODE_3_LOAD 0x40
#define ARM_MODE_3_INV 0x80

#define ARM_MODE_4_REG 0x0F
#define ARM_MODE_4_INC 0x10  // incr or decr
#define ARM_MODE_4_BFR 0x20  // before or after
#define ARM_MODE_4_WBK 0x40  // writeback?
#define ARM_MODE_4_S 0x80    // S bit set?

#define ARM_MODE_5_REG 0x0F
#define ARM_MODE_5_IS_OPTION 0x10  // is value option (as opposed to offset)
#define ARM_MODE_5_RR 0x20         // MCRR or MRCC instrs

#define REG_NO_SP 13
#define REG_NO_LR 14
#define REG_NO_PC 15

#define INJECTED_CALL_LR_MAGIC 0xfffffffc
#define INJECTED_CALL_MAX_CYCLES 200000000

#define cpuPrvGetRegNotPC(cpu, reg) (cpu->regs[reg])

/*

        coprocessors:

                                0    - DSP (pxa only)
                                0, 1 - WMMX (pxa only)
                                11   - VFP (arm standard)
                                15   - system control (arm standard)
*/

struct ArmBankedRegs {
    uint32_t R13, R14;
    uint32_t SPSR;  // usr mode doesn't have an SPSR
};

struct ArmCpu {
    uint32_t regs[16];  // current active regs as per current mode
    uint32_t SPSR;
    bool N, Z, C, V, Q, T, I, F;
    uint8_t M;

    uint32_t curInstrPC;

    struct ArmBankedRegs bank_usr;  // usr regs when in another mode
    struct ArmBankedRegs bank_svc;  // svc regs when in another mode
    struct ArmBankedRegs bank_abt;  // abt regs when in another mode
    struct ArmBankedRegs bank_und;  // und regs when in another mode
    struct ArmBankedRegs bank_irq;  // irq regs when in another mode
    struct ArmBankedRegs bank_fiq;  // fiq regs when in another mode
    uint32_t extra_regs[5];  // fiq regs when not in fiq mode, usr regs when in fiq mode. R8-12

    uint16_t waitingIrqs;
    uint16_t waitingFiqs;
    uint16_t CPAR;

    struct ArmCoprocessor coproc[16];  // coprocessors

    // various other cpu config options
    uint32_t vectorBase;  // address of vector base

    uint32_t pid;  // for fcse

    bool isInjectedCall;

    struct icache *ic;
    struct ArmMmu *mmu;
    struct ArmMem *mem;
    struct ArmCP15 *cp15;

    struct PacePatch *pacePatch;
    bool modePace;

    struct stub *debugStub;
    struct PatchDispatch *patchDispatch;
};

static uint32_t *table_thumb2arm = NULL;

static uint32_t cpuPrvClz(uint32_t val) {
    if (!val) return 32;

    if (sizeof(int) == sizeof(uint32_t)) return __builtin_clz(val);

    if (sizeof(long) == sizeof(uint32_t)) return __builtin_clzl(val);

    if (sizeof(long long) == sizeof(uint32_t)) return __builtin_clzll(val);

    ERR("CLZ undefined");
}

static uint32_t cpuPrvROR(uint32_t val, uint_fast8_t ror) {
    if (ror) val = (val >> ror) | (val << (32 - ror));

    return val;
}

static void cpuPrvSetPC(struct ArmCpu *cpu, uint32_t pc)  // with interworking
{
    cpu->regs[REG_NO_PC] = pc & ~1UL;
    cpu->T = (pc & 1);

    if (!cpu->T && (pc & 2)) ERR("Attempt to branch to non-word-aligned ARM address");
}

static uint32_t cpuPrvGetReg(struct ArmCpu *cpu, uint_fast8_t reg, bool wasT) {
    uint32_t ret;

    ret = cpu->regs[reg];
    if (reg == REG_NO_PC) {
        ret += wasT ? 2 : 4;
    }

    return ret;
}

static void cpuPrvSetRegNotPC(struct ArmCpu *cpu, uint_fast8_t reg, uint32_t val) {
    cpu->regs[reg] = val;
}

static void cpuPrvSetReg(struct ArmCpu *cpu, uint_fast8_t reg, uint32_t val) {
    if (reg == REG_NO_PC)
        cpuPrvSetPC(cpu, val);
    else
        cpuPrvSetRegNotPC(cpu, reg, val);
}

static struct ArmBankedRegs *cpuPrvModeToBankedRegsPtr(struct ArmCpu *cpu, uint_fast8_t mode) {
    switch (mode) {
        case ARM_SR_MODE_USR:
        case ARM_SR_MODE_SYS:
            return &cpu->bank_usr;

        case ARM_SR_MODE_FIQ:
            return &cpu->bank_fiq;

        case ARM_SR_MODE_IRQ:
            return &cpu->bank_irq;

        case ARM_SR_MODE_SVC:
            return &cpu->bank_svc;

        case ARM_SR_MODE_ABT:
            return &cpu->bank_abt;

        case ARM_SR_MODE_UND:
            return &cpu->bank_und;

        default:
            ERR("Invalid mode passed to cpuPrvModeToBankedRegsPtr()");
            return NULL;
    }
}

static void cpuPrvSwitchToMode(struct ArmCpu *cpu, uint_fast8_t newMode) {
    struct ArmBankedRegs *saveTo, *getFrom;
    uint_fast8_t i, curMode;
    uint32_t tmp;

    curMode = cpu->M;
    if (curMode == newMode) return;

    if (curMode == ARM_SR_MODE_FIQ || newMode == ARM_SR_MODE_FIQ) {  // bank/unbank the fiq regs

        for (i = 0; i < 5; i++) {
            tmp = cpu->extra_regs[i];
            cpu->extra_regs[i] = cpu->regs[i + 8];
            cpu->regs[i + 8] = tmp;
        }
    }

    saveTo = cpuPrvModeToBankedRegsPtr(cpu, curMode);
    getFrom = cpuPrvModeToBankedRegsPtr(cpu, newMode);

    if (saveTo == getFrom)
        return;  // we're done if no regs to switch [this happens if we switch user<->system]

    saveTo->R13 = cpu->regs[REG_NO_SP];
    saveTo->R14 = cpu->regs[REG_NO_LR];
    saveTo->SPSR = cpu->SPSR;

    cpu->regs[REG_NO_SP] = getFrom->R13;
    cpu->regs[REG_NO_LR] = getFrom->R14;
    cpu->SPSR = getFrom->SPSR;

    cpu->M = newMode;
}

static void cpuPrvSetPSRlo8(struct ArmCpu *cpu, uint_fast8_t val) {
    cpuPrvSwitchToMode(cpu, val & ARM_SR_M);
    cpu->T = !!(val & ARM_SR_T);
    cpu->F = !!(val & ARM_SR_F);
    cpu->I = !!(val & ARM_SR_I);
}

static void cpuPrvSetPSRhi8(struct ArmCpu *cpu, uint32_t val) {
    cpu->N = !!(val & ARM_SR_N);
    cpu->Z = !!(val & ARM_SR_Z);
    cpu->C = !!(val & ARM_SR_C);
    cpu->V = !!(val & ARM_SR_V);
    cpu->Q = !!(val & ARM_SR_Q);
}

static uint32_t cpuPrvMaterializeCPSR(struct ArmCpu *cpu) {
    uint32_t ret = 0;

    if (cpu->N) ret |= ARM_SR_N;
    if (cpu->Z) ret |= ARM_SR_Z;
    if (cpu->C) ret |= ARM_SR_C;
    if (cpu->V) ret |= ARM_SR_V;
    if (cpu->Q) ret |= ARM_SR_Q;
    if (cpu->T) ret |= ARM_SR_T;
    if (cpu->I) ret |= ARM_SR_I;
    if (cpu->F) ret |= ARM_SR_F;
    ret |= cpu->M;

    return ret;
}

uint32_t cpuGetRegExternal(struct ArmCpu *cpu, uint_fast8_t reg) {
    if (reg < 16)  // real reg
        return (reg == REG_NO_PC) ? (cpu->curInstrPC + (cpu->T ? 1 : 0)) : cpu->regs[reg];
    else if (reg == ARM_REG_NUM_CPSR)
        return cpuPrvMaterializeCPSR(cpu);
    else if (reg == ARM_REG_NUM_SPSR)
        return cpu->SPSR;
    else
        return 0;
}

void cpuSetReg(struct ArmCpu *cpu, uint_fast8_t reg, uint32_t val) {
    if (reg == ARM_REG_NUM_CPSR) {
        cpuPrvSetPSRlo8(cpu, val);
        cpuPrvSetPSRhi8(cpu, val);
    } else if (reg < 16)
        cpuPrvSetReg(cpu, reg, val);
}

#define cpuSetReg _DO_NOT_USE_cpuSetReg_IN_CPU_C_

static void cpuPrvException(struct ArmCpu *cpu, uint32_t vector_pc, uint32_t lr,
                            uint_fast8_t newLowBits)  // enters arm mode
{
    if (cpu->modePace) {
        if (!paceSave68kState()) {
            uint32_t addr;
            bool wasWrite;
            uint_fast8_t sz, fsr;

            paceGetMemeryFault(&addr, &wasWrite, &sz, &fsr);

            fprintf(stderr,
                    "ignoreing memory fault in PACE during save68kState: %s, sz=%u, addr=%#010x, "
                    "fsr=%#04x\n",
                    wasWrite ? "write" : "read", (uint32_t)sz, addr, fsr);
        }

        cpu->modePace = false;
    }

    uint32_t spsr = cpuPrvMaterializeCPSR(cpu);

    cpuPrvSetPSRlo8(cpu, newLowBits);
    cpu->SPSR = spsr;
    cpu->regs[REG_NO_LR] = lr;
    cpu->regs[REG_NO_PC] = vector_pc;
}

// input addr is VA not MVA
static void cpuPrvHandleMemErr(struct ArmCpu *cpu, uint32_t addr, uint_fast8_t sz, bool write,
                               bool instrFetch, uint_fast8_t fsr) {
// fprintf(stderr, "access error to 0x%08x with fsr %u from 0x%08x\n", addr, fsr,
// cpu->regs[REG_NO_PC]);

// FCSE
#ifdef SUPPORT_FCSE
    if (addr < 0x02000000UL)  // report addr is MVA
        addr |= cpu->pid;
#endif

    cp15SetFaultStatus(cpu->cp15, addr, fsr);

    if (instrFetch) {
        // handle prefetch abort (LR is addr of aborted instr + 4)
        cpuPrvException(cpu, cpu->vectorBase + ARM_VECTOR_OFFT_P_ABT, cpu->curInstrPC + 4,
                        ARM_SR_MODE_ABT | ARM_SR_I);
    } else {
        // handle data abort (LR is addr of aborted instr + 8)
        cpuPrvException(cpu, cpu->vectorBase + ARM_VECTOR_OFFT_D_ABT, cpu->curInstrPC + 8,
                        ARM_SR_MODE_ABT | ARM_SR_I);
    }
}

static uint32_t cpuPrvArmAdrMode_1(struct ArmCpu *cpu, uint32_t instr, bool *carryOutP, bool wasT) {
    uint_fast8_t v, a;
    bool co = cpu->C;  // be default carry out = C flag
    uint32_t ret;

    if (instr & 0x02000000UL) {  // immed

        v = (instr >> 7) & 0x1E;
        ret = cpuPrvROR(instr & 0xFF, v);
        if (v) co = !!(ret & 0x80000000UL);
    } else {
        v = (instr >> 5) & 3;                         // get shift type
        ret = cpuPrvGetReg(cpu, instr & 0x0F, wasT);  // get Rm

        if (instr & 0x00000010UL) {  // reg with reg shift

            a = cpuPrvGetRegNotPC(
                cpu, (instr >> 8) & 0x0F);  // get the relevant part of Rs, we only care for
                                            // lower 8 bits (note we use uint8 for this)

            if (a != 0) {  // else all is already good

                switch (v) {  // perform shifts

                    case 0:  // LSL

                        if (a < 32) {
                            co = (ret >> (32 - a)) & 1;
                            ret = ret << a;
                        } else if (a == 32) {
                            co = ret & 1;
                            ret = 0;
                        } else {  // >32
                            co = 0;
                            ret = 0;
                        }
                        break;

                    case 1:  // LSR

                        if (a < 32) {
                            co = (ret >> (a - 1)) & 1;
                            ret = ret >> a;
                        } else if (a == 32) {
                            co = ret >> 31;
                            ret = 0;
                        } else {  // >32
                            co = 0;
                            ret = 0;
                        }
                        break;

                    case 2:  // ASR

                        if (a < 32) {
                            co = (ret >> (a - 1)) & 1;
                            ret = ((int32_t)ret >> a);
                        } else {  // >=32
                            if (ret & 0x80000000UL) {
                                co = 1;
                                ret = 0xFFFFFFFFUL;
                            } else {
                                co = 0;
                                ret = 0;
                            }
                        }
                        break;

                    case 3:  // ROR

                        if (a == 0) {
                            // nothing...
                        } else {
                            a &= 0x1F;
                            if (a == 0)
                                co = ret >> 31;
                            else {
                                co = (ret >> (a - 1)) & 1;
                                ret = cpuPrvROR(ret, a);
                            }
                        }
                        break;
                }
            }
        } else {  // reg with immed shift

            a = (instr >> 7) & 0x1F;  // get imm

            switch (v) {
                case 0:  // LSL

                    if (a == 0) {
                        // nothing
                    } else {
                        co = (ret >> (32 - a)) & 1;
                        ret = ret << a;
                    }
                    break;

                case 1:  // LSR

                    if (a == 0) {
                        co = ret >> 31;
                        ret = 0;
                    } else {
                        co = (ret >> (a - 1)) & 1;
                        ret = ret >> a;
                    }
                    break;

                case 2:  // ASR

                    if (a == 0) {
                        if (ret & 0x80000000UL) {
                            co = 1;
                            ret = 0xFFFFFFFFUL;
                        } else {
                            co = 0;
                            ret = 0;
                        }
                    } else {
                        co = (ret >> (a - 1)) & 1;
                        if (ret & 0x80000000UL)
                            ret = (ret >> a) | (0xFFFFFFFFUL << (32 - a));
                        else
                            ret = ret >> a;
                    }
                    break;

                case 3:  // ROR or RRX

                    if (a == 0) {  // RRX
                        a = co;
                        co = ret & 1;
                        ret = ret >> 1;
                        if (a) ret |= 0x80000000UL;
                    } else {
                        co = (ret >> (a - 1)) & 1;
                        ret = cpuPrvROR(ret, a);
                    }
                    break;
            }
        }
    }

    *carryOutP = co;
    return ret;
}

/*
idea:

addbefore is what to add to add to base reg before addressing, addafter is what to add after. we
ALWAYS do writeback, but if not requested by instr, it will be zero

for [Rx, 5]   baseReg = x addbefore = 5 addafter = -5
for [Rx, 5]!  baseReg = x addBefore = 0 addafter = 0
for [Rx], 5   baseReg = x addBefore = 0 addAfter = 5

t = T bit (LDR vs LDRT)

baseReg is returned in return val along with flags:


ARM_MODE_2_REG	is mask for reg
ARM_MODE_2_WORD	is flag for word access
ARM_MODE_2_LOAD	is flag for load
ARM_MODE_2_INV	is flag for invalid instructions
ARM_MODE_2_T	is flag for T


*/
static uint_fast8_t cpuPrvArmAdrMode_2(struct ArmCpu *cpu, uint32_t instr, uint32_t *addBeforeP,
                                       uint32_t *addWritebackP, bool wasT) {
    uint_fast8_t reg, shift;
    uint32_t val;

    reg = (instr >> 16) & 0x0F;

    if (!(instr & 0x02000000UL))  // immediate
        val = instr & 0xFFFUL;
    else {  //[scaled] register

        if (instr & 0x00000010UL)
            reg |= ARM_MODE_2_INV;  // invalid instrucitons need to be
                                    // reported

        val = cpuPrvGetRegNotPC(cpu, instr & 0x0F);
        shift = (instr >> 7) & 0x1F;
        switch ((instr >> 5) & 3) {
            case 0:  // LSL
                val <<= shift;
                break;

            case 1:  // LSR
                val = shift ? (val >> shift) : 0;
                break;

            case 2:  // ASR
                val = shift ? (uint32_t)((((int32_t)val) >> shift))
                            : ((val & 0x80000000UL) ? 0xFFFFFFFFUL : 0x00000000UL);
                break;

            case 3:  // ROR/RRX
                if (shift)
                    val = cpuPrvROR(val, shift);
                else {  // RRX
                    val = val >> 1;
                    if (cpu->C) val |= 0x80000000UL;
                }
        }
    }

    if (!(instr & 0x00400000UL)) reg |= ARM_MODE_2_WORD;
    if (instr & 0x00100000UL) reg |= ARM_MODE_2_LOAD;
    if (!(instr & 0x00800000UL)) val = -val;
    if (!(instr & 0x01000000UL)) {
        *addBeforeP = 0;
        *addWritebackP = val;

        if (instr & 0x00200000UL) reg |= ARM_MODE_2_T;
    } else if (instr & 0x00200000UL) {
        *addBeforeP = val;
        *addWritebackP = val;
    } else {
        *addBeforeP = val;
        *addWritebackP = 0;
    }

    return reg;
}

/*
same comments as for addr mode 2 apply

#define ARM_MODE_3_REG	0x0F	//flag for actual reg number used
#define ARM_MODE_3_TYPE	0x30	//flag for the below 4 types
#define ARM_MODE_3_H	0x00
#define ARM_MODE_3_SH	0x10
#define ARM_MODE_3_SB	0x20
#define ARM_MODE_3_D	0x30
#define ARM_MODE_3_LOAD	0x40
#define ARM_MODE_3_INV	0x80
*/

static uint_fast8_t cpuPrvArmAdrMode_3(struct ArmCpu *cpu, uint32_t instr, uint32_t *addBeforeP,
                                       uint32_t *addWritebackP, bool wasT) {
    uint_fast8_t reg;
    uint32_t val;
    bool S, H, L;

    reg = (instr >> 16) & 0x0F;

    if (instr & 0x00400000UL)  // immediate
        val = ((instr >> 4) & 0xF0) | (instr & 0x0F);
    else {
        if (instr & 0x00000F00UL) reg |= ARM_MODE_3_INV;  // bits 8-11 must be 1 always
        val = cpuPrvGetRegNotPC(cpu, instr & 0x0F);
    }

    L = !!(instr & 0x00100000UL);
    H = !!(instr & 0x00000020UL);
    S = !!(instr & 0x00000040UL);

    if (S && H)
        reg |= ARM_MODE_3_SH;
    else if (S)
        reg |= ARM_MODE_3_SB;
    else if (H)
        reg |= ARM_MODE_3_H;
    else
        reg |= ARM_MODE_3_INV;  // S == 0 && H == 0 is invalid mode 3 operation

    if ((instr & 0x00000090UL) != 0x00000090UL)
        reg |= ARM_MODE_3_INV;  // bits 4 and 7 must be 1 always

    if (S && !L) {  // LDRD/STRD is encoded thusly

        reg = (reg & ~ARM_MODE_3_TYPE) | ARM_MODE_3_D;
        L = !H;
    }
    if (L) reg |= ARM_MODE_3_LOAD;
    if (!(instr & 0x00800000UL)) val = -val;
    if (!(instr & 0x01000000UL)) {
        *addBeforeP = 0;
        *addWritebackP = val;

        if (instr & 0x00200000UL)
            reg |= ARM_MODE_3_INV;  // W must be 0 in this case, else unpredictable (in this case -
                                    // invalid instr)
    } else if (instr & 0x00200000UL) {
        *addBeforeP = val;
        *addWritebackP = val;
    } else {
        *addBeforeP = val;
        *addWritebackP = 0;
    }

    return reg;
}

/*
        #define ARM_MODE_4_REG	0x0F
        #define ARM_MODE_4_INC	0x10	//incr or decr
        #define ARM_MODE_4_BFR	0x20	//after or before
        #define ARM_MODE_4_WBK	0x40	//writeback?
        #define ARM_MODE_4_S	0x80	//S bit set?
*/

static uint_fast8_t cpuPrvArmAdrMode_4(struct ArmCpu *cpu, uint32_t instr, bool usesUsrRegs,
                                       uint_fast16_t *regs) {
    uint_fast8_t reg;

    *regs = instr & 0xffff;

    reg = (instr >> 16) & 0x0F;
    if ((instr & 0x00400000UL) &&
        !usesUsrRegs)  // real hw ignores "S" in modes that use user mode regs
        reg |= ARM_MODE_4_S;
    if (instr & 0x00200000UL) reg |= ARM_MODE_4_WBK;
    if (instr & 0x00800000UL) reg |= ARM_MODE_4_INC;
    if (instr & 0x01000000UL) reg |= ARM_MODE_4_BFR;

    return reg;
}

/*
#define ARM_MODE_5_REG		0x0F
#define ARM_MODE_5_IS_OPTION	0x10	//is value option (as opposed to offset)
#define ARM_MODE_5_RR		0x20	//MCRR or MRCC instrs
*/
static uint_fast8_t cpuPrvArmAdrMode_5(struct ArmCpu *cpu, uint32_t instr, uint32_t *addBeforeP,
                                       uint32_t *addAfterP, uint8_t *optionValP) {
    uint_fast8_t reg;
    uint32_t val;

    val = instr & 0xFF;
    reg = (instr >> 16) & 0x0F;

    *addBeforeP = 0;
    *addAfterP = 0;
    *optionValP = 0;

    if (!(instr & 0x01000000UL)) {  // unindexed or postindexed

        if (instr & 0x00200000UL)  // postindexed
            *addAfterP = val;
        else {  // unindexed
            if (!(instr & 0x00800000UL))
                reg |= ARM_MODE_5_RR;  // U must be 1 for unindexed, else it is MCRR/MRCC
            *optionValP = val;
        }
    } else {  // offset or preindexed

        *addBeforeP = val;

        if (instr & 0x00200000UL)  // preindexed
            *addAfterP = val;
    }

    if (!(reg & ARM_MODE_5_IS_OPTION)) {
        if (!(instr & 0x00800000UL)) {
            *addBeforeP = -*addBeforeP;
            *addAfterP = -*addAfterP;
        }
    }
    return reg;
}

static void cpuPrvSetPSR(struct ArmCpu *cpu, uint_fast8_t mask, bool privileged, bool R,
                         uint32_t val) {
    if (R)  // setting SPSR in sys or usr mode is no harm since they arent used, so just do it
            // without any checks
        cpu->SPSR = val;
    else {
        if (privileged && (mask & 1)) cpuPrvSetPSRlo8(cpu, val);

        if (mask & 8) cpuPrvSetPSRhi8(cpu, val);
    }
}

static bool cpuPrvSignedAdditionOverflows(int32_t a, int32_t b, int32_t sum) {
    int32_t c;

    return __builtin_add_overflow_i32(a, b, &c);
}

static bool cpuPrvSignedSubtractionOverflows(int32_t a, int32_t b, int32_t diff)  // diff = a - b
{
    int32_t c;

    return __builtin_sub_overflow_i32(a, b, &c);
}

static int32_t cpuPrvMedia_signedSaturate32(int32_t sign) {
    return (sign < 0) ? -0x80000000UL : 0x7ffffffful;
}

static bool cpuPrvMemOpEx(struct ArmCpu *cpu, void *buf, uint32_t vaddr, uint_fast8_t size,
                          bool write, bool priviledged, uint_fast8_t *fsrP) {
    uint32_t pa;

    gdbStubReportMemAccess(cpu->debugStub, vaddr, size, write);

    if (size & (size - 1)) {  // size is not a power of two
        if (fsrP) *fsrP = 1;  // alignment fault;
        return false;
    }

    if (vaddr & (size - 1)) {  // bad alignment
        if (fsrP) *fsrP = 1;   // alignment fault;
        return false;
    }

// FCSE
#ifdef SUPPORT_FCSE
    if (vaddr < 0x02000000UL) vaddr |= cpu->pid;
#endif

    struct ArmMemRegion *region;

    if (!mmuTranslate(cpu->mmu, vaddr, priviledged, write, &pa, fsrP, NULL, &region)) return false;

    bool ok = region ? region->aF(region->uD, pa, size, write, buf)
                     : memAccess(cpu->mem, pa, size,
                                 (write ? MEM_ACCESS_TYPE_WRITE : MEM_ACCESS_TYPE_READ), buf);

    if (!ok) {
        if (fsrP) *fsrP = 10;  // external abort on non-linefetch

        return false;
    }

    return true;
}

// for internal use
static bool cpuPrvMemOp(struct ArmCpu *cpu, void *buf, uint32_t vaddr, uint_fast8_t size,
                        bool write, bool priviledged, uint_fast8_t *fsrP) {
    if (cpuPrvMemOpEx(cpu, buf, vaddr, size, write, priviledged, fsrP)) return true;

    fprintf(stderr, "%c of %u bytes to 0x%08lx failed!\n", (int)(write ? 'W' : 'R'), (unsigned)size,
            (unsigned long)vaddr);
    gdbStubDebugBreakRequested(cpu->debugStub);

    return false;
}

// for external use
bool cpuMemOpExternal(struct ArmCpu *cpu, void *buf, uint32_t vaddr, uint_fast8_t size,
                      bool write)  // for external use
{
    return cpuPrvMemOpEx(cpu, buf, vaddr, size, write, true, NULL);
}

#ifdef SUPPORT_AXIM_PRINTF
static uint32_t cpuPrvAximSysPrintfGetParam(struct ArmCpu *cpu, uint32_t *atP) {
    uint32_t at = *atP;

    *atP += 4;

    return cpuPrvMemOp(cpu, &at, at, 4, false, true, NULL) ? at : 0;
}

static void cpuPrvAximSysPrintf(struct ArmCpu *cpu) {
    if (cpu->curInstrPC == 0x8006EE64UL) {
        uint32_t a = cpu->regs[0] / 2, params = cpu->regs[1], ptr;
        bool infmt = false;
        uint16_t v;

        while (cpuPrvMemOp(cpu, &v, a++ * 2, 2, false, true, NULL) && v) {
            if (infmt) switch (v) {
                    case '%':
                        fprintf(stderr, "%%");
                        infmt = false;
                        break;

                    case 'a':
                        fprintf(stderr, "0x%08x", cpuPrvAximSysPrintfGetParam(cpu, &params));
                        infmt = false;
                        break;

                    case '0' ... '9':
                    case 'l':
                    case '.':
                        break;

                    case 'd':
                        fprintf(stderr, "%d", cpuPrvAximSysPrintfGetParam(cpu, &params));
                        infmt = false;
                        break;

                    case 'u':
                        fprintf(stderr, "%u", cpuPrvAximSysPrintfGetParam(cpu, &params));
                        infmt = false;
                        break;

                    case 'x':
                    case 'X':
                        fprintf(stderr, "%x", cpuPrvAximSysPrintfGetParam(cpu, &params));
                        infmt = false;
                        break;

                    case 's':
                        ptr = cpuPrvAximSysPrintfGetParam(cpu, &params) / 2;
                        while (cpuPrvMemOp(cpu, &v, ptr++ * 2, 2, false, true, NULL) && v)
                            fprintf(stderr, "%c", v);
                        infmt = false;
                        break;

                    default:
                        fprintf(stderr, "unknown format modifier %%%c", v);
                        infmt = false;
                        break;
                }
            else if (v == '%')
                infmt = true;
            else
                fprintf(stderr, "%c", v);
        }
    }
}
#endif

static uint64_t cpuPrv64FromHalves(uint64_t hi, uint32_t lo) {
    // better than shifting in almost all compilers
    union {
        struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
            uint32_t lo;
            uint32_t hi;
#elif __BYTE_ORDER == __BIG_ENDIAN
            uint32_t hi;
            uint32_t lo;
#else
    #error "endianness undefined"
#endif
        };
        uint64_t val;
    } t;

    t.hi = hi;
    t.lo = lo;

    return t.val;
}

static bool cpuPrvSignedSubtractionWithPossibleCarryOverflows(uint32_t a, uint32_t b,
                                                              uint32_t diff)  // diff = a - b
{
    return ((a ^ b) & (a ^ diff)) >> 31;
}

static bool cpuPrvSignedAdditionWithPossibleCarryOverflows(uint32_t a, uint32_t b, uint32_t sum) {
    return ((a ^ b ^ 0x80000000UL) & (a ^ sum)) >> 31;
}

static void handlePaceMemoryFault(struct ArmCpu *cpu) {
    uint32_t addr;
    bool wasWrite;
    uint_fast8_t sz, fsr;

    paceGetMemeryFault(&addr, &wasWrite, &sz, &fsr);
    cpuPrvHandleMemErr(cpu, addr, sz, wasWrite, false, fsr);
}

// PACE entrypoint was called from ARM: regular invocation
static void handlePaceEnter(struct ArmCpu *cpu) {
    bool privileged = cpu->M != ARM_SR_MODE_USR;
    uint_fast8_t fsr = 0;

    cpu->regs[REG_NO_SP] -= 4;
    if (!cpuPrvMemOp(cpu, &cpu->regs[REG_NO_LR], cpu->regs[REG_NO_SP], 4, true, privileged, &fsr))
        return cpuPrvHandleMemErr(cpu, cpu->regs[REG_NO_SP], 4, true, false, fsr);

    cpu->regs[REG_NO_SP] -= 4;
    if (!cpuPrvMemOp(cpu, &cpu->regs[0], cpu->regs[REG_NO_SP], 4, true, privileged, &fsr))
        return cpuPrvHandleMemErr(cpu, cpu->regs[REG_NO_SP], 4, true, false, fsr);

    paceSetPriviledged(privileged);
    paceSetStatePtr(cpu->regs[0]);

    if (!paceLoad68kState()) return handlePaceMemoryFault(cpu);

    cpu->modePace = true;

    fprintf(stderr, "enter PACE\n");

    return;
}

// PACE execution was resumed from ARM: resume after interrupt / exception
static void handlePaceResume(struct ArmCpu *cpu) {
    paceSetPriviledged(cpu->M != ARM_SR_MODE_USR);
    paceSetStatePtr(cpu->regs[0]);

    if (!paceLoad68kState()) return handlePaceMemoryFault(cpu);

    cpu->modePace = true;

    ERR("resume PACE\n");
}

// PACE was reentered after a callout
static void handlePaceReturnFromCallout(struct ArmCpu *cpu) {
    bool privileged = cpu->M != ARM_SR_MODE_USR;
    uint_fast8_t fsr = 0;

    // restore r0 / state pointer from stack
    if (!cpuPrvMemOp(cpu, &cpu->regs[0], cpu->regs[REG_NO_SP], 4, false, privileged, &fsr))
        return cpuPrvHandleMemErr(cpu, cpu->regs[REG_NO_SP], 4, false, false, fsr);

    paceSetPriviledged(privileged);
    paceSetStatePtr(cpu->regs[0]);

    if (!paceLoad68kState()) return handlePaceMemoryFault(cpu);

    cpu->modePace = true;

    ERR("PACE return from callout\n");
}

static bool handleInvalidInstruction(struct ArmCpu *cpu, uint32_t instr) {
    if (instr != INSTR_PACE) return false;

    uint32_t pa;
    struct ArmMemRegion *region;
    if (!mmuTranslate(cpu->mmu, cpu->curInstrPC, true, false, &pa, NULL, NULL, &region))
        return false;

    if (pa == cpu->pacePatch->enterPace) {
        handlePaceEnter(cpu);
        return true;
    } else if (pa == cpu->pacePatch->resumePace) {
        handlePaceResume(cpu);
        return true;
    } else if (pa == cpu->pacePatch->returnFromCallout) {
        handlePaceReturnFromCallout(cpu);
        return true;
    }

    return false;
}

#ifdef SUPPORT_Z72_PRINTF
static uint32_t cpuPrvZ72sysPrintfGetParam(struct ArmCpu *cpu, uint32_t *paramIdxP) {
    uint32_t idx = (*paramIdxP)++, val;

    if (idx < 4) return cpu->regs[idx];

    idx -= 4;
    cpuPrvMemOp(cpu, &val, cpu->regs[REG_NO_SP] + idx * 4, 4, false, true, NULL);

    return val;
}

static void cpuPrvZ72sysPrintf(struct ArmCpu *cpu) {
    if (cpu->curInstrPC == 0x200959BCUL) {
        uint32_t a = cpu->regs[0], paramIdx = 1, ptr;
        bool infmt = false;
        char v;

        while (cpuPrvMemOp(cpu, &v, a++, 1, false, true, NULL) && v) {
            if (infmt) switch (v) {
                    case '%':
                        fprintf(stderr, "%%");
                        infmt = false;
                        break;

                    case 'a':
                        fprintf(stderr, "0x%08x", cpuPrvZ72sysPrintfGetParam(cpu, &paramIdx));
                        infmt = false;
                        break;

                    case '0' ... '9':
                    case 'l':
                    case '.':
                        break;

                    case 'd':
                        fprintf(stderr, "%d", cpuPrvZ72sysPrintfGetParam(cpu, &paramIdx));
                        infmt = false;
                        break;

                    case 'u':
                        fprintf(stderr, "%u", cpuPrvZ72sysPrintfGetParam(cpu, &paramIdx));
                        infmt = false;
                        break;

                    case 'x':
                    case 'X':
                        fprintf(stderr, "%x", cpuPrvZ72sysPrintfGetParam(cpu, &paramIdx));
                        infmt = false;
                        break;

                    case 's':
                        ptr = cpuPrvZ72sysPrintfGetParam(cpu, &paramIdx);
                        while (cpuPrvMemOp(cpu, &v, ptr++, 1, false, true, NULL) && v)
                            fprintf(stderr, "%c", v);
                        infmt = false;
                        break;

                    default:
                        fprintf(stderr, "unknown format modifier %%%c", v);
                        infmt = false;
                        break;
                }
            else if (v == '%')
                infmt = true;
            else
                fprintf(stderr, "%c", v);
        }
    }
}
#endif

static void cpuPrvExecInstr(struct ArmCpu *cpu, uint32_t instr, bool wasT, bool privileged) {
    uint32_t op1, op2, res, sr, ea, memVal32, addBefore, addAfter;
    bool specialInstr = false, ok;
    uint_fast8_t mode, cpNo, fsr, sourceReg, destReg;
    uint_fast16_t regsList;
    uint16_t memVal16;
    uint8_t memVal8;
    uint64_t res64;

#ifdef SUPPORT_AXIM_PRINTF
    cpuPrvAximSysPrintf(cpu);
#endif

#ifdef SUPPORT_Z72_PRINTF
    cpuPrvZ72sysPrintf(cpu);
#endif

    switch (instr >> 28) {
        case 0:  // EQ
            if (!cpu->Z) return;
            break;

        case 1:  // NE
            if (cpu->Z) return;
            break;

        case 2:  // CS
            if (!cpu->C) return;
            break;

        case 3:  // CC
            if (cpu->C) return;
            break;

        case 4:  // MI
            if (!cpu->N) return;
            break;

        case 5:  // PL
            if (cpu->N) return;
            break;

        case 6:  // VS
            if (!cpu->V) return;
            break;

        case 7:  // VC
            if (cpu->V) return;
            break;

        case 8:  // HI
            if (!cpu->C || cpu->Z) return;
            break;

        case 9:  // LS
            if (cpu->C && !cpu->Z) return;
            break;

        case 10:  // GE
            if (cpu->N != cpu->V) return;
            break;

        case 11:  // LT
            if (cpu->N == cpu->V) return;
            break;

        case 12:  // GT
            if (cpu->Z || cpu->N != cpu->V) return;
            break;

        case 13:  // LE
            if (!cpu->Z && cpu->N == cpu->V) return;
            break;

        case 14:  // AL
            break;

        case 15:  // NV
            specialInstr = true;

            switch ((instr >> 24) & 0x0f) {
                case 5:
                case 7:
                    // PLD
                    if ((instr & 0x0D70F000UL) == 0x0550F000UL) goto instr_done;
                    goto invalid_instr;

                case 10:
                case 11:
                    goto b_bl_blx;

                case 12:
                case 13:
                    goto coproc_mem_2reg;

                case 14:
                    goto coproc_dp;

                default:
                    goto invalid_instr;
            }
            break;
    }

    switch ((instr >> 24) & 0x0F) {
        case 0:
        case 1:  // data processing immediate shift, register shift and misc instrs and mults
            if ((instr & 0x00000090UL) == 0x00000090) {  // multiplies, extra load/stores
                                                         // (table 3.2)

                if ((instr & 0x00000060UL) == 0x00000000) {  // swp[b], mult(acc), mult(acc) long

                    if (instr & 0x01000000UL) {  // SWB/SWPB

                        switch ((instr >> 20) & 0x0F) {
                            case 0:  // SWP

                                ea = cpuPrvGetRegNotPC(cpu, (instr >> 16) & 0x0F);
                                ok = cpuPrvMemOp(cpu, &memVal32, ea, 4, false, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 4, false, false, fsr);
                                    goto instr_done;
                                }
                                op1 = memVal32;
                                memVal32 = cpuPrvGetRegNotPC(cpu, instr & 0x0F);
                                ok = cpuPrvMemOp(cpu, &memVal32, ea, 4, true, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 4, true, false, fsr);
                                    goto instr_done;
                                }
                                cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F, op1);
                                break;

                            case 4:  // SWPB

                                ea = cpuPrvGetRegNotPC(cpu, (instr >> 16) & 0x0F);
                                ok = cpuPrvMemOp(cpu, &memVal8, ea, 1, false, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 1, false, false, fsr);
                                    goto instr_done;
                                }
                                op1 = memVal8;
                                memVal8 = cpuPrvGetRegNotPC(cpu, instr & 0x0F);
                                ok = cpuPrvMemOp(cpu, &memVal8, ea, 1, true, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 1, true, false, fsr);
                                    goto instr_done;
                                }
                                cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F, op1);
                                break;

                            default:
                                goto invalid_instr;
                        }

                    } else
                        switch ((instr >> 20) & 0x0F) {  // multiplies

                            case 0:  // MUL
                            case 1:

                                res = 0;
                                if (instr & 0x0000F000UL) goto invalid_instr;
                                goto mul32;

                            case 2:  // MLA
                            case 3:

                                res = cpuPrvGetRegNotPC(cpu, (instr >> 12) & 0x0F);
                            mul32:
                                res += cpuPrvGetRegNotPC(cpu, (instr >> 8) & 0x0F) *
                                       cpuPrvGetRegNotPC(cpu, instr & 0x0F);
                                cpuPrvSetRegNotPC(cpu, (instr >> 16) & 0x0F, res);
                                if (instr & 0x00100000UL) {  // S

                                    cpu->Z = !res;
                                    cpu->N = res >> 31;
                                }
                                goto instr_done;

                            case 8:  // UMULL
                            case 9:
                            case 12:  // SMULL
                            case 13:

                                res64 = 0;
                                goto mul64;

                            case 10:  // UMLAL
                            case 11:
                            case 14:  // SMLAL
                            case 15:

                                res64 = cpuPrv64FromHalves(
                                    cpuPrvGetRegNotPC(cpu, (instr >> 16) & 0x0F),
                                    cpuPrvGetRegNotPC(cpu, (instr >> 12) & 0x0F));

                            mul64:
                                op1 = cpuPrvGetRegNotPC(cpu, (instr >> 8) & 0x0F);
                                op2 = cpuPrvGetRegNotPC(cpu, instr & 0x0F);

                                if (instr & 0x00400000UL)
                                    res64 += (int64_t)(int32_t)op1 * (int64_t)(int32_t)op2;
                                else
                                    res64 += (uint64_t)(uint32_t)op1 * (uint64_t)(uint32_t)op2;

                                cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F, res64);
                                cpuPrvSetRegNotPC(cpu, (instr >> 16) & 0x0F, res64 >> 32);

                                if (instr & 0x00100000UL) {  // S

                                    cpu->Z = !res64;
                                    cpu->N = res64 >> 63;
                                }
                                break;

                            default:
                                goto invalid_instr;
                        }
                } else {  // load/store signed/unsigned byte/halfword/two_words

                    uint32_t doubleMem[2];

                    mode = cpuPrvArmAdrMode_3(cpu, instr, &addBefore, &addAfter, wasT);
                    ea = cpuPrvGetReg(cpu, mode & ARM_MODE_3_REG, wasT);
                    ea += addBefore;

                    if (mode & ARM_MODE_3_LOAD) {
                        switch (mode & ARM_MODE_3_TYPE) {
                            case ARM_MODE_3_H:

                                ok = cpuPrvMemOp(cpu, &memVal16, ea, 2, false, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 2, false, false, fsr);
                                    goto instr_done;
                                }
                                cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F, memVal16);
                                break;

                            case ARM_MODE_3_SH:

                                ok = cpuPrvMemOp(cpu, &memVal16, ea, 2, false, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 2, false, false, fsr);
                                    goto instr_done;
                                }
                                cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F,
                                                  (int32_t)(int16_t)memVal16);
                                break;

                            case ARM_MODE_3_SB:

                                ok = cpuPrvMemOp(cpu, &memVal8, ea, 1, false, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 1, false, false, fsr);
                                    goto instr_done;
                                }
                                cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F,
                                                  (int32_t)(int8_t)memVal8);
                                break;

                            case ARM_MODE_3_D:

                                ok = cpuPrvMemOp(cpu, doubleMem, ea, 8, false, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 8, false, false, fsr);
                                    goto instr_done;
                                }

                                cpuPrvSetRegNotPC(cpu, ((instr >> 12) & 0x0F) + 0, doubleMem[0]);
                                cpuPrvSetRegNotPC(cpu, ((instr >> 12) & 0x0F) + 1, doubleMem[1]);
                                break;
                        }
                    } else {
                        switch (mode & ARM_MODE_3_TYPE) {
                            case ARM_MODE_3_H:

                                memVal16 = cpuPrvGetReg(cpu, (instr >> 12) & 0x0F, wasT);
                                ok = cpuPrvMemOp(cpu, &memVal16, ea, 2, true, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 2, true, false, fsr);
                                    goto instr_done;
                                }
                                break;

                            case ARM_MODE_3_SH:
                            case ARM_MODE_3_SB:
                                goto invalid_instr;

                            case ARM_MODE_3_D:

                                doubleMem[0] = cpuPrvGetRegNotPC(cpu, ((instr >> 12) & 0x0F) + 0);
                                doubleMem[1] = cpuPrvGetRegNotPC(cpu, ((instr >> 12) & 0x0F) + 1);
                                ok = cpuPrvMemOp(cpu, doubleMem, ea, 8, true, privileged, &fsr);
                                if (!ok) {
                                    cpuPrvHandleMemErr(cpu, ea, 8, true, false, fsr);
                                    goto instr_done;
                                }
                                break;
                        }
                    }
                    if (addAfter)
                        cpuPrvSetRegNotPC(cpu, mode & ARM_MODE_3_REG, ea - addBefore + addAfter);
                }
                goto instr_done;
            } else if ((instr & 0x01900000UL) == 0x01000000UL) {  // misc instrs (table 3.3)

                switch ((instr >> 4) & 0x0F) {
                    case 0:  // move reg to PSR or move PSR to reg

                        if ((instr & 0x00BF0FFFUL) == 0x000F0000UL) {  // move PSR to reg

                            // access in user and sys mode is undefined. for us that means returning
                            // garbage that is currently in "cpu->SPSR"
                            cpuPrvSetRegNotPC(
                                cpu, (instr >> 12) & 0x0F,
                                (instr & 0x00400000UL) ? cpu->SPSR : cpuPrvMaterializeCPSR(cpu));
                        } else if ((instr & 0x00B0FFF0UL) == 0x0020F000UL) {  // move reg to PSR

                            cpuPrvSetPSR(cpu, (instr >> 16) & 0x0F, privileged,
                                         !!(instr & 0x00400000UL),
                                         cpuPrvGetReg(cpu, instr & 0x0F, wasT));
                        } else
                            goto invalid_instr;
                        goto instr_done;

                    case 1:  // BLX/BX/BXJ or CLZ
                    case 3:

                        if (instr & 0x00400000UL) {  // CLZ

                            cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F,
                                              cpuPrvClz(cpuPrvGetRegNotPC(cpu, instr & 0xF)));
                        } else {  // BL / BLX / BXJ

                            if ((instr & 0x0FFFFF00UL) != 0x012FFF00UL) goto invalid_instr;

                            if ((instr & 0x00000030UL) == 0x00000030UL)
                                cpuPrvSetRegNotPC(
                                    cpu, REG_NO_LR,
                                    cpu->curInstrPC + (wasT ? 3 : 4));  // save return value for BLX
                            cpuPrvSetPC(cpu, cpuPrvGetReg(cpu, instr & 0x0F, wasT));
                        }
                        goto instr_done;

                    case 5:  // enhanced DSP adds/subtracts

                        if (instr & 0x00000F00UL) goto invalid_instr;
                        op1 = cpuPrvGetRegNotPC(cpu, instr & 0x0F);          // Rm
                        op2 = cpuPrvGetRegNotPC(cpu, (instr >> 16) & 0x0F);  // Rn
                        switch ((instr >> 21) & 3) {                         // what op?

                            case 0:  // QADD

                                res = op1 + op2;
                                if (cpuPrvSignedAdditionOverflows(op1, op2, res)) {
                                    cpu->Q = 1;
                                    res = cpuPrvMedia_signedSaturate32(op1);
                                }
                                break;

                            case 1:  // QSUB

                                res = op1 - op2;
                                if (cpuPrvSignedAdditionOverflows(op1, op2, res)) {
                                    cpu->Q = 1;
                                    res = cpuPrvMedia_signedSaturate32(op1);
                                }
                                break;
                            case 2:  // QDADD

                                res = op2 + op2;
                                if (cpuPrvSignedAdditionOverflows(op2, op2, res)) {
                                    cpu->Q = 1;
                                    res = cpuPrvMedia_signedSaturate32(op2);
                                }
                                op2 = res;

                                res = op1 + op2;
                                if (cpuPrvSignedAdditionOverflows(op1, op2, res)) {
                                    cpu->Q = 1;
                                    res = cpuPrvMedia_signedSaturate32(op1);
                                }
                                break;

                            case 3:  // QDSUB

                                res = op2 + op2;
                                if (cpuPrvSignedAdditionOverflows(op2, op2, res)) {
                                    cpu->Q = 1;
                                    res = cpuPrvMedia_signedSaturate32(op2);
                                }
                                op2 = res;

                                res = op1 - op2;
                                if (cpuPrvSignedAdditionOverflows(op1, op2, res)) {
                                    cpu->Q = 1;
                                    res = cpuPrvMedia_signedSaturate32(op1);
                                }
                                break;

                            default:
                                __builtin_unreachable();
                                res = 0;
                                break;
                        }
                        cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F, res);
                        goto instr_done;

                    case 7:  // soft breakpoint

                        cpuPrvException(cpu, cpu->vectorBase + ARM_VECTOR_OFFT_P_ABT,
                                        cpu->curInstrPC + 4, ARM_SR_MODE_ABT | ARM_SR_I);
                        goto instr_done;

                    case 8:  // enhanced DSP multiplies
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                        if ((instr & 0x00000090UL) != 0x00000080UL) goto invalid_instr;
                        op1 = cpuPrvGetRegNotPC(cpu, instr & 0x0F);         // Rm
                        op2 = cpuPrvGetRegNotPC(cpu, (instr >> 8) & 0x0F);  // Rs
                        switch ((instr >> 21) & 3) {                        // what op?
                            case 0:                                         // SMLAxy
                                if (instr & 0x00000020UL)
                                    op1 >>= 16;
                                else
                                    op1 = (uint16_t)op1;

                                if (instr & 0x00000040UL)
                                    op2 >>= 16;
                                else
                                    op2 = (uint16_t)op2;

                                res = (int32_t)(int16_t)op1 * (int32_t)(int16_t)op2;
                                op1 = res;
                                op2 = cpuPrvGetRegNotPC(cpu, (instr >> 12) & 0x0F);  // Rn
                                res = op1 + op2;
                                if (cpuPrvSignedAdditionOverflows(op1, op2, res)) cpu->Q = 1;
                                cpuPrvSetRegNotPC(cpu, (instr >> 16) & 0x0F, res);
                                break;

                            case 1:  // SMLAWy/SMULWy

                                if (instr & 0x00000040UL)
                                    op2 >>= 16;
                                else
                                    op2 = (uint16_t)op2;

                                res = (((int64_t)(int32_t)op1 * (int64_t)(int16_t)op2) >> 16);

                                if (instr & 0x00000020UL) {  // SMULWy
                                    if (instr & 0x0000F000UL) goto invalid_instr;
                                } else {  // SMLAWy

                                    op1 = res;
                                    op2 = cpuPrvGetRegNotPC(cpu, (instr >> 12) & 0x0F);  // Rn
                                    res = op1 + op2;
                                    if (cpuPrvSignedAdditionOverflows(op1, op2, res)) cpu->Q = 1;
                                }
                                cpuPrvSetRegNotPC(cpu, (instr >> 16) & 0x0F, res);
                                break;

                            case 2:  // SMLALxy

                                if (instr & 0x00000020UL)
                                    op1 >>= 16;
                                else
                                    op1 = (uint16_t)op1;

                                if (instr & 0x00000040UL)
                                    op2 >>= 16;
                                else
                                    op2 = (uint16_t)op2;

                                res64 = cpuPrv64FromHalves(
                                    cpuPrvGetRegNotPC(cpu, (instr >> 16) & 0x0F),
                                    cpuPrvGetRegNotPC(cpu, (instr >> 12) & 0x0F));
                                res64 += (int32_t)(int16_t)op1 * (int32_t)(int16_t)op2;

                                cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F, res64);
                                cpuPrvSetRegNotPC(cpu, (instr >> 16) & 0x0F, res64 >> 32);
                                break;

                            case 3:  // SMULxy

                                if (instr & 0x0000F000UL) goto invalid_instr;

                                if (instr & 0x00000020UL)
                                    op1 >>= 16;
                                else
                                    op1 = (uint16_t)op1;

                                if (instr & 0x00000040UL)
                                    op2 >>= 16;
                                else
                                    op2 = (uint16_t)op2;

                                res = (int32_t)(int16_t)op1 * (int32_t)(int16_t)op2;
                                cpuPrvSetRegNotPC(cpu, (instr >> 16) & 0x0F, res);
                                break;
                        }
                        goto instr_done;

                    default:
                        goto invalid_instr;
                }
            }

            goto data_processing;
            break;

        case 2:
        case 3:  // data process immediate val, move imm to SR

        data_processing:  // data processing
        {
            bool cOut, setFlags = !!(instr & 0x00100000UL);

            op2 = cpuPrvArmAdrMode_1(cpu, instr, &cOut, wasT);

            switch ((instr >> 21) & 0x0F) {
                case 0:  // AND
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 & op2;
                    break;

                case 1:  // EOR
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 ^ op2;
                    break;

                case 2:  // SUB
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 - op2;
                    if (setFlags) {
                        cpu->V = cpuPrvSignedSubtractionOverflows(op1, op2, res);
                        cOut = !__builtin_sub_overflow_u32(op1, op2, &res);
                    }
                    break;

                case 3:  // RSB
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op2 - op1;
                    if (setFlags) {
                        cpu->V = cpuPrvSignedSubtractionOverflows(op2, op1, res);
                        cOut = !__builtin_sub_overflow_u32(op2, op1, &res);
                    }
                    break;

                case 4:  // ADD
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 + op2;
                    if (setFlags) {
                        cpu->V = cpuPrvSignedAdditionOverflows(op1, op2, res);
                        cOut = __builtin_add_overflow_u32(op1, op2, &res);
                    }
                    break;

                case 5:  // ADC
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = res64 = (uint64_t)op1 + op2 + (cpu->C ? 1 : 0);
                    if (setFlags) {  // hard to get this right in C in 32 bits so go to 64...
                        cOut = res64 >> 32;
                        cpu->V = cpuPrvSignedAdditionWithPossibleCarryOverflows(op1, op2, res);
                        cpu->V = (res64 >> 31) == 1 || (res64 >> 31) == 2;
                    }
                    break;

                case 6:  // SBC
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = res64 = (uint64_t)op1 - op2 - (cpu->C ? 0 : 1);
                    if (setFlags) {  // hard to get this right in C in 32 bits so go to 64...
                        cOut = !(res64 >> 32);
                        cpu->V = cpuPrvSignedSubtractionWithPossibleCarryOverflows(op1, op2, res);
                    }
                    break;

                case 7:  // RSC
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = res64 = (uint64_t)op2 - op1 - (cpu->C ? 0 : 1);
                    if (setFlags) {  // hard to get this right in C in 32 bits so go to 64...
                        cOut = !(res64 >> 32);
                        cpu->V = cpuPrvSignedSubtractionWithPossibleCarryOverflows(op2, op1, res);
                    }
                    break;

                case 8:  // TST
                    if (!setFlags) goto invalid_instr;
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 & op2;
                    goto dp_flag_set;

                case 9:               // TEQ
                    if (!setFlags) {  // MSR CPSR, imm

                        cpuPrvSetPSR(cpu, (instr >> 16) & 0x0F, privileged, false,
                                     cpuPrvROR(instr & 0xFF, ((instr >> 8) & 0x0F) * 2));
                        goto instr_done;
                    }
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 ^ op2;
                    goto dp_flag_set;

                case 10:  // CMP
                    if (!setFlags) goto invalid_instr;
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 - op2;
                    cpu->V = cpuPrvSignedSubtractionOverflows(op1, op2, res);
                    cOut = !__builtin_sub_overflow_u32(op1, op2, &res);
                    goto dp_flag_set;

                case 11:              // CMN
                    if (!setFlags) {  // MSR SPSR, imm

                        cpuPrvSetPSR(cpu, (instr >> 16) & 0x0F, privileged, true,
                                     cpuPrvROR(instr & 0xFF, ((instr >> 8) & 0x0F) * 2));
                        goto instr_done;
                    }
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 + op2;
                    cpu->V = cpuPrvSignedAdditionOverflows(op1, op2, res);
                    cOut = __builtin_add_overflow_u32(op1, op2, &res);
                    goto dp_flag_set;

                case 12:  // ORR
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 | op2;
                    break;

                case 13:  // MOV
                    res = op2;
                    break;

                case 14:  // BIC
                    op1 = cpuPrvGetReg(cpu, (instr >> 16) & 0x0F, wasT);
                    res = op1 & ~op2;
                    break;

                case 15:  // MVN
                    res = ~op2;
                    break;

                default:
                    __builtin_unreachable();
                    res = 0;
                    break;
            }

            if (!setFlags)  // simple store
                cpuPrvSetReg(cpu, (instr >> 12) & 0x0F, res);
            else if (((instr >> 12) & 0x0F) == REG_NO_PC) {  // copy SPSR to CPSR. we allow in user
                                                             // mode too - allowed and faster

                sr = cpu->SPSR;
                cpuPrvSetPSRlo8(cpu, sr);
                cpuPrvSetPSRhi8(cpu, sr);
                cpu->regs[REG_NO_PC] = res;  // do it right here - if we let it use cpuPrvSetReg, it
                                             // will check lower bit...
            } else {                         // store and set flags

                cpuPrvSetReg(cpu, (instr >> 12) & 0x0F, res);

            dp_flag_set:
                cpu->C = cOut;
                cpu->N = res >> 31;
                cpu->Z = !res;
            }
            goto instr_done;
        } break;

        case 4:
        case 5:  // load/store imm offset

            goto load_store_mode_2;
            break;

        case 6:
        case 7:  // load/store reg offset

            if (instr & 0x00000010UL)  // media and undefined instrs
                goto invalid_instr;

        load_store_mode_2:
            mode = cpuPrvArmAdrMode_2(cpu, instr, &addBefore, &addAfter, wasT);
            if (mode & ARM_MODE_2_INV) goto invalid_instr;
            if (mode & ARM_MODE_2_T) privileged = false;

            sourceReg = mode & ARM_MODE_2_REG;
            ea = cpuPrvGetReg(cpu, sourceReg, wasT);
            ea += addBefore;

            if (mode & ARM_MODE_2_LOAD) {
                if (mode & ARM_MODE_2_WORD) {
                    ok = cpuPrvMemOp(cpu, &memVal32, ea, 4, false, privileged, &fsr);
                    if (!ok) {
                        cpuPrvHandleMemErr(cpu, ea, 4, false, false, fsr);
                        goto instr_done;
                    }

                    destReg = (instr >> 12) & 0x0F;

                    // from RePalm:
                    //
                    // #define CALL_OSCALL(tab,num)	"LDR R12,[R9, #-" #tab "] \nLDR PC,[R12, #"
                    // #num "]"
                    if (sourceReg == 9 && destReg == 12)
                        patchDispatchOnLoadR12FromR9(cpu->patchDispatch, addBefore);
                    if (sourceReg == 12 && destReg == REG_NO_PC)
                        patchDispatchOnLoadPcFromR12(cpu->patchDispatch, addBefore, cpu->regs);

                    cpuPrvSetReg(cpu, destReg, memVal32);
                } else {
                    ok = cpuPrvMemOp(cpu, &memVal8, ea, 1, false, privileged, &fsr);
                    if (!ok) {
                        cpuPrvHandleMemErr(cpu, ea, 1, false, false, fsr);
                        goto instr_done;
                    }
                    cpuPrvSetRegNotPC(cpu, (instr >> 12) & 0x0F, memVal8);
                }
            } else {
                op1 = cpuPrvGetReg(cpu, (instr >> 12) & 0x0F, wasT);

                if (mode & ARM_MODE_2_WORD) {
                    memVal32 = op1;
                    ok = cpuPrvMemOp(cpu, &memVal32, ea, 4, true, privileged, &fsr);
                    if (!ok) {
                        cpuPrvHandleMemErr(cpu, ea, 4, true, false, fsr);
                        goto instr_done;
                    }
                } else {
                    memVal8 = op1;
                    ok = cpuPrvMemOp(cpu, &memVal8, ea, 1, true, privileged, &fsr);
                    if (!ok) {
                        cpuPrvHandleMemErr(cpu, ea, 1, true, false, fsr);
                        goto instr_done;
                    }
                }
            }
            if (addAfter) cpuPrvSetRegNotPC(cpu, mode & ARM_MODE_2_REG, ea - addBefore + addAfter);
            goto instr_done;

        case 8:
        case 9:  // load/store multiple
        {
            bool userModeRegs = false, copySPSR = false, isLoad = !!(instr & 0x00100000UL);
            uint32_t
                loadedPc = 0xfffffffful,
                origBaseRegVal;  // so we can restore on load failure, even if we loaded into it
            uint_fast8_t idx, regNo;

            mode = cpuPrvArmAdrMode_4(
                cpu, instr, (cpu->M == ARM_SR_MODE_USR) || (cpu->M == ARM_SR_MODE_SYS), &regsList);
            origBaseRegVal = ea = cpuPrvGetRegNotPC(cpu, mode & ARM_MODE_4_REG);
            if (mode & ARM_MODE_4_S) {  // sort out what "S" means
                if (isLoad && (regsList & (1 << REG_NO_PC)))
                    copySPSR = true;
                else
                    userModeRegs = true;
            }

            for (idx = 0; idx < 16; idx++) {
                regNo = (mode & ARM_MODE_4_INC) ? idx : 15 - idx;
                if (!(regsList & (1 << regNo))) continue;

                // if this is a store, get the value to store
                if (!isLoad) {
                    memVal32 = cpuPrvGetReg(cpu, regNo, wasT);
                    if (unlikely(userModeRegs)) {
                        if (regNo >= 8 && regNo <= 12 &&
                            (cpu->M == ARM_SR_MODE_FIQ))  // handle fiq/usr banked regs
                            memVal32 = cpu->extra_regs[regNo - 8];
                        else if (regNo == REG_NO_SP)
                            memVal32 = cpu->bank_usr.R13;
                        else if (regNo == REG_NO_LR)
                            memVal32 = cpu->bank_usr.R14;
                    }
                }

                // perform mem op
                if (mode & ARM_MODE_4_BFR) ea += (mode & ARM_MODE_4_INC) ? 4 : -4;
                ok = cpuPrvMemOp(cpu, &memVal32, ea, 4, !isLoad, privileged, &fsr);
                if (!ok) {
                    cpuPrvHandleMemErr(cpu, ea, 4, !isLoad, false, fsr);
                    if (regsList &
                        (1 << (mode &
                               ARM_MODE_4_REG)))  // restore base if we had already overwritten it
                        cpuPrvSetReg(cpu, mode & ARM_MODE_4_REG, origBaseRegVal);
                    goto instr_done;
                }
                if (!(mode & ARM_MODE_4_BFR)) ea += (mode & ARM_MODE_4_INC) ? 4 : -4;

                // if this is a load, store the value we just loaded
                if (isLoad) {
                    if (unlikely(userModeRegs)) {
                        if (regNo >= 8 && regNo <= 12 &&
                            (cpu->M == ARM_SR_MODE_FIQ)) {  // handle fiq/usr banked regs
                            cpu->extra_regs[regNo - 8] = memVal32;
                            continue;
                        } else if (regNo == REG_NO_SP) {
                            cpu->bank_usr.R13 = memVal32;
                            continue;
                        } else if (regNo == REG_NO_LR) {
                            cpu->bank_usr.R14 = memVal32;
                            continue;
                        }
                    }

                    if (unlikely(regNo == REG_NO_PC && copySPSR))
                        loadedPc = memVal32;
                    else
                        cpuPrvSetReg(cpu, regNo, memVal32);
                }
            }
            if (mode & ARM_MODE_4_WBK) cpuPrvSetRegNotPC(cpu, mode & ARM_MODE_4_REG, ea);
            if (copySPSR) {
                sr = cpu->SPSR;
                cpuPrvSetPSRlo8(cpu, sr);
                cpuPrvSetPSRhi8(cpu, sr);
                cpu->regs[REG_NO_PC] = loadedPc;  // direct write - yes
                if (cpu->T)
                    cpu->regs[REG_NO_PC] &= ~1;
                else
                    cpu->regs[REG_NO_PC] &= ~3;
            }
        }
            goto instr_done;

        case 10:
        case 11:  // B/BL/BLX(if cond=0b1111)

        b_bl_blx:
            ea = instr << 8;
            ea = ((int32_t)ea) >> 7;
            ea += 4;
            if (!wasT) ea <<= 1;
            ea += cpu->curInstrPC;

            if (specialInstr) {  // handle BLX
                if (instr & 0x01000000UL) ea += 2;
                cpu->regs[REG_NO_LR] = cpu->curInstrPC + (wasT ? 2 : 4);
                if (!cpu->T) ea |= 1UL;  // set T flag if needed
            } else {                     // not BLX -> differentiate between BL and B
                if (instr & 0x01000000UL) cpu->regs[REG_NO_LR] = cpu->curInstrPC + (wasT ? 2 : 4);
                if (cpu->T) ea |= 1UL;  // keep T flag as needed
            }
            cpuPrvSetPC(cpu, ea);
            goto instr_done;

        case 12:
        case 13:  // coprocessor load/store and double register transfers
        coproc_mem_2reg:
            cpNo = (instr >> 8) & 0x0F;

            mode = cpuPrvArmAdrMode_5(cpu, instr, &addBefore, &addAfter, &memVal8);

            if (cpNo >= 14) {  // cp14 and cp15 are for priviledged users only
                if (!privileged) goto invalid_instr;
            } else if (!(cpu->CPAR & (1UL << cpNo)))  // others are access-controlled by CPAR
                goto invalid_instr;

            if (mode & ARM_MODE_5_RR) {  // handle MCRR, MRCC

                if (!cpu->coproc[cpNo].twoRegF ||
                    !cpu->coproc[cpNo].twoRegF(cpu, cpu->coproc[cpNo].userData,
                                               !!(instr & 0x00100000UL), (instr >> 4) & 0x0F,
                                               (instr >> 12) & 0x0F, (instr >> 16) & 0x0F,
                                               instr & 0x0F))
                    goto invalid_instr;
            } else {  // handle LDC/STC

                if (!cpu->coproc[cpNo].memAccess ||
                    !cpu->coproc[cpNo].memAccess(
                        cpu, cpu->coproc[cpNo].userData, specialInstr, !!(instr & 0x00400000UL),
                        !(instr & 0x00100000UL), (instr >> 12) & 0x0F, mode & ARM_MODE_5_REG,
                        addBefore, addAfter, (mode & ARM_MODE_5_IS_OPTION) ? &memVal8 : NULL))
                    goto invalid_instr;
            }
            goto instr_done;

        case 14:  // coprocessor data processing and register transfers
        coproc_dp:
            cpNo = (instr >> 8) & 0x0F;

            if (cpNo >= 14) {  // cp14 and cp15 are for priviledged users only
                if (!privileged) goto invalid_instr;
            } else if (!(cpu->CPAR & (1UL << cpNo)))  // others are access-controlled by CPAR
                goto invalid_instr;

            if (instr & 0x00000010UL) {  // MCR[2]/MRC[2]

                if (!cpu->coproc[cpNo].regXfer ||
                    !cpu->coproc[cpNo].regXfer(cpu, cpu->coproc[cpNo].userData, specialInstr,
                                               !!(instr & 0x00100000UL), (instr >> 21) & 0x07,
                                               (instr >> 12) & 0x0F, (instr >> 16) & 0x0F,
                                               instr & 0x0F, (instr >> 5) & 0x07))
                    goto invalid_instr;
            } else {  // CDP

                if (!cpu->coproc[cpNo].dataProcessing ||
                    !cpu->coproc[cpNo].dataProcessing(cpu, cpu->coproc[cpNo].userData, specialInstr,
                                                      (instr >> 20) & 0x0F, (instr >> 12) & 0x0F,
                                                      (instr >> 16) & 0x0F, instr & 0x0F,
                                                      (instr >> 5) & 0x07))
                    goto invalid_instr;
            }
            goto instr_done;

        case 15:  // SWI

            // some semihosting support
            if ((wasT && (instr & 0x00fffffful) == 0xab) ||
                (!wasT && (instr & 0x00fffffful) == 0x123456ul)) {
                if (cpu->regs[0] == 4) {
                    uint32_t addr = cpu->regs[1];
                    uint8_t ch;

                    while (cpuPrvMemOp(cpu, &ch, addr++, 1, false, true, &fsr) && ch)
                        fprintf(stderr, "%c", ch);
                } else if (cpu->regs[0] == 3) {
                    uint8_t ch;

                    if (cpuPrvMemOp(cpu, &ch, cpu->regs[1], 1, false, true, &fsr) && ch)
                        fprintf(stderr, "%c", ch);
                } else if (cpu->regs[0] == 0x132) {
                    fprintf(stderr, "debug break requested\n");
                    gdbStubDebugBreakRequested(cpu->debugStub);
                }
                goto instr_done;
            }

            cpuPrvException(cpu, cpu->vectorBase + ARM_VECTOR_OFFT_SWI,
                            cpu->curInstrPC + (wasT ? 2 : 4), ARM_SR_MODE_SVC | ARM_SR_I);
            goto instr_done;
    }

invalid_instr:

    if (wasT || !handleInvalidInstruction(cpu, instr)) {
        fprintf(stderr, "Invalid instr 0x%08lx seen at 0x%08lx with CPSR 0x%08lx\n",
                (unsigned long)instr, (unsigned long)cpu->curInstrPC,
                (unsigned long)cpuPrvMaterializeCPSR(cpu));
        cpuPrvException(cpu, cpu->vectorBase + ARM_VECTOR_OFFT_UND,
                        cpu->curInstrPC + (wasT ? 2 : 4), ARM_SR_MODE_UND | ARM_SR_I);
    }

instr_done:
    return;
}

static void cpuPrvCycleArm(struct ArmCpu *cpu) {
    uint32_t instr, pc, fetchPc;
    bool privileged, ok;
    uint_fast8_t fsr;

    privileged = cpu->M != ARM_SR_MODE_USR;

    cpu->curInstrPC = cpu->regs[REG_NO_PC];  // needed for stub to get proper pc
    gdbStubReportPc(cpu->debugStub, cpu->regs[REG_NO_PC], true);  // early in case it changes PC

    // fetch instruction
    cpu->curInstrPC = fetchPc = pc = cpu->regs[REG_NO_PC];

// FCSE
#ifdef SUPPORT_FCSE
    if (fetchPc < 0x02000000UL) fetchPc |= cpu->pid;
#endif

#ifdef USE_ICACHE
    ok = icacheFetch(cpu->ic, fetchPc, 4, &fsr, &instr);
#else
    ok = cpuPrvMemOp(cpu, &instr, fetchPc, 4, false, privileged, &fsr);
#endif
    if (!ok)
        cpuPrvHandleMemErr(cpu, pc, 4, false, true, fsr);
    else {
        cpu->regs[REG_NO_PC] += 4;
        cpuPrvExecInstr(cpu, instr, false, privileged);
    }
}

static inline void cpuPrvExecThumb(struct ArmCpu *cpu, uint16_t instrT, bool privileged) {
#define THUMB_FAIL ERR("thumb opcode should be transcoded:" __FILE__ ":" str(__LINE__));
    uint32_t v32;
    uint16_t v16;
    uint_fast8_t v8;

    switch (instrT >> 12) {
        case 4:  // LDR(3) ADD(4) CMP(3) MOV(3) BX MVN CMP(2) CMN TST ADC SBC NEG MUL LSL(2) LSR(2)
                 // ASR(2) ROR AND EOR ORR BIC

            if (instrT & 0x0800) {  // LDR(3)
                const uint32_t pc = cpuPrvGetReg(cpu, REG_NO_PC, true) & ~0x03UL;
                const uint32_t addr = pc + ((instrT & 0xff) << 2);
                uint32_t memVal32;
                uint_fast8_t fsr;

                bool ok = cpuPrvMemOp(cpu, &memVal32, addr, 4, false, privileged, &fsr);
                if (!ok)
                    cpuPrvHandleMemErr(cpu, addr, 4, false, false, fsr);
                else
                    cpuPrvSetReg(cpu, (instrT >> 8) & 0x07, memVal32);

                return;
            } else if (instrT & 0x0400) {  // ADD(4) CMP(3) MOV(3) BX

                uint8_t vD;

                vD = (instrT & 7) | ((instrT >> 4) & 0x08);
                v8 = (instrT >> 3) & 0xF;

                switch ((instrT >> 8) & 3) {
                    case 0:  // ADD(4)

                        // special handling required for PC destination
                        v32 = cpuPrvGetReg(cpu, vD, true) + cpuPrvGetReg(cpu, v8, true);
                        if (vD == 15) v32 |= 1;
                        cpuPrvSetReg(cpu, vD, v32);

                        return;

                    case 2:  // MOV(3)

                        // special handling required for PC destination
                        v32 = cpuPrvGetReg(cpu, v8, true);
                        if (vD == 15) v32 |= 1;
                        cpuPrvSetReg(cpu, vD, v32);

                        return;

                    case 3:                 // BX
                        if (instrT & 0x80)  // BLX
                            cpu->regs[REG_NO_LR] = cpu->regs[REG_NO_PC] + 1;

                        v8 = (instrT >> 3) & 0x0F;
                        cpuPrvSetPC(cpu,
                                    v8 == 15 ? ((cpu->regs[REG_NO_PC] + 2) & ~3UL) : cpu->regs[v8]);

                        return;

                    default:
                        THUMB_FAIL;
                }
            } else {  // AND EOR LSL(2) LSR(2) ASR(2) ADC SBC ROR TST NEG CMP(2) CMN ORR MUL BIC MVN
                THUMB_FAIL;
            }

            break;

        case 10:  // ADD(5) ADD(6)	(bit11 set = add(6))
            cpuPrvSetReg(cpu, (instrT >> 8) & 0x07,
                         ((instrT & 0x0800) ? cpuPrvGetRegNotPC(cpu, REG_NO_SP)
                                            : (cpuPrvGetReg(cpu, REG_NO_PC, true) & ~0x03UL)) +
                             ((instrT & 0xff) << 2));

            return;

        case 14:  // B(2) BL BLX(1) undefined instr space
        case 15:
            v16 = (instrT & 0x7FF);

            switch ((instrT >> 11) & 3) {
                case 0:  // B(2)
                    THUMB_FAIL;

                case 1:  // BLX(1)_suffix
                    v32 = cpu->regs[REG_NO_PC];
                    cpu->regs[REG_NO_PC] =
                        (cpu->regs[REG_NO_LR] + 2 + (((uint32_t)v16) << 1)) & ~3UL;
                    cpu->regs[REG_NO_LR] = v32 | 1UL;
                    cpu->T = 0;

                    return;

                case 2:  // BLX(1)_prefix BL_prefix
                    v32 = v16;
                    if (instrT & 0x0400) v32 |= 0x000FF800UL;
                    cpu->regs[REG_NO_LR] = cpu->regs[REG_NO_PC] + (v32 << 12);

                    return;

                case 3:  // BL_suffix
                    v32 = cpu->regs[REG_NO_PC];
                    cpu->regs[REG_NO_PC] = cpu->regs[REG_NO_LR] + 2 + (((uint32_t)v16) << 1);
                    cpu->regs[REG_NO_LR] = v32 | 1UL;

                    return;

                default:
                    THUMB_FAIL;
            }
    }

    THUMB_FAIL;
#undef THUMB_FAIL
}

static void cpuPrvCycleThumb(struct ArmCpu *cpu) {
    bool privileged, ok;
    uint32_t pc, fetchPc;
    uint16_t instrT;
    uint32_t instrA;
    uint_fast8_t fsr;

    privileged = cpu->M != ARM_SR_MODE_USR;

    cpu->curInstrPC = cpu->regs[REG_NO_PC];  // needed for stub to get proper pc
    gdbStubReportPc(cpu->debugStub, cpu->regs[REG_NO_PC], true);  // early in case it changes PC

    cpu->curInstrPC = fetchPc = pc = cpu->regs[REG_NO_PC];

// FCSE
#ifdef SUPPORT_FCSE
    if (fetchPc < 0x02000000UL) fetchPc |= cpu->pid;
#endif

#ifdef USE_ICACHE
    ok = icacheFetch(cpu->ic, fetchPc, 2, &fsr, &instrT);
#else
    ok = cpuPrvMemOp(cpu, &instrT, fetchPc, 2, false, privileged, &fsr);
#endif
    if (!ok) {
        cpuPrvHandleMemErr(cpu, pc, 2, false, true, fsr);
        return;  // exit here so that debugger can see us execute first instr of execption handler
    }
    cpu->regs[REG_NO_PC] += 2;

    instrA = table_thumb2arm[instrT];
    if (instrA)
        cpuPrvExecInstr(cpu, instrA, true, privileged);
    else
        cpuPrvExecThumb(cpu, instrT, privileged);
}

static uint32_t translateThumb(uint16_t instrT) {
    bool vB;
    uint32_t instr = 0xE0000000UL /*most likely thing*/;
    uint16_t v16;
    uint_fast8_t v8;

    switch (instrT >> 12) {
        case 0:  // LSL(1) LSR(1) ASR(1) ADD(1) SUB(1) ADD(3) SUB(3)
        case 1:
            if ((instrT & 0x1800) != 0x1800) {  // LSL(1) LSR(1) ASR(1)

                instr |= 0x01B00000UL | ((instrT & 0x7) << 12) | ((instrT >> 3) & 7) |
                         ((instrT >> 6) & 0x60) | ((instrT << 1) & 0xF80);
            } else {
                vB = !!(instrT & 0x0200);  // SUB or ADD ?
                instr |= ((vB ? 5UL : 9UL) << 20) | (((uint32_t)(instrT & 0x38)) << 13) |
                         ((instrT & 0x07) << 12) | ((instrT >> 6) & 0x07);

                if (instrT & 0x0400) {  // ADD(1) SUB(1)

                    instr |= 0x02000000UL;
                } else {  // ADD(3) SUB(3)

                    // nothing to do here
                }
            }
            break;

        case 2:  // MOV(1) CMP(1) ADD(2) SUB(2)
        case 3:
            instr |= instrT & 0x00FF;
            switch ((instrT >> 11) & 3) {
                case 0:  // MOV(1)
                    instr |= 0x03B00000UL | ((instrT & 0x0700) << 4);
                    break;

                case 1:  // CMP(1)
                    instr |= 0x03500000UL | (((uint32_t)(instrT & 0x0700)) << 8);
                    break;

                case 2:  // ADD(2)
                    instr |= 0x02900000UL | ((instrT & 0x0700) << 4) |
                             (((uint32_t)(instrT & 0x0700)) << 8);
                    break;

                case 3:  // SUB(2)
                    instr |= 0x02500000UL | ((instrT & 0x0700) << 4) |
                             (((uint32_t)(instrT & 0x0700)) << 8);
                    break;
            }
            break;

        case 4:  // LDR(3) ADD(4) CMP(3) MOV(3) BX MVN CMP(2) CMN TST ADC SBC NEG MUL LSL(2) LSR(2)
                 // ASR(2) ROR AND EOR ORR BIC

            if (instrT & 0x0800) {  // LDR(3)
                return 0;
            } else if (instrT & 0x0400) {  // ADD(4) CMP(3) MOV(3) BX

                uint8_t vD;

                vD = (instrT & 7) | ((instrT >> 4) & 0x08);
                v8 = (instrT >> 3) & 0xF;

                switch ((instrT >> 8) & 3) {
                    case 0:  // ADD(4)
                        return 0;

                    case 1:  // CMP(3)

                        instr |= 0x01500000UL | (((uint32_t)vD) << 16) | v8;
                        break;

                    case 2:  // MOV(3)
                        return 0;

                    case 3:  // BX
                        return 0;

                    default:
                        goto undefined;
                }
            } else {  // AND EOR LSL(2) LSR(2) ASR(2) ADC SBC ROR TST NEG CMP(2) CMN ORR MUL BIC MVN
                      // (in val_tabl order)
                static const uint32_t val_tabl[16] = {
                    0x00100000UL, 0x00300000UL, 0x01B00010UL, 0x01B00030UL,
                    0x01B00050UL, 0x00B00000UL, 0x00D00000UL, 0x01B00070UL,
                    0x01100000UL, 0x02700000UL, 0x01500000UL, 0x01700000UL,
                    0x01900000UL, 0x00100090UL, 0x01D00000UL, 0x01F00000UL};

                // 00 = none
                // 10 = bit0 val
                // 11 = bit3 val
                // MVN BIC MUL ORR CMN CMP(2) NEG TST ROR SBC ADC ASR(2) LSR(2) LSL(2) EOR AND

                const uint32_t use16 = 0x2AAE280AUL;  // 0010 1010 1010 1110 0010 1000 0000 1010
                const uint32_t use12 = 0xA208AAAAUL;  // 1010 0010 0000 1000 1010 1010 1010 1010
                const uint32_t use8 = 0x0800C3F0UL;   // 0000 1000 0000 0000 1100 0011 1111 0000
                const uint32_t use0 = 0xFFF3BEAFUL;   // 1111 1111 1111 0011 1011 1110 1010 1111
                uint8_t vals[4] = {0};

                vals[2] = (instrT & 7);
                vals[3] = (instrT >> 3) & 7;
                v8 = (instrT >> 6) & 15;
                instr |= val_tabl[v8];
                v8 <<= 1;
                instr |= ((uint32_t)(vals[(use16 >> v8) & 3UL])) << 16;
                instr |= ((uint32_t)(vals[(use12 >> v8) & 3UL])) << 12;
                instr |= ((uint32_t)(vals[(use8 >> v8) & 3UL])) << 8;
                instr |= ((uint32_t)(vals[(use0 >> v8) & 3UL])) << 0;
            }
            break;

        case 5:  // STR(2)  STRH(2) STRB(2) LDRSB LDR(2) LDRH(2) LDRB(2) LDRSH		(in val_tbl
                 // orver)
        {
            static const uint32_t val_tabl[8] = {0x07800000UL, 0x018000B0UL, 0x07C00000UL,
                                                 0x019000D0UL, 0x07900000UL, 0x019000B0UL,
                                                 0x07D00000UL, 0x019000F0UL};
            instr |= ((instrT >> 6) & 7) | ((instrT & 7) << 12) |
                     (((uint32_t)(instrT & 0x38)) << 13) | val_tabl[(instrT >> 9) & 7];
        } break;

        case 6:  // LDR(1) STR(1)	(bit11 set = ldr)

            instr |= ((instrT & 7) << 12) | (((uint32_t)(instrT & 0x38)) << 13) |
                     ((instrT >> 4) & 0x7C) | 0x05800000UL;
            if (instrT & 0x0800) instr |= 0x00100000UL;
            break;

        case 7:  // LDRB(1) STRB(1)	(bit11 set = ldrb)

            instr |= ((instrT & 7) << 12) | (((uint32_t)(instrT & 0x38)) << 13) |
                     ((instrT >> 6) & 0x1F) | 0x05C00000UL;
            if (instrT & 0x0800) instr |= 0x00100000UL;
            break;

        case 8:  // LDRH(1) STRH(1)	(bit11 set = ldrh)

            instr |= ((instrT & 7) << 12) | (((uint32_t)(instrT & 0x38)) << 13) |
                     ((instrT >> 5) & 0x0E) | ((instrT >> 1) & 0x300) | 0x01C000B0UL;
            if (instrT & 0x0800) instr |= 0x00100000UL;
            break;

        case 9:  // LDR(4) STR(3)	(bit11 set = ldr)

            instr |= ((instrT & 0x700) << 4) | ((instrT & 0xFF) << 2) | 0x058D0000UL;
            if (instrT & 0x0800) instr |= 0x00100000UL;
            break;

        case 10:  // ADD(5) ADD(6)	(bit11 set = add(6))
            return 0;

        case 11:  // ADD(7) SUB(4) PUSH POP BKPT

            if ((instrT & 0x0600) == 0x0400) {  // PUSH POP

                instr |= (instrT & 0xFF) | 0x000D0000UL;

                if (instrT & 0x0800) {  // POP

                    if (instrT & 0x0100) instr |= 0x00008000UL;
                    instr |= 0x08B00000UL;
                } else {  // PUSH

                    if (instrT & 0x0100) instr |= 0x00004000UL;
                    instr |= 0x09200000UL;
                }
            } else if (instrT & 0x0100) {
                goto undefined;
            } else
                switch ((instrT >> 9) & 7) {
                    case 0:  // ADD(7) SUB(4)

                        instr |= 0x020DDF00UL | (instrT & 0x7F) |
                                 ((instrT & 0x0080) ? 0x00400000UL : 0x00800000UL);
                        break;

                    case 7:  // BKPT

                        instr |= 0x01200070UL | (instrT & 0x0F) | ((instrT & 0xF0) << 4);
                        break;

                    default:

                        goto undefined;
                }
            break;

        case 12:  // LDMIA STMIA		(bit11 set = ldmia)
            instr |= 0x08800000UL | (((uint32_t)(instrT & 0x700)) << 8) | (instrT & 0xFF);
            if (instrT & 0x0800) instr |= 0x00100000UL;
            if (!((1UL << ((instrT >> 8) & 0x07)) & instrT))
                instr |= 0x00200000UL;  // set W bit if needed
            break;

        case 13:  // B(1), SWI, undefined instr space
            v8 = ((instrT >> 8) & 0x0F);
            if (v8 == 14) {  // undefined instr
                goto undefined;
            } else if (v8 == 15) {  // SWI
                instr |= 0x0F000000UL | (instrT & 0xFF);
            } else {  // B(1)
                instr = (((uint32_t)v8) << 28) | 0x0A000000UL | (instrT & 0xFF);
                if (instrT & 0x80) instr |= 0x00FFFF00UL;
            }
            break;

        case 14:  // B(2) BL BLX(1) undefined instr space
        case 15:
            v16 = (instrT & 0x7FF);
            switch ((instrT >> 11) & 3) {
                case 0:  // B(2)

                    instr |= 0x0A000000UL | v16;
                    if (instrT & 0x0400) instr |= 0x00FFF800UL;
                    break;

                case 1:  // BLX(1)_suffix
                case 2:  // BLX(1)_prefix BL_prefix
                case 3:  // BL_suffix
                    return 0;
            }

            if (instrT & 0x0800)
                goto undefined;  // avoid BLX_suffix and undefined instr space in there
            instr |= 0x0A000000UL | (instrT & 0x7FF);
            if (instrT & 0x0400) instr |= 0x00FFF800UL;
            break;
    }

    return instr;

undefined:

    return 0xE7F000F0UL | (instrT & 0x0F) |
           ((instrT & 0xFFF0)
            << 4);  // guranteed undefined instr, inside it we store the original thumb instr :)=-)
}

struct ArmCpu *cpuInit(uint32_t pc, struct ArmMem *mem, bool xscale, bool omap, int debugPort,
                       uint32_t cpuid, uint32_t cacheId, struct PatchDispatch *patchDispatch,
                       struct PacePatch *pacePatch) {
    struct ArmCpu *cpu = (struct ArmCpu *)malloc(sizeof(*cpu));

    if (!cpu) ERR("cannot alloc CPU");

    memset(cpu, 0, sizeof(*cpu));

    cpu->debugStub = gdbStubInit(cpu, debugPort);
    if (!cpu->debugStub) ERR("Cannot init debug stub");

    cpu->I = true;  // start w/o interrupts in supervisor mode
    cpu->F = true;
    cpu->M = ARM_SR_MODE_SVC;

    cpu->mem = mem;
    cpuPrvSetPC(cpu, pc);

    cpu->mmu = mmuInit(mem, xscale);
    if (!cpu->mmu) ERR("Cannot init MMU");

    paceInit(cpu->mem, cpu->mmu);

    cpu->ic = icacheInit(mem, cpu->mmu);
    if (!cpu->ic) ERR("Cannot init icache");

    cpu->cp15 = cp15Init(cpu, cpu->mmu, cpu->ic, cpuid, cacheId, xscale, omap);
    if (!cpu->mmu) ERR("Cannot init CP15");

    cpu->patchDispatch = patchDispatch;
    cpu->pacePatch = pacePatch;

    if (!table_thumb2arm) {
        table_thumb2arm = malloc(0x10000 * sizeof(uint32_t));

        for (uint32_t instr = 0; instr < 0x10000; instr++)
            table_thumb2arm[instr] = translateThumb(instr);
    }

    return cpu;
}

struct ArmCpu *cpuPrepareInjectedCall(struct ArmCpu *cpu, struct ArmCpu *scratchState) {
    if (!scratchState) scratchState = (struct ArmCpu *)malloc(sizeof(*scratchState));
    memcpy(scratchState, cpu, sizeof(*scratchState));

    return scratchState;
}

void cpuFinishInjectedCall(struct ArmCpu *cpu, struct ArmCpu *scratchState) {
    cpu->waitingIrqs = scratchState->waitingIrqs;
    cpu->waitingFiqs = scratchState->waitingFiqs;
}

uint32_t *cpuGetRegisters(struct ArmCpu *cpu) { return cpu->regs; }

void cpuExecuteInjectedCall(struct ArmCpu *cpu, uint32_t syscall) {
    const uint8_t table = syscall >> 12;
    uint32_t tableAddr;
    if (!cpuPrvMemOpEx(cpu, &tableAddr, cpu->regs[9] - table, 4, false, true, NULL))
        ERR("failed to dispatch syscall %#010x: unable to read table address\n", syscall);

    const uint32_t offset = syscall & 0xfff;
    uint32_t entryAddr;
    if (!cpuPrvMemOpEx(cpu, &entryAddr, tableAddr + offset, 4, false, true, NULL))
        ERR("failed to dispatch syscall %#010x: unable to read entry point\n", syscall);

    cpu->regs[REG_NO_PC] = entryAddr;
    cpu->isInjectedCall = true;
    cpu->T = false;
    cpu->regs[REG_NO_LR] = INJECTED_CALL_LR_MAGIC;

    uint64_t cycle = 0;
    while (cpu->regs[REG_NO_PC] != INJECTED_CALL_LR_MAGIC) {
        cpuCycle(cpu);

        if (cycle++ == INJECTED_CALL_MAX_CYCLES)
            ERR("failed to execute syscall: cycle limit reached\n");
    }
}

static void cpuPrvPaceSyscall(struct ArmCpu *cpu) {
    ERR("PACE syscall to %#06x\n", paceReadTrapWord());
}

static void cpuPrvCyclePace(struct ArmCpu *cpu) {
    switch (paceExecute()) {
        case pace_status_ok:
            return;

        case pace_status_division_by_zero:
            ERR("PACE callout: division by zero\n");
            break;

        case pace_status_illegal_instr:
            ERR("PACE callout: illegal instruction\n");
            break;

        case pace_status_line_1010:
            ERR("PACE callout: line 1010\n");
            break;

        case pace_status_line_1111:
            ERR("PACE callout: line 1111\n");
            break;

        case pace_status_memory_fault:
            ERR("PACE: memory fault\n");
            break;

        case pace_status_syscall:
            cpuPrvPaceSyscall(cpu);
            break;

        case pace_status_trap0:
            ERR("PACE callout: trap 0\n");
            break;

        case pace_status_trap8:
            ERR("PACE callout: trap 8\n");
            break;

        case pace_status_return:
            ERR("PACE return\n");
            break;

        default:
            ERR("PACE callout: invalid instruction\n");
            break;
    }
}

void cpuCycle(struct ArmCpu *cpu) {
    if (unlikely(cpu->waitingFiqs && !cpu->F && !cpu->isInjectedCall))
        cpuPrvException(cpu, cpu->vectorBase + ARM_VECTOR_OFFT_FIQ, cpu->regs[REG_NO_PC] + 4,
                        ARM_SR_MODE_FIQ | ARM_SR_I | ARM_SR_F);
    else if (unlikely(cpu->waitingIrqs && !cpu->I && !cpu->isInjectedCall))
        cpuPrvException(cpu, cpu->vectorBase + ARM_VECTOR_OFFT_IRQ, cpu->regs[REG_NO_PC] + 4,
                        ARM_SR_MODE_IRQ | ARM_SR_I);

    cp15Cycle(cpu->cp15);
    patchOnBeforeExecute(cpu->patchDispatch, cpu->regs);

    if (cpu->modePace)
        cpuPrvCyclePace(cpu);
    else if (cpu->T)
        cpuPrvCycleThumb(cpu);
    else
        cpuPrvCycleArm(cpu);
}

void cpuIrq(struct ArmCpu *cpu, bool fiq, bool raise) {  // unraise when acknowledged

    if (fiq) {
        if (raise) {
            cpu->waitingFiqs++;
        } else if (cpu->waitingFiqs) {
            cpu->waitingFiqs--;
        } else {
            fprintf(stderr, "Cannot unraise FIQ when none raised");
        }
    } else {
        if (raise) {
            cpu->waitingIrqs++;
        } else if (cpu->waitingIrqs) {
            cpu->waitingIrqs--;
        } else {
            fprintf(stderr, "Cannot unraise IRQ when none raised");
        }
    }
}

void cpuCoprocessorRegister(struct ArmCpu *cpu, uint8_t cpNum, struct ArmCoprocessor *coproc) {
    cpu->coproc[cpNum] = *coproc;
}

void cpuSetVectorAddr(struct ArmCpu *cpu, uint32_t adr) { cpu->vectorBase = adr; }

uint16_t cpuGetCPAR(struct ArmCpu *cpu) { return cpu->CPAR; }

void cpuSetCPAR(struct ArmCpu *cpu, uint16_t cpar) { cpu->CPAR = cpar; }

void cpuSetPid(struct ArmCpu *cpu, uint32_t pid) { cpu->pid = pid; }

uint32_t cpuGetPid(struct ArmCpu *cpu) { return cpu->pid; }
