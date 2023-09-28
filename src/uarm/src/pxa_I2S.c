//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <stdlib.h>
#include <string.h>

#include "pxa_DMA.h"
#include "pxa_IC.h"
#include "soc_I2S.h"
#include "util.h"

#define PXA_I2S_BASE 0x40400000UL
#define PXA_I2S_SIZE 0x00010000UL

struct SocI2s {
    struct SocDma *dma;
    struct SocIc *ic;

    uint16_t sacr0;
    uint16_t sasr0;
    uint8_t sacr1;
    uint8_t sadiv;
    uint8_t saimr;

    uint32_t txFifo[16];
    uint32_t rxFifo[16];
    uint8_t txFifoEnts;
    uint8_t rxFifoEnts;
};

static void socI2sPrvIrqUpdate(struct SocI2s *i2s) {
    socIcInt(i2s->ic, PXA_I_I2S, !!(i2s->sasr0 & i2s->saimr & 0x78));
}

static void socI2sPrvTxFifoRecalc(struct SocI2s *i2s) {
    i2s->sasr0 &= ~0x0f09;

    i2s->sasr0 |= (i2s->txFifoEnts & 0x0f) << 8;
    if (i2s->txFifoEnts != sizeof(i2s->txFifo) / sizeof(*i2s->txFifo)) i2s->sasr0 |= 0x01;

    if (i2s->txFifoEnts < ((i2s->sacr0 >> 8) & 0x0f)) i2s->sasr0 |= 0x08;

    socI2sPrvIrqUpdate(i2s);
    socDmaExternalReq(i2s->dma, DMA_CMR_I2S_TX, !!(i2s->sasr0 & 0x08));
}

static void socI2sPrvRxFifoRecalc(struct SocI2s *i2s) {
    i2s->sasr0 &= ~0xf012;

    i2s->sasr0 |= (i2s->rxFifoEnts & 0x0f) << 12;
    if (!i2s->rxFifoEnts) i2s->sasr0 |= 0x02;

    if (i2s->rxFifoEnts >= ((i2s->sacr0 >> 12) & 0x0f) + 1) i2s->sasr0 |= 0x10;

    socI2sPrvIrqUpdate(i2s);

    socDmaExternalReq(i2s->dma, DMA_CMR_I2S_RX, !!(i2s->sasr0 & 0x10));
}

static bool socI2sPrvFifoW(struct SocI2s *i2s, uint32_t val) {
    if (i2s->txFifoEnts == sizeof(i2s->txFifo) / sizeof(*i2s->txFifo)) {
        fprintf(stderr, "TX fifo overrun\n");
        return true;
    }
    i2s->txFifo[i2s->txFifoEnts++] = val;
    socI2sPrvTxFifoRecalc(i2s);

    return true;
}

static bool socI2sPrvFifoR(struct SocI2s *i2s, uint32_t *valP) {
    if (!i2s->rxFifoEnts) {
        fprintf(stderr, "RX fifo underrun\n");
        return false;
    }
    *valP = i2s->rxFifo[0];
    memmove(i2s->rxFifo + 0, i2s->rxFifo + 1, sizeof(*i2s->rxFifo) * --i2s->rxFifoEnts);
    socI2sPrvRxFifoRecalc(i2s);

    return true;
}

static bool socI2sPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                void *buf) {
    struct SocI2s *i2s = (struct SocI2s *)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_I2S_BASE) >> 2;

    if (write) val = *(uint32_t *)buf;

    switch (pa) {
        case 0:
            if (write)
                i2s->sacr0 = val & 0xff3d;
            else
                val = i2s->sacr0;
            break;

        case 1:
            if (write)
                i2s->sacr1 = val & 0x39;
            else
                val = i2s->sacr1;
            break;

        case 3:
            if (write)
                return false;
            else
                val = i2s->sasr0;
            break;

        case 5:
            if (write)
                i2s->saimr = val & 0x78;
            else
                val = i2s->saimr;
            break;

        case 6:
            if (write)
                i2s->sasr0 &= ~(val & 0x60);
            else
                return false;
            break;

        case 24:
            if (write)
                i2s->sadiv = val & 0x7f;
            else
                val = i2s->sadiv;
            break;

        case 32:
            if (write)
                return socI2sPrvFifoW(i2s, val);
            else
                return socI2sPrvFifoR(i2s, (uint32_t *)buf);
            break;

        default:
            return false;
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

struct SocI2s *socI2sInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma) {
    struct SocI2s *i2s = (struct SocI2s *)malloc(sizeof(*i2s));

    if (!i2s) ERR("cannot alloc I2C");

    memset(i2s, 0, sizeof(*i2s));
    i2s->ic = ic;
    i2s->dma = dma;
    i2s->sacr0 = 0x7700;
    i2s->sasr0 = 0x0001;
    i2s->sadiv = 0x001a;

    if (!memRegionAdd(physMem, PXA_I2S_BASE, PXA_I2S_SIZE, socI2sPrvMemAccessF, i2s))
        ERR("cannot add I2S to MEM\n");

    return i2s;
}

void socI2sPeriodic(struct SocI2s *i2s) {
    uint32_t val = 0;

    // consume a sample if tx is allowed
    if (!(i2s->sacr1 & 0x10)) {
        if (i2s->txFifoEnts) {
            val = i2s->txFifo[0];
            memmove(i2s->txFifo + 0, i2s->txFifo + 1, sizeof(*i2s->txFifo) * --i2s->txFifoEnts);
        } else
            i2s->sasr0 |= 0x20;
    }

    // get a sample if RX is allowed
    if (!(i2s->sacr1 & 0x08)) {
        if (i2s->rxFifoEnts != sizeof(i2s->rxFifo) / sizeof(*i2s->rxFifo)) {
            i2s->rxFifo[i2s->rxFifoEnts++] = val;
        } else
            i2s->sasr0 |= 0x40;
    }

    socI2sPrvTxFifoRecalc(i2s);
    socI2sPrvRxFifoRecalc(i2s);
}
