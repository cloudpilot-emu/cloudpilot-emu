//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa255_UDC.h"

#include <stdlib.h>
#include <string.h>

#include "cputil.h"
#include "mem.h"
#include "savestate/savestateAll.h"

#define PXA_UDC_BASE 0x40600000UL
#define PXA_UDC_SIZE 0x00001000UL

#define SAVESTATE_VERSION 0

struct Pxa255Udc {
    struct SocDma *dma;
    struct SocIc *ic;

    uint32_t reg4;

    uint8_t ccr, uicr0, uicr1;

    template <typename T>
    void DoSaveLoad(T &chunkHelper) {
        chunkHelper.Do32(reg4).Do(typename T::Pack8() << ccr << uicr0 << uicr1);
    }
};

static bool pxa255UdcPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                   void *buf) {
    struct Pxa255Udc *udc = (struct Pxa255Udc *)userData;
    uint32_t val;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_UDC_BASE) >> 2;

    if (write) val = *(uint32_t *)buf;

    switch (pa) {
        case 0:  // UDCCR
            if (write)
                udc->ccr = val;
            else
                val = udc->ccr;
            break;

        case 0x01:  // undocumented reg
            if (write)
                udc->reg4 = val;
            else
                val = udc->reg4;
            break;

        case 0x14:  // UICR0
            if (write)
                udc->uicr0 = val;
            else
                val = udc->uicr0;
            break;

        case 0x15:  // UICR1
            if (write)
                udc->uicr1 = val;
            else
                val = udc->uicr1;
            break;

        // Stub those for now
        case 0x04:  // UDCCS0
        case 0x16:  // USIR0
        case 0x17:  // USIR1
        case 0x18:  // UFNRH
            if (!write) val = 0;
            break;

            // other regs, TODO

        default:
            return false;
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

struct Pxa255Udc *pxa255UdcInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma) {
    struct Pxa255Udc *udc = (struct Pxa255Udc *)malloc(sizeof(*udc));

    if (!udc) ERR("cannot alloc UDC");

    memset(udc, 0, sizeof(*udc));

    udc->ic = ic;
    udc->dma = dma;
    udc->ccr = 0xa0;
    udc->uicr0 = 0xff;
    udc->uicr1 = 0xff;

    if (!memRegionAdd(physMem, PXA_UDC_BASE, PXA_UDC_SIZE, pxa255UdcPrvMemAccessF, udc))
        ERR("cannot add UDC to MEM\n");

    return udc;
}

template <typename T>
void pxa255UdcSave(struct Pxa255Udc *udc, T &savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pxa255udc, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    udc->DoSaveLoad(helper);
}

template <typename T>
void pxa255UdcLoad(struct Pxa255Udc *udc, T &loader) {
    auto chunk = loader.GetChunk(ChunkType::pxa255udc, SAVESTATE_VERSION, "pxa255 udc");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    udc->DoSaveLoad(helper);
}

template void pxa255UdcSave<Savestate<ChunkType>>(Pxa255Udc *udc, Savestate<ChunkType> &savestate);
template void pxa255UdcSave<SavestateProbe<ChunkType>>(Pxa255Udc *udc,
                                                       SavestateProbe<ChunkType> &savestate);
template void pxa255UdcLoad<SavestateLoader<ChunkType>>(Pxa255Udc *udc,
                                                        SavestateLoader<ChunkType> &loader);
