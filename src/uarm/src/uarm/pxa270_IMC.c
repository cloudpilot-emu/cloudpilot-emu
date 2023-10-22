//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa270_IMC.h"

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "util.h"

#define PXA270_IMC_BASE 0x58000000ul
#define PXA270_IMC_SIZE 0x0c

struct PxaImc {
    uint32_t mcr;
    uint8_t impmsr;
};

static bool pxaImcPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                void* buf) {
    struct PxaImc* imc = (struct PxaImc*)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA270_IMC_BASE) >> 2;

    if (write) val = *(uint32_t*)buf;

    switch (pa) {
        case 0x00 / 4:
            if (write)
                imc->mcr = val & 0x00ff0ffful;
            else
                val = imc->mcr;
            break;

        case 0x08 / 4:
            if (write)
                return false;  // forbidden
            else
                val = 0;  // all in run mode, forever
            break;
    }

    if (!write) *(uint32_t*)buf = val;

    return true;
}

struct PxaImc* pxaImcInit(struct ArmMem* physMem) {
    struct PxaImc* imc = (struct PxaImc*)malloc(sizeof(*imc));

    if (!imc) ERR("cannot alloc IMC");

    memset(imc, 0, sizeof(*imc));

    if (!memRegionAdd(physMem, PXA270_IMC_BASE, PXA270_IMC_SIZE, pxaImcPrvMemAccessF, imc))
        ERR("cannot add IMC to MEM\n");

    return imc;
}
