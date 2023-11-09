//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_PwrClk.h"

#include <stdlib.h>
#include <string.h>

#include "SoC.h"
#include "util.h"

#define PXA_CLOCK_MANAGER_BASE 0x41300000UL
#define PXA_CLOCK_MANAGER_SIZE 0x00001000UL

#define PXA_POWER_MANAGER_BASE 0x40F00000UL
#define PXA_POWER_MANAGER_SIZE 0x00001000UL

struct PxaPwrClk {
    struct ArmCpu *cpu;
    struct SoC *soc;
    uint32_t CCCR, CKEN, OSCR;  // clocks manager regs
    uint32_t pwrRegs[13];       // we care so little about these, we don't even name them
    bool turbo;
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
                    val = pc->turbo ? 1 : 0;
                else if (val == 1 && op2 == 0) {
                    socSleep(pc->soc);
                } else if (val != 0 && op2 == 0) {
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

    if (pa < 13) {
        if (write)
            pc->pwrRegs[pa] = val;
        else
            val = pc->pwrRegs[pa];
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

struct PxaPwrClk *pxaPwrClkInit(struct ArmCpu *cpu, struct ArmMem *physMem, struct SoC *soc) {
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
    pc->soc = soc;
    pc->CCCR = 0x00000122UL;  // set CCCR to almost default value (we use mult 32 not 27)
    pc->CKEN = 0x000179EFUL;  // set CKEN to default value
    pc->OSCR = 0x00000003UL;  // 32KHz oscillator on and stable
    pc->pwrRegs[1] = 0x20;    // set PSSR
    pc->pwrRegs[3] = 3;       // set PWER
    pc->pwrRegs[4] = 3;       // set PRER
    pc->pwrRegs[5] = 3;       // set PFER
    pc->pwrRegs[12] = 1;      // set RCSR as is for power on reset

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
