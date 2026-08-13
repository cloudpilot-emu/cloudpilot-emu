//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_SSP.h"

#include <cstdlib>
#include <cstring>

#include "cputil.h"
#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"
#include "savestate/savestateAll.h"

#define PXA_SSP_SIZE 0x00010000UL

#define DMA_OFST_RX 0
#define DMA_OFST_TX 1

#define SAVESTATE_VERSION 0

struct PxaSsp {
    struct PxaDma *dma;
    struct PxaIc *ic;
    struct Reschedule reschedule;
    uint32_t base;
    uint8_t irqNo;
    uint8_t dmaReqNoBase;

    uint32_t cr0, cr1, sr;

    SspClientProcF procF[8];
    void *procD[8];

    uint16_t rxFifo[16], txFifo[16];
    uint8_t rxFifoUsed, txFifoUsed;

    template <typename T>
    void DoSaveLoad(T &chunkHelper) {
        chunkHelper.Do32(cr0)
            .Do32(cr1)
            .Do32(sr)
            .DoBuffer16(rxFifo, sizeof(rxFifo) >> 1)
            .DoBuffer16(txFifo, sizeof(txFifo) >> 1)
            .Do(typename T::Pack8() << rxFifoUsed << txFifoUsed);
    }
};

static void pxaSspPrvIrqsUpdate(struct PxaSsp *ssp) {
    bool irq = false;

    if ((ssp->sr & 0x40) && (ssp->cr1 & 0x01)) irq = true;
    if ((ssp->sr & 0x20) && (ssp->cr1 & 0x02)) irq = true;

    pxaIcInt(ssp->ic, ssp->irqNo, irq);
}

static void pxaSspPrvRecalcRxFifoSta(struct PxaSsp *ssp) {
    ssp->sr &= ~0xf048;

    if (ssp->rxFifoUsed) ssp->sr |= 0x08;
    ssp->sr |= (((ssp->rxFifoUsed - 1) & 0x0f) << 12);
    if (ssp->rxFifoUsed > ((ssp->cr1 >> 10) & 0x0f)) ssp->sr |= 0x40;

    pxaDmaExternalReq(ssp->dma, ssp->dmaReqNoBase + DMA_OFST_RX, !!(ssp->sr & 0x40));

    pxaSspPrvIrqsUpdate(ssp);
}

static void pxaSspPrvRecalcTxFifoSta(struct PxaSsp *ssp) {
    ssp->sr &= ~0x0f24;

    if (ssp->txFifoUsed != sizeof(ssp->txFifo) / sizeof(*ssp->txFifo)) ssp->sr |= 0x04;
    ssp->sr |= ((ssp->txFifoUsed & 0x0f) << 8);
    if (ssp->txFifoUsed <= ((ssp->cr1 >> 6) & 0x0f)) ssp->sr |= 0x20;

    pxaDmaExternalReq(ssp->dma, ssp->dmaReqNoBase + DMA_OFST_RX, !!(ssp->sr & 0x20));

    pxaSspPrvIrqsUpdate(ssp);
}

static bool pxaSspPrvFifoR(struct PxaSsp *ssp, uint16_t *valP) {
    if (!ssp->rxFifoUsed) {
        fprintf(stderr, "SSP RX FIFO UNDERFLOW\n");
        *valP = 0;
        return true;
    }

    *valP = ssp->rxFifo[0];
    memmove(ssp->rxFifo + 0, ssp->rxFifo + 1, sizeof(uint16_t) * --ssp->rxFifoUsed);

    pxaSspPrvRecalcRxFifoSta(ssp);

    return true;
}

static bool pxaSspPrvFifoW(struct PxaSsp *ssp, uint16_t val) {
    if (ssp->txFifoUsed == sizeof(ssp->txFifo) / sizeof(*ssp->txFifo)) {
        fprintf(stderr, "SSP TX FIFO OVERFLOW\n");
        return true;
    }

    ssp->sr |= 0x10;  // busy
    ssp->txFifo[ssp->txFifoUsed++] = val;
    pxaSspPrvRecalcTxFifoSta(ssp);

    ssp->reschedule.rescheduleCb(ssp->reschedule.ctx, RESCHEDULE_TASK_SSP);

    return true;
}

static bool pxaSspPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                void *buf) {
    struct PxaSsp *ssp = (struct PxaSsp *)userData;
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
                return pxaSspPrvFifoW(ssp, val);
            else
                return pxaSspPrvFifoR(ssp, (uint16_t *)buf);

        default:
            return false;
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

void pxaSspPeriodic(struct PxaSsp *ssp) {
    if (!ssp->txFifoUsed)
        ssp->sr &= ~0x10;
    else {
        uint32_t val = ssp->txFifo[0], ret = 0, i;

        memmove(ssp->txFifo + 0, ssp->txFifo + 1, sizeof(uint16_t) * --ssp->txFifoUsed);
        pxaSspPrvRecalcTxFifoSta(ssp);

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
            pxaSspPrvRecalcRxFifoSta(ssp);
        }
    }
}

struct PxaSsp *pxaSspInit(struct ArmMem *physMem, struct Reschedule reschedule, struct PxaIc *ic,
                          struct PxaDma *dma, uint32_t base, uint_fast8_t irqNo,
                          uint_fast8_t dmaReqNoBase) {
    struct PxaSsp *ssp = (struct PxaSsp *)malloc(sizeof(*ssp));

    if (!ssp) ERR("cannot alloc SSP");

    memset(ssp, 0, sizeof(*ssp));

    ssp->ic = ic;
    ssp->reschedule = reschedule;
    ssp->dma = dma;
    ssp->base = base;
    ssp->irqNo = irqNo;
    ssp->dmaReqNoBase = dmaReqNoBase;
    pxaSspPrvRecalcTxFifoSta(ssp);

    if (!memRegionAdd(physMem, base, PXA_SSP_SIZE, pxaSspPrvMemAccessF, ssp))
        ERR("cannot add SSP to MEM\n");

    return ssp;
}

bool pxaSspAddClient(struct PxaSsp *ssp, SspClientProcF procF, void *userData) {
    uint32_t i;

    for (i = 0; i < sizeof(ssp->procF) / sizeof(*ssp->procF); i++) {
        if (ssp->procF[i]) continue;

        ssp->procF[i] = procF;
        ssp->procD[i] = userData;
        return true;
    }

    return false;
}

bool pxaSspTaskRequired(struct PxaSsp *ssp) { return ssp->sr & 0x10; }

template <typename T>
void pxaSspSave(struct PxaSsp *ssp, T &savestate, uint32_t index) {
    auto chunk = savestate.GetChunk(ChunkType::pxaSsp + index, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    ssp->DoSaveLoad(helper);
}

template <typename T>
void pxaSspLoad(struct PxaSsp *ssp, T &loader, uint32_t index) {
    auto chunk = loader.GetChunkOrFail(ChunkType::pxaSsp + index, SAVESTATE_VERSION, "pxa ssp");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    ssp->DoSaveLoad(helper);
}

template void pxaSspSave<Savestate<ChunkType>>(PxaSsp *ssp, Savestate<ChunkType> &savestate,
                                               uint32_t index);
template void pxaSspSave<SavestateProbe<ChunkType>>(PxaSsp *ssp,
                                                    SavestateProbe<ChunkType> &savestate,
                                                    uint32_t index);
template void pxaSspLoad<SavestateLoader<ChunkType>>(PxaSsp *ssp,
                                                     SavestateLoader<ChunkType> &loader,
                                                     uint32_t index);
