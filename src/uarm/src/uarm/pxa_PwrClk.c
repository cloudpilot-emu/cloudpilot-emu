//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_PwrClk.h"

#include <stdlib.h>
#include <string.h>

#include "SoC.h"
#include "util.h"

#define PXA_CLOCK_MANAGER_BASE 0x41300000UL
#define PXA_CLOCK_MANAGER_SIZE 0x00001000UL

#define PXA_POWER_MANAGER_BASE 0x40F00000UL
#define PXA_POWER_MANAGER_SIZE 0x00000180UL

struct PxaPwrClk {
    struct ArmCpu *cpu;
    struct SoC *soc;
    uint32_t CCCR, CKEN, OSCR;  // clocks manager regs
    // power mgr common between 255 and 270
    uint32_t PMCR, PSSR, PSPR, PWER, PRER, PFER, PEDR, PCFR, PGSR[4], RCSR, PMFW;
    // power mgr 270 only
    uint32_t PSTR, PVCR, PUCR, PKWR, PKSR, PCMD[32];
    bool turbo, isPXA270;
};

static bool pxaPwrClkPrvCoproc7regXferFunc(struct ArmCpu *cpu, void *userData, bool two, bool read,
                                           uint8_t op1, uint8_t Rx, uint8_t CRn, uint8_t CRm,
                                           uint8_t op2) {
    // PSFS read?
    // real hardware ignors CRm if CRn is 4, so do we
    if (read && !two && !op1 && !op2 && CRn == 4) {
        cpuSetReg(cpu, Rx, 0);

        return true;
    }

    return false;
}

static bool pxaPwrClkPrvCoproc14regXferFunc(struct ArmCpu *cpu, void *userData, bool two, bool read,
                                            uint8_t op1, uint8_t Rx, uint8_t CRn, uint8_t CRm,
                                            uint8_t op2) {
    struct PxaPwrClk *pc = (struct PxaPwrClk *)userData;
    uint32_t val = 0;

    if (!read) val = cpuGetRegExternal(cpu, Rx);

    if (CRm == 0 && op1 == 0 && !two) {
        switch (CRn) {
            case 6:
                if (op2 != 0) break;
                if (read)
                    val = 0;
                else {
                    pc->turbo = (val & 1) != 0;
                    if (val & 2)
                        fprintf(stderr, "Set speed mode (CCCR + cp14 reg6) to 0x%08lx 0x%08lx\n",
                                (unsigned long)pc->CCCR, (unsigned long)val);
                }
                goto success;

            case 7:
                if (read)
                    val = 0;
                else if (val == 1 && op2 == 0) {
                    socSleep(pc->soc);
                } else if (val == 3 && op2 == 0) {
                    // pretend we woke up
                    pc->RCSR |= 4;
                    cpuReset(pc->cpu, 0);
                } else if (op2 == 0) {
                    fprintf(stderr,
                            "Someone tried to set processor power mode (cp14 reg7) to 0x%08lx\n",
                            (unsigned long)val);
                }
                goto success;

            case 10:  // notifies debugger about shit. if no debugger we just continue
                if (op2 != 0) break;
                if (read) val = 0;
                goto success;
        }
    }

    return false;

success:

    if (read) cpuSetReg(cpu, Rx, val);

    return true;
}

static bool pxaPwrClkPrvClockMgrMemAccessF(void *userData, uint32_t pa, uint8_t size, bool write,
                                           void *buf) {
    struct PxaPwrClk *pc = (struct PxaPwrClk *)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_CLOCK_MANAGER_BASE) >> 2;

    if (write) val = *(uint32_t *)buf;

    switch (pa) {
        case 0:  // CCCR
            if (write)
                pc->CCCR = val;
            else
                val = pc->CCCR;
            break;

        case 1:  // CKEN
            if (write)
                pc->CKEN = val;
            else
                val = pc->CKEN;
            break;

        case 2:  // OSCR
            if (!write) val = pc->OSCR;
            // no writing to this register
            break;
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

static bool pxaPwrClkPrvPowerMgrMemAccessF(void *userData, uint32_t pa, uint_fast8_t size,
                                           bool write, void *buf) {
    struct PxaPwrClk *pc = (struct PxaPwrClk *)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_POWER_MANAGER_BASE) >> 2;

    if (write) val = *(uint32_t *)buf;

    switch (pa) {
        case 0x00 / 4:  // PMCR
            if (write)
                pc->PMCR = val & (pc->isPXA270 ? 0x3f : 0x01);
            else
                val = pc->PMCR;
            break;

        case 0x04 / 4:  // PSSR
            if (write)
                pc->PSSR &= ~val;

            else
                val = pc->PSSR;
            break;

        case 0x08 / 4:  // PSPR
            if (write)
                pc->PSPR = val;
            else
                val = pc->PSPR;
            break;

        case 0x0c / 4:  // PWER
            if (write)
                pc->PWER = val & (pc->isPXA270 ? 0xdf9ffe1b : 0xf000ffff);
            else
                val = pc->PWER;
            break;

        case 0x10 / 4:  // PRER
            if (write)
                pc->PRER = val & (pc->isPXA270 ? 0x0100fe1b : 0x0000ffff);
            else
                val = pc->PRER;
            break;

        case 0x14 / 4:  // PFER
            if (write)
                pc->PFER = val & (pc->isPXA270 ? 0x0100fe1b : 0x0000ffff);
            else
                val = pc->PFER;
            break;

        case 0x18 / 4:  // PEDR
            if (write)
                pc->PEDR &= ~val;
            else
                val = pc->PEDR;
            break;

        case 0x1c / 4:  // PCFR
            if (write)
                pc->PCFR = val & (pc->isPXA270 ? 0xdcd7 : 0x0007);
            else
                val = pc->PCFR;
            break;

        case 0x2c / 4:  // PGSR[3]
            if (!pc->isPXA270) return false;
            val &= 0x01ffffff;
            // fallthrough
        case 0x28 / 4:  // PGSR[2]
        case 0x24 / 4:  // PGSR[1]
        case 0x20 / 4:  // PGSR[0]
            if (write)
                pc->PGSR[pa - 0x20 / 4] = val;
            else
                val = pc->PGSR[pa - 0x20 / 4];
            break;

        case 0x30 / 4:  // RCSR
            if (write)
                pc->RCSR &= ~val;
            else
                val = pc->RCSR;
            break;

        case 0x34 / 4:  // PMFW (pxa255) / PSLR (pxa270)
            if (write)
                pc->PMFW = val & (pc->isPXA270 ? 0xffd00f0c : 0x00000002);
            else
                val = pc->PMFW;
            break;

        case 0x38 / 4:  // PSTR
            if (!pc->isPXA270) return false;
            if (write)
                pc->PSTR = val & 0x00000f0c;
            else
                val = pc->PSTR;
            break;

        case 0x40 / 4:  // PVCR (we do not emulate the actual PM i2c bus)
            if (!pc->isPXA270) return false;
            if (write)
                pc->PVCR = val & 0x01f04fff;
            else
                val = pc->PVCR;
            break;

        case 0x4c / 4:  // PUCR
            if (!pc->isPXA270) return false;
            if (write)
                pc->PUCR = val & 0x0000002d;
            else
                val = pc->PUCR;
            break;

        case 0x50 / 4:  // PKWR
            if (!pc->isPXA270) return false;
            if (write)
                pc->PKWR = val & 0x000fffff;
            else
                val = pc->PKWR;
            break;

        case 0x54 / 4:  // PKSR
            if (!pc->isPXA270) return false;
            if (write)
                pc->PKWR &= ~val;
            else
                val = pc->PKSR;
            break;

        case 0x80 / 4 ... 0xfc / 4:
            if (!pc->isPXA270) return false;
            if (write)
                pc->PCMD[pa - 0x80 / 4] = val & 0x00001fff;
            else
                val = pc->PCMD[pa - 0x80 / 4];
            break;

        default:
            return false;
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

struct PxaPwrClk *pxaPwrClkInit(struct ArmCpu *cpu, struct ArmMem *physMem, struct SoC *soc,
                                bool isPXA270) {
    struct PxaPwrClk *pc = (struct PxaPwrClk *)malloc(sizeof(*pc));
    struct ArmCoprocessor cp14 = {
        .regXfer = pxaPwrClkPrvCoproc14regXferFunc,
        .userData = pc,
    };
    struct ArmCoprocessor cp7 = {
        .regXfer = pxaPwrClkPrvCoproc7regXferFunc,
        .userData = pc,
    };

    if (!pc) ERR("cannot alloc PWRCLKMGR");

    memset(pc, 0, sizeof(*pc));

    pc->cpu = cpu;
    pc->isPXA270 = isPXA270;
    pc->soc = soc;
    pc->CCCR = 0x00000122UL;  // set CCCR to almost default value (we use mult 32 not 27)
    pc->CKEN = 0x000179EFUL;  // set CKEN to default value
    pc->OSCR = 0x00000003UL;  // 32KHz oscillator on and stable
    pc->PSSR = 0x20;
    pc->PWER = 0x03;
    pc->PRER = 0x03;
    pc->PFER = 0x03;
    pc->PMFW = isPXA270 ? 0xcc000000 : 0x00000000;

    // pretend we just power-no-resetted
    pc->RCSR |= 1;

    cpuCoprocessorRegister(cpu, 14, &cp14);
    cpuCoprocessorRegister(cpu, 7, &cp7);

    if (!memRegionAdd(physMem, PXA_CLOCK_MANAGER_BASE, PXA_CLOCK_MANAGER_SIZE,
                      pxaPwrClkPrvClockMgrMemAccessF, pc))
        ERR("cannot add CLKMGR to MEM\n");

    if (!memRegionAdd(physMem, PXA_POWER_MANAGER_BASE, PXA_POWER_MANAGER_SIZE,
                      pxaPwrClkPrvPowerMgrMemAccessF, pc))
        ERR("cannot add PWRMGR to MEM\n");

    return pc;
}
