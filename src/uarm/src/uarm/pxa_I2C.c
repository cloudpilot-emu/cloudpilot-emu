//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_IC.h"
#include "soc_I2C.h"
#include "util.h"

#define PXA_I2C_BASE 0x40301680UL
#define PXA_I2C_SIZE 0x00000024UL

#define REG_IDX_IBMR 0
#define REG_IDX_IDBR 1
#define REG_IDX_ICR 2
#define REG_IDX_ISR 3
#define REG_IDX_ISAR 4

struct SocI2c {
    struct SocDma *dma;
    struct SocIc *ic;
    uint16_t icr;
    uint16_t isr;
    uint8_t db;
    uint8_t isa;

    struct DevI2C {
        I2cDeviceActionF actF;
        void *userData;
    } devs[8];

    uint8_t waitForAddr : 1;
    uint8_t latentBusy : 1;
};

bool socI2cDeviceAdd(struct SocI2c *i2c, I2cDeviceActionF actF, void *userData) {
    uint_fast8_t i;

    for (i = 0; i < sizeof(i2c->devs) / sizeof(*i2c->devs); i++) {
        if (i2c->devs[i].actF) continue;

        i2c->devs[i].actF = actF;
        i2c->devs[i].userData = userData;

        return true;
    }

    return false;
}

static void socI2cPrvRecalcIrq(struct SocI2c *i2c) {
    uint32_t effectiveIsr = i2c->isr & 0x6f0;

    if (!(i2c->icr & 0x2000))  // SADIE
        effectiveIsr &= ~(1 << 9);
    if (!(i2c->icr & 0x1000))  // ALDIE
        effectiveIsr &= ~(1 << 5);
    if (!(i2c->icr & 0x0800))  // SSDIE
        effectiveIsr &= ~(1 << 4);
    if (!(i2c->icr & 0x0400))  // BEIE
        effectiveIsr &= ~(1 << 10);
    if (!(i2c->icr & 0x0200))  // IRFIE
        effectiveIsr &= ~(1 << 7);
    if (!(i2c->icr & 0x0100))  // ITEIE
        effectiveIsr &= ~(1 << 6);

    socIcInt(i2c->ic, PXA_I_I2C, !!effectiveIsr);
}

static uint_fast8_t socI2cPrvAction(struct SocI2c *i2c, enum ActionI2C action, uint8_t param) {
    uint_fast8_t ret = 0, i;

    for (i = 0; i < sizeof(i2c->devs) / sizeof(*i2c->devs); i++) {
        if (!i2c->devs[i].actF) continue;

        ret |= i2c->devs[i].actF(i2c->devs[i].userData, action, param);
    }

    // fprintf(stderr, "I2C: %s 0x%02x -> 0x%02x\n", acts[action], param, ret);

    return ret;
};

static void socI2cPrvCrW(struct SocI2c *i2c, uint32_t val) {
    uint32_t diffBits = i2c->icr ^ val;

    // irq masking & nonactionable bits update
    i2c->icr = val;

    if (!(val & 0x40)) return;

    if (val & diffBits & 0x01) {
        socI2cPrvAction(i2c, (i2c->isr & 4) ? i2cRestart : i2cStart, 0);
        i2c->waitForAddr = 1;
        i2c->isr |= 0x4;
    }
    if (val & 0x08) {
        if (i2c->waitForAddr) {
            if (i2c->isr & 0x40) fprintf(stderr, "i2c: sending from empty buffer\n");

            i2c->isr = (i2c->isr & ~1) | (i2c->db & 1);
            i2c->isr = (i2c->isr & ~2) | (socI2cPrvAction(i2c, i2cTx, i2c->db) ? 0 : 2);
            i2c->waitForAddr = 0;
            i2c->isr |= 0x40;
        } else if (!(i2c->isr & 1)) {  // TXing

            if (i2c->isr & 0x40) fprintf(stderr, "i2c: sending from empty buffer\n");

            i2c->isr = (i2c->isr & ~2) | (socI2cPrvAction(i2c, i2cTx, i2c->db) ? 0 : 2);
            i2c->isr |= 0x40;
        } else {  // RXing

            if (i2c->isr & 0x80) fprintf(stderr, "i2c: recving into full buffer\n");

            i2c->db = socI2cPrvAction(i2c, i2cRx, !(i2c->icr & 4));
            i2c->isr |= 0x80;
            // record ack/nak we sent
            i2c->isr &= ~2;
            if (i2c->icr & 4) i2c->isr |= 2;
        }
        if (val & 2) {
            socI2cPrvAction(i2c, i2cStop, 0);
            i2c->isr &= ~0x1;
            i2c->latentBusy = 1;
        }
        i2c->icr &= ~8;
    }
    if (val & 0x10) {
        socI2cPrvAction(i2c, i2cStop, 0);
        i2c->isr &= ~0x1;
        i2c->latentBusy = 1;
    }

    socI2cPrvRecalcIrq(i2c);
}

static bool socI2cPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                void *buf) {
    struct SocI2c *i2c = (struct SocI2c *)userData;
    uint32_t val;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_I2C_BASE) >> 2;
    if (pa & 1) return false;
    pa >>= 1;

    if (write) val = *(uint32_t *)buf;

    if (write && i2c->latentBusy) {
        i2c->isr &= ~4;
        i2c->latentBusy = 0;
    }

    switch (pa) {
        case REG_IDX_IBMR:
            if (write) return false;
            val = 0x03;  // lines are high;
            break;

        case REG_IDX_IDBR:
            if (write) {
                i2c->isr &= ~0x40;
                i2c->db = val;
            } else
                val = i2c->db;
            socI2cPrvRecalcIrq(i2c);
            break;

        case REG_IDX_ICR:
            if (write) {
                socI2cPrvCrW(i2c, val);
                socI2cPrvRecalcIrq(i2c);
            } else
                val = i2c->icr;
            break;

        case REG_IDX_ISR:
            if (write) {
                i2c->isr &= ~(val & 0x6f0);
                socI2cPrvRecalcIrq(i2c);
            } else
                val = i2c->isr;
            break;

        case REG_IDX_ISAR:
            if (write)
                i2c->isa = val & 0x7f;
            else
                val = i2c->isa;
            break;

        default:
            return false;
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

struct SocI2c *socI2cInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma) {
    struct SocI2c *i2c = (struct SocI2c *)malloc(sizeof(*i2c));

    if (!i2c) ERR("cannot alloc I2C");

    memset(i2c, 0, sizeof(*i2c));
    i2c->dma = dma;
    i2c->ic = ic;
    i2c->isr |= 0x40;  // tx empty

    if (!memRegionAdd(physMem, PXA_I2C_BASE, PXA_I2C_SIZE, socI2cPrvMemAccessF, i2c))
        ERR("cannot add I2C to MEM\n");

    return i2c;
}
