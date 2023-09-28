//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"
#include "soc_AC97.h"
#include "util.h"

#define PXA_AC97_BASE 0x40500000UL
#define PXA_AC97_SIZE 0x00010000UL

struct AC97Fifo {
    uint8_t readPtr;
    uint8_t numItems;
    uint8_t dmaChannelNum;
    uint8_t isRxFifo;
    uint32_t data[16];

    uint8_t *icr, *isr;

    uint32_t lastReadSample;
};

struct Ac97CodecStruct {
    Ac97CodecRegR regR;
    Ac97CodecRegW regW;
    Ac97CodecFifoR fifoR;
    Ac97CodecFifoW fifoW;
    void *userData;

    uint16_t prevReadVal;

    struct AC97Fifo txFifo;
    struct AC97Fifo rxFifo;
};

struct SocAC97 {
    struct SocDma *dma;
    struct SocIc *ic;

    uint8_t pocr, picr, mccr, posr, pisr, mcsr, car, mocr, mosr, micr, misr;
    uint32_t gcr, gsr, pcdr;

    // prmary audio is PCM
    // secondary is mic
    // primary modem is modem
    struct Ac97CodecStruct primaryAudio, secondaryAudio, primaryModem, secondaryModem;
};

static void socAC97prvIrqUpdate(struct SocAC97 *ac97) {
    bool irq = false;

    irq = irq || !!(ac97->posr & (ac97->pocr << 1) & 0x14);
    irq = irq || !!(ac97->pisr & (ac97->picr << 1) & 0x14);
    irq = irq || !!(ac97->mccr & (ac97->mcsr << 1) & 0x14);
    irq = irq || !!(ac97->mocr & (ac97->mosr << 1) & 0x14);
    irq = irq || !!(ac97->micr & (ac97->misr << 1) & 0x14);

    irq = irq || (ac97->gcr & ac97->gsr & 0x300);
    irq = irq || (ac97->gsr & 0x000c0000ul);

    socIcInt(ac97->ic, PXA_I_AC97, irq);
}

static void socAC97PrvFifoDmaUpdate(struct SocAC97 *ac97, struct AC97Fifo *fifo) {
    bool fifoReadyForRead = fifo->isRxFifo ? (fifo->numItems >= 8) : (fifo->numItems < 8);

    if (fifoReadyForRead)
        *fifo->isr |= 4;
    else
        *fifo->isr &= ~4;

    if (fifo->dmaChannelNum) socDmaExternalReq(ac97->dma, fifo->dmaChannelNum, fifoReadyForRead);
}

static bool socAC97PrvFifoAdd(struct SocAC97 *ac97, struct AC97Fifo *fifo, uint32_t val) {
    uint32_t ptr, cap = sizeof(fifo->data) / sizeof(*fifo->data);
    bool ret = false;

    if (fifo->numItems == cap)
        *fifo->isr |= 0x10;  // full
    else {
        ret = true;

        ptr = (fifo->readPtr + fifo->numItems++) % cap;
        fifo->data[ptr] = val;
    }

    socAC97PrvFifoDmaUpdate(ac97, fifo);

    return ret;
}

static bool socAC97PrvFifoGet(struct SocAC97 *ac97, struct AC97Fifo *fifo, uint32_t *valP) {
    uint32_t cap = sizeof(fifo->data) / sizeof(*fifo->data);
    bool ret = false;

    if (!fifo->numItems) {
        if (valP) *valP = fifo->lastReadSample;

        *fifo->isr |= 0x10;  // empty
    } else {
        fifo->lastReadSample = fifo->data[fifo->readPtr];

        if (valP) *valP = fifo->lastReadSample;

        if (++fifo->readPtr == cap) fifo->readPtr = 0;

        fifo->numItems--;

        ret = true;
    }

    socAC97PrvFifoDmaUpdate(ac97, fifo);

    return ret;
}

static bool socAC97PrvFifoW(struct SocAC97 *ac97, struct Ac97CodecStruct *codec, uint32_t val) {
    return socAC97PrvFifoAdd(ac97, &codec->txFifo, val);
}

static bool socAC97PrvFifoR(struct SocAC97 *ac97, struct Ac97CodecStruct *codec, uint32_t *valP) {
    return socAC97PrvFifoGet(ac97, &codec->rxFifo, valP);
}

static bool socAC97PrvPcmFifoW(struct SocAC97 *ac97, uint32_t val) {
    return socAC97PrvFifoW(ac97, &ac97->primaryAudio, val);
}

static bool socAC97PrvPcmFifoR(struct SocAC97 *ac97, uint32_t *valP) {
    return socAC97PrvFifoR(ac97, &ac97->primaryAudio, valP);
}

static bool socAC97PrvMicFifoR(struct SocAC97 *ac97, uint32_t *valP) {
    return socAC97PrvFifoR(ac97, &ac97->secondaryAudio, valP);
}

static bool socAC97PrvModemFifoW(struct SocAC97 *ac97, uint32_t val) {
    return socAC97PrvFifoW(ac97, &ac97->primaryModem, val);
}

static bool socAC97PrvModemFifoR(struct SocAC97 *ac97, uint32_t *valP) {
    return socAC97PrvFifoR(ac97, &ac97->primaryModem, valP);
}

static bool socAC97PrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                 void *buf) {
    struct SocAC97 *ac97 = (struct SocAC97 *)userData;
    struct Ac97CodecStruct *cd = NULL;
    uint32_t val = 0;

    pa -= PXA_AC97_BASE;

    if (size != 4 && size != 2) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa >>= 2;

    if (write) val = (size == 2) ? *(uint16_t *)buf : *(uint32_t *)buf;

    switch (pa) {
        case 0:
            if (write) {
                ac97->pocr = val & 0x0e;
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->pocr;
            break;

        case 1:
            if (write) {
                ac97->picr = val & 0x0a;
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->picr;
            break;

        case 2:
            if (write) {
                ac97->mccr = val & 0x0a;
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->mccr;
            break;

        case 3:
            if (write) {
                ac97->gcr = val & 0x000c033ful;
                ac97->gsr = (ac97->gsr & ~8ul) | (val & 8);  // set shut off status
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->gcr;
            break;

        case 4:
            if (write) {
                ac97->posr &= ~(val & 0x10);
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->posr;
            break;

        case 5:
            if (write) {
                ac97->pisr &= ~(val & 0x18);
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->pisr;
            break;

        case 6:
            if (write) {
                ac97->mcsr &= ~(val & 0x18);
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->mcsr;
            break;

        case 7:
            if (write) {
                ac97->gsr &= ~(val & 0x000c8c01ul);
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->gsr;
            break;

        case 8:
            if (write)
                ac97->car &= 0xfffffffeul | (val & 1);
            else if (ac97->car)
                val = 1;
            else {
                val = 0;
                ac97->car = 1;
            }
            break;

        case 16:
            if (write)
                return socAC97PrvPcmFifoW(ac97, val);
            else
                return socAC97PrvPcmFifoR(ac97, (uint32_t *)buf);
            break;

        case 24:
            if (write)
                return false;
            else
                return socAC97PrvMicFifoR(ac97, (uint32_t *)buf);
            break;

        case 64:
            if (write) {
                ac97->mocr = val & 0x0a;
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->mocr;
            break;

        case 66:
            if (write) {
                ac97->micr = val & 0x0a;
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->micr;
            break;

        case 68:
            if (write) {
                ac97->mosr &= ~(val & 0x10);
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->mosr;
            break;

        case 70:
            if (write) {
                ac97->misr &= ~(val & 0x18);
                socAC97prvIrqUpdate(ac97);
            } else
                val = ac97->misr;
            break;

        case 80:
            if (write)
                return socAC97PrvModemFifoW(ac97, val);
            else
                return socAC97PrvModemFifoR(ac97, (uint32_t *)buf);
            break;
    }
    if (pa >= 0x080 && pa < 0x0c0)
        cd = &ac97->primaryAudio;
    else if (pa >= 0x0c0 && pa < 0x100)
        cd = &ac97->secondaryAudio;
    else if (pa >= 0x0100 && pa < 0x140)
        cd = &ac97->primaryModem;
    else if (pa >= 0x0140 && pa < 0x180)
        cd = &ac97->secondaryModem;

    if (cd) {
        uint16_t readVal = cd->prevReadVal;

        pa &= 0x3f;
        pa *= 2;

        if (write && cd->regW && cd->regW(cd->userData, pa, val))
            ac97->gsr |= 0x00080000ul;
        else if (!write && cd->regR && cd->regR(cd->userData, pa, &cd->prevReadVal))
            ac97->gsr |= 0x00040000ul;

        ac97->car = 0;
        val = readVal;

        socAC97prvIrqUpdate(ac97);
    }

    if (!write) {
        if (size == 2)
            *(uint16_t *)buf = val;
        else
            *(uint32_t *)buf = val;
    }

    return true;
}

struct SocAC97 *socAC97Init(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma) {
    struct SocAC97 *ac97 = (struct SocAC97 *)malloc(sizeof(*ac97));

    if (!ac97) ERR("cannot alloc AC97");

    memset(ac97, 0, sizeof(*ac97));

    ac97->ic = ic;
    ac97->dma = dma;
    ac97->gsr = 0x100;  // primary codec is ready

    ac97->primaryAudio.txFifo.dmaChannelNum = DMA_CMR_AC97_AUDIO_TX;
    ac97->primaryAudio.txFifo.icr = &ac97->pocr;
    ac97->primaryAudio.txFifo.isr = &ac97->posr;

    ac97->primaryAudio.rxFifo.dmaChannelNum = DMA_CMR_AC97_AUDIO_RX;
    ac97->primaryAudio.rxFifo.isRxFifo = true;
    ac97->primaryAudio.rxFifo.icr = &ac97->picr;
    ac97->primaryAudio.rxFifo.isr = &ac97->pisr;

    // for easiness we treat mic as a secondary codec in our data structures
    ac97->secondaryAudio.rxFifo.dmaChannelNum = DMA_CMR_AC97_MIC;
    ac97->secondaryAudio.rxFifo.isRxFifo = true;
    ac97->secondaryAudio.rxFifo.icr = &ac97->mccr;
    ac97->secondaryAudio.rxFifo.isr = &ac97->mcsr;

    ac97->primaryModem.txFifo.dmaChannelNum = DMA_CMR_AC97_MODEM_TX;
    ac97->primaryModem.txFifo.icr = &ac97->mocr;
    ac97->primaryModem.txFifo.isr = &ac97->mosr;

    ac97->primaryModem.rxFifo.dmaChannelNum = DMA_CMR_AC97_MODEM_RX;
    ac97->primaryModem.rxFifo.isRxFifo = true;
    ac97->primaryModem.rxFifo.icr = &ac97->micr;
    ac97->primaryModem.rxFifo.isr = &ac97->misr;

    if (!memRegionAdd(physMem, PXA_AC97_BASE, PXA_AC97_SIZE, socAC97PrvMemAccessF, ac97))
        ERR("cannot add AC97 to MEM\n");

    return ac97;
}

void socAC97Periodic(struct SocAC97 *ac97) {
    // nothing - codecs do their own work getting data to and from us :)
}

static struct Ac97CodecStruct *socAC97prvCodecPtrGet(struct SocAC97 *ac97, enum Ac97Codec which) {
    switch (which) {
        case Ac97PrimaryAudio:
            return &ac97->primaryAudio;

        case Ac97SecondaryAudio:
            return &ac97->secondaryAudio;

        case Ac97PrimaryModem:
            return &ac97->primaryModem;

        case Ac97SecondaryModem:
            return &ac97->secondaryModem;

        default:
            return NULL;
    }
}

void socAC97clientAdd(struct SocAC97 *ac97, enum Ac97Codec which, Ac97CodecRegR regR,
                      Ac97CodecRegW regW, void *userData) {
    struct Ac97CodecStruct *cd = socAC97prvCodecPtrGet(ac97, which);

    cd->regR = regR;
    cd->regW = regW;
    cd->userData = userData;
}

bool socAC97clientClientWantData(struct SocAC97 *ac97, enum Ac97Codec which, uint32_t *dataPtr) {
    struct Ac97CodecStruct *cd = socAC97prvCodecPtrGet(ac97, which);

    (void)socAC97PrvFifoGet(ac97, &cd->txFifo, dataPtr);

    // they still get data, just last valid data. not fresh new data
    return true;
}

void socAC97clientClientHaveData(struct SocAC97 *ac97, enum Ac97Codec which, uint32_t data) {
    struct Ac97CodecStruct *cd = socAC97prvCodecPtrGet(ac97, which);

    (void)socAC97PrvFifoAdd(ac97, &cd->rxFifo, data);
}
