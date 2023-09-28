//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa270_UDC.h"

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "util.h"

#define PXA_UDC_BASE 0x40600000UL
#define PXA_UDC_SIZE 0x00001000UL

struct Ep {
    uint32_t ccra;  // unused for ep0
    uint16_t csr;
    uint16_t bcr;
};

struct Pxa270Udc {
    struct SocDma *dma;
    struct SocIc *ic;

    struct Ep ep[24];
    uint32_t udccr, udcicr[2], udcisr[2], udcotgicr, udcotgisr, up2ocr;
    uint16_t udcfnr;
    uint8_t up3ocr;
};

static bool pxa270UdcPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                   void *buf) {
    struct Pxa270Udc *udc = (struct Pxa270Udc *)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_UDC_BASE) >> 2;

    if (write) val = *(uint32_t *)buf;

    if (pa >= 0x100 / 4 && pa < 0x160 / 4) {
        uint_fast8_t epNo = pa - 0x100 / 4;

        if (!write)
            val = udc->ep[epNo].csr;
        else if (!epNo)
            udc->ep[epNo].csr = ((udc->ep[epNo].csr & 0x1ff) | (val & 0x20e)) & ~(val & 0x191);
        else
            udc->ep[epNo].csr = ((udc->ep[epNo].csr & 0x3d7) | (val & 0x1a8)) & ~(val & 0x016);
    } else if (pa >= 0x200 / 4 && pa < 0x260 / 4) {
        uint_fast8_t epNo = pa - 0x200 / 4;

        if (write)
            ;  // ignored
        else
            val = udc->ep[epNo].bcr;
    } else if (pa >= 0x300 / 4 && pa < 0x360 / 4) {
        uint_fast8_t epNo = pa - 0x300 / 4;

        ERR("FIFO %c for USB EP %d\n", write ? 'W' : 'R', epNo);
    } else if (pa >= 0x404 / 4 && pa < 0x460 / 4) {
        uint_fast8_t epNo = pa - 0x400 / 4;

        if (write) {
            udc->ep[epNo].ccra = val & 0x07fffffful;
        } else
            val = udc->ep[epNo].ccra;
    } else
        switch (pa) {
            case 0x000 / 4:  // UDCCR
                if (write)
                    udc->udccr = ((udc->udccr & 0x70011ffeul) | (val & 0x80000001ul)) &
                                 ~(val & 0x0000001cul);
                else
                    val = udc->udccr;
                break;

            case 0x04 / 4:  // UDICICR0
                if (write)
                    udc->udcicr[0] = val;
                else
                    val = udc->udcicr[0];
                break;

            case 0x08 / 4:  // UDICICR1
                if (write)
                    udc->udcicr[1] = val & 0xf800fffful;
                else
                    val = udc->udcicr[1];
                break;

            case 0x0c / 4:  // UDICISR0
                if (write)
                    udc->udcisr[0] = val;
                else
                    val = udc->udcisr[0];
                break;

            case 0x10 / 4:  // UDICISR1
                if (write)
                    udc->udcisr[1] = val & 0xf800fffful;
                else
                    val = udc->udcisr[1];
                break;

            case 0x14 / 4:  // UDCFNR
                if (write)
                    ;  // ignored
                else
                    val = udc->udcfnr;
                break;

            case 0x18 / 4:  // UDCOTGICR
                if (write)
                    udc->udcotgicr = val & 0x010303fful;
                else
                    val = udc->udcotgicr;
                break;

            case 0x1c / 4:  // UDCOTGISR
                if (write)
                    udc->udcotgisr &= ~(val & 0x010303fful);
                else
                    val = udc->udcotgisr;
                break;

            case 0x20 / 4:  // UP2OCR
                if (write)
                    udc->up2ocr = val & 0x070307fful;
                else
                    val = udc->up2ocr;
                break;

            case 0x24 / 4:  // UP3OCR
                if (write)
                    udc->up3ocr = val & 3;
                else
                    val = udc->up3ocr;
                break;

            default:
                return false;
        }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

struct Pxa270Udc *pxa270UdcInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma) {
    struct Pxa270Udc *udc = (struct Pxa270Udc *)malloc(sizeof(*udc));

    if (!udc) ERR("cannot alloc UDC");

    memset(udc, 0, sizeof(*udc));

    udc->ic = ic;
    udc->dma = dma;
    udc->up2ocr = 0x00030000ul;

    if (!memRegionAdd(physMem, PXA_UDC_BASE, PXA_UDC_SIZE, pxa270UdcPrvMemAccessF, udc))
        ERR("cannot add UDC to MEM\n");

    return udc;
}
