//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"
#include "soc_SSP.h"
#include "util.h"

#define PXA_SSP_SIZE 0x00010000UL

#define DMA_OFST_RX 0
#define DMA_OFST_TX 1

struct SocSsp {
    struct SocDma *dma;
    struct SocIc *ic;
    struct Reschedule reschedule;
    uint32_t base;
    uint8_t irqNo;
    uint8_t dmaReqNoBase;

    uint32_t cr0, cr1, sr;

    SspClientProcF procF[8];
    void *procD[8];

    uint16_t rxFifo[16], txFifo[16];
    uint8_t rxFifoUsed, txFifoUsed;
};

static void socSspPrvIrqsUpdate(struct SocSsp *ssp) {
    bool irq = false;

    if ((ssp->sr & 0x40) && (ssp->cr1 & 0x01)) irq = true;
    if ((ssp->sr & 0x20) && (ssp->cr1 & 0x02)) irq = true;

    socIcInt(ssp->ic, ssp->irqNo, irq);
}

static void socSspPrvRecalcRxFifoSta(struct SocSsp *ssp) {
    ssp->sr &= ~0xf048;

    if (ssp->rxFifoUsed) ssp->sr |= 0x08;
    ssp->sr |= (((ssp->rxFifoUsed - 1) & 0x0f) << 12);
    if (ssp->rxFifoUsed > ((ssp->cr1 >> 10) & 0x0f)) ssp->sr |= 0x40;

    socDmaExternalReq(ssp->dma, ssp->dmaReqNoBase + DMA_OFST_RX, !!(ssp->sr & 0x40));

    socSspPrvIrqsUpdate(ssp);
}

static void socSspPrvRecalcTxFifoSta(struct SocSsp *ssp) {
    ssp->sr &= ~0x0f24;

    if (ssp->txFifoUsed != sizeof(ssp->txFifo) / sizeof(*ssp->txFifo)) ssp->sr |= 0x04;
    ssp->sr |= ((ssp->txFifoUsed & 0x0f) << 8);
    if (ssp->txFifoUsed <= ((ssp->cr1 >> 6) & 0x0f)) ssp->sr |= 0x20;

    socDmaExternalReq(ssp->dma, ssp->dmaReqNoBase + DMA_OFST_RX, !!(ssp->sr & 0x20));

    socSspPrvIrqsUpdate(ssp);
}

static bool socSspPrvFifoR(struct SocSsp *ssp, uint16_t *valP) {
    if (!ssp->rxFifoUsed) {
        fprintf(stderr, "SSP RX FIFO UNDERFLOW\n");
        *valP = 0;
        return true;
    }

    *valP = ssp->rxFifo[0];
    memmove(ssp->rxFifo + 0, ssp->rxFifo + 1, sizeof(uint16_t) * --ssp->rxFifoUsed);

    socSspPrvRecalcRxFifoSta(ssp);

    return true;
}

static bool socSspPrvFifoW(struct SocSsp *ssp, uint16_t val) {
    if (ssp->txFifoUsed == sizeof(ssp->txFifo) / sizeof(*ssp->txFifo)) {
        fprintf(stderr, "SSP TX FIFO OVERFLOW\n");
        return true;
    }

    ssp->sr |= 0x10;  // busy
    ssp->txFifo[ssp->txFifoUsed++] = val;
    socSspPrvRecalcTxFifoSta(ssp);

    ssp->reschedule.rescheduleCb(ssp->reschedule.ctx, RESCHEDULE_TASK_SSP);

    return true;
}

static bool socSspPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                void *buf) {
    struct SocSsp *ssp = (struct SocSsp *)userData;
    uint32_t val;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - ssp->base) >> 2;

    if (write) val = *(uint32_t *)buf;

    switch (pa) {
        case 0:
            if (write)
                ssp->cr0 = val;
            else
                val = ssp->cr0;
            break;

        case 1:
            if (write)
                ssp->cr1 = val;
            else
                val = ssp->cr1;
            break;

        case 2:
            if (write)
                ssp->sr = (ssp->sr & ~0x80) | (val & 0x80);
            else
                val = ssp->sr;
            break;

        case 4:
            if (write)
                return socSspPrvFifoW(ssp, val);
            else
                return socSspPrvFifoR(ssp, (uint16_t *)buf);

        default:
            return false;
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

void socSspPeriodic(struct SocSsp *ssp) {
    if (!ssp->txFifoUsed)
        ssp->sr &= ~0x10;
    else {
        uint32_t val = ssp->txFifo[0], ret = 0, i;

        memmove(ssp->txFifo + 0, ssp->txFifo + 1, sizeof(uint16_t) * --ssp->txFifoUsed);
        socSspPrvRecalcTxFifoSta(ssp);

        if (ssp->cr1 & 4)  // loopback
            ret = val;
        else {
            for (i = 0; i < sizeof(ssp->procF) / sizeof(*ssp->procF); i++) {
                if (!ssp->procF[i]) continue;

                ret |= ssp->procF[i](ssp->procD[i], 1 + (ssp->cr0 & 15), val);
            }
        }

        if (ssp->rxFifoUsed == sizeof(ssp->rxFifo) / sizeof(*ssp->rxFifo)) {
            fprintf(stderr, "SSP RX FIFO OVERFLOW\n");
            ssp->sr |= 0x80;
        } else {
            ssp->rxFifo[ssp->rxFifoUsed++] = ret;
            socSspPrvRecalcRxFifoSta(ssp);
        }
    }
}

struct SocSsp *socSspInit(struct ArmMem *physMem, struct Reschedule reschedule, struct SocIc *ic,
                          struct SocDma *dma, uint32_t base, uint_fast8_t irqNo,
                          uint_fast8_t dmaReqNoBase) {
    struct SocSsp *ssp = (struct SocSsp *)malloc(sizeof(*ssp));

    if (!ssp) ERR("cannot alloc SSP");

    memset(ssp, 0, sizeof(*ssp));

    ssp->ic = ic;
    ssp->reschedule = reschedule;
    ssp->dma = dma;
    ssp->base = base;
    ssp->irqNo = irqNo;
    ssp->dmaReqNoBase = dmaReqNoBase;
    socSspPrvRecalcTxFifoSta(ssp);

    if (!memRegionAdd(physMem, base, PXA_SSP_SIZE, socSspPrvMemAccessF, ssp))
        ERR("cannot add SSP to MEM\n");

    return ssp;
}

bool socSspAddClient(struct SocSsp *ssp, SspClientProcF procF, void *userData) {
    uint32_t i;

    for (i = 0; i < sizeof(ssp->procF) / sizeof(*ssp->procF); i++) {
        if (ssp->procF[i]) continue;

        ssp->procF[i] = procF;
        ssp->procD[i] = userData;
        return true;
    }

    return false;
}

bool socSspTaskRequired(struct SocSsp *ssp) { return ssp->sr & 0x10; }