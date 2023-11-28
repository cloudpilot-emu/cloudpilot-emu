//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_MemCtrl.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

#define PXA_MEM_CONTROLLER_BASE 0x48000000UL
#define PXA_MEM_CONTROLLER_SIZE 0x00004000UL

struct PxaMemCtrlr {
    uint32_t mdcnfg, mdrefr, msc[3], mecr, sxcnfg, sxmrs, mcmem[2], mcatt[2], mcio[2], mdmrs;

    uint32_t arbCntrl, bscntr[4], mdmrslp, reg_0x20;
    uint16_t sa1110;
    uint8_t lcdbscntr;
    bool g2;
};

static bool pxaMemCtrlrPrvClockMgrMemAccessF(void* userData, uint32_t pa, uint_fast8_t size,
                                             bool write, void* buf) {
    struct PxaMemCtrlr* mc = (struct PxaMemCtrlr*)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;  // we do not support non-word accesses
    }

    pa = (pa - PXA_MEM_CONTROLLER_BASE) >> 2;

    if (write) {
        val = *(uint32_t*)buf;
        fprintf(stderr, " MEM: 0x%08x -> [0x%08lx]\n", val, pa * 4 + PXA_MEM_CONTROLLER_BASE);
    }

    switch (pa) {
        case 0:  // MDCNFG
            if (write)
                mc->mdcnfg = val;
            else
                val = mc->mdcnfg;
            break;

        case 1:  // MDREFR
            if (write)
                mc->mdrefr = val;
            else
                val = mc->mdrefr;
            break;

        case 2:  // MSCx
        case 3:
        case 4:
            if (write)
                mc->msc[pa - 2] = val;
            else
                val = mc->msc[pa - 2];
            break;

        case 5:  // MECR
            if (write)
                mc->mecr = val;
            else
                val = mc->mecr;
            break;

        case 7:  // SXCNFG
            if (write)
                mc->sxcnfg = val;
            else
                val = mc->sxcnfg;
            break;

        case 8:  // unknonw reg
            if (!mc->g2) return false;
            if (write)
                mc->reg_0x20 = val;
            else
                val = mc->reg_0x20;
            break;

        case 9:  // SXMRS
            if (mc->g2) return false;
            if (write)
                mc->sxmrs = val;
            else
                val = mc->sxmrs;
            break;

        case 10:  // MCMEMx
        case 11:
            if (write)
                mc->mcmem[pa - 10] = val;
            else
                val = mc->mcmem[pa - 10];
            break;

        case 12:  // MCATTx
        case 13:
            if (write)
                mc->mcatt[pa - 12] = val;
            else
                val = mc->mcatt[pa - 12];
            break;

        case 14:  // MCIOx
        case 15:
            if (write)
                mc->mcio[pa - 14] = val;
            else
                val = mc->mcio[pa - 14];
            break;

        case 16:  // MDMRS
            if (write)
                mc->mdmrs = val;
            else
                val = mc->mdmrs;
            break;

        case 17:  // BOOT_DEF
            if (write) return false;
            val = 9;  // boot from 16 bit memory
            break;

        case 18:  // ARB_CNTRL
            if (!mc->g2) return false;
            if (write)
                mc->arbCntrl = val & 0xff800ffful;
            else
                val = mc->arbCntrl;
            break;

        case 19:  // BSCNTR0
        case 20:  // BSCNTR1
            if (!mc->g2) return false;
            if (write)
                mc->bscntr[pa - 19] = val;
            else
                val = mc->bscntr[pa - 19];
            break;

        case 21:  // LCDBSCNTR
            if (!mc->g2) return false;
            if (write)
                mc->lcdbscntr = val & 0x0f;
            else
                val = mc->lcdbscntr;
            break;

        case 22:  // MDMRSLP
            if (write)
                mc->mdmrslp = val;
            else
                val = mc->mdmrslp;
            break;

        case 23:  // BSCNTR2
        case 24:  // BSCNTR3
            if (!mc->g2) return false;
            if (write)
                mc->bscntr[pa - 23 + 2] = val;
            else
                val = mc->bscntr[pa - 23 + 2];
            break;

        case 25:  // SA1110
            if (write)
                mc->sa1110 = val & 0x313f;
            else
                val = mc->sa1110;
            break;

        default:
            return false;
    }

    if (!write) *(uint32_t*)buf = val;

    return true;
}

struct PxaMemCtrlr* pxaMemCtrlrInit(struct ArmMem* physMem, uint_fast8_t socRev) {
    struct PxaMemCtrlr* mc = (struct PxaMemCtrlr*)malloc(sizeof(*mc));
    uint_fast8_t i;

    if (!mc) ERR("cannot alloc MEMC");

    memset(mc, 0, sizeof(*mc));
    mc->g2 = socRev == 2;

    mc->mdcnfg = 0x0b000b00ul;
    mc->mdmrs = 0x00220022ul;
    mc->mdrefr = 0x23ca4ffful;
    mc->sxcnfg = 0x40044004ul;

    for (i = 0; i < 3; i++) mc->msc[i] = 0x7ff07ff0ul;

    mc->arbCntrl = 0x00800234ul;
    for (i = 0; i < 4; i++) mc->bscntr[i] = 0x55555555ul;
    mc->lcdbscntr = 0x05;

    if (!memRegionAdd(physMem, PXA_MEM_CONTROLLER_BASE, PXA_MEM_CONTROLLER_SIZE,
                      pxaMemCtrlrPrvClockMgrMemAccessF, mc))
        ERR("cannot add MEMC to MEM\n");

    return mc;
}
