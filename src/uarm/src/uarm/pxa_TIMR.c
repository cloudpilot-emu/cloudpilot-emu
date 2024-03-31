//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_TIMR.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_IC.h"
#include "util.h"

#define PXA_TIMR_BASE 0x40A00000UL
#define PXA_TIMR_SIZE 0x00010000UL

struct PxaTimr {
    struct SocIc *ic;

    uint32_t OSMR[4];  // Match Register 0-3
    uint32_t OSCR;     // Counter Register
    uint8_t OIER;      // Interrupt Enable
    uint8_t OWER;      // Watchdog enable
    uint8_t OSSR;      // Status Register
};

static void pxaTimrPrvRaiseLowerInts(struct PxaTimr *timr) {
    socIcInt(timr->ic, PXA_I_TIMR0, (timr->OSSR & 1) != 0);
    socIcInt(timr->ic, PXA_I_TIMR1, (timr->OSSR & 2) != 0);
    socIcInt(timr->ic, PXA_I_TIMR2, (timr->OSSR & 4) != 0);
    socIcInt(timr->ic, PXA_I_TIMR3, (timr->OSSR & 8) != 0);
}

static void pxaTimrPrvCheckMatchSingleStep(struct PxaTimr *timr, uint_fast8_t idx) {
    uint_fast8_t v = 1UL << idx;

    if (timr->OSCR == timr->OSMR[idx]) {
        if (idx == 3 && timr->OWER) ERR("WDT fires\n");

        if (timr->OIER & v) timr->OSSR |= v;
    }
}

static void pxaTimrPrvCheckMatchMultiStep(struct PxaTimr *timr, uint_fast8_t idx, uint32_t oscrOld,
                                          uint32_t batchedTicks) {
    uint_fast8_t v = 1UL << idx;

    if (batchedTicks >= (uint32_t)(timr->OSMR[idx] - oscrOld)) {
        if (idx == 3 && timr->OWER) ERR("WDT fires\n");

        if (timr->OIER & v) timr->OSSR |= v;
    }
}

static void pxaTimrPrvUpdateSingleStep(struct PxaTimr *timr) {
    uint_fast8_t i;

    for (i = 0; i < 4; i++) pxaTimrPrvCheckMatchSingleStep(timr, i);

    pxaTimrPrvRaiseLowerInts(timr);
}

static void pxaTimrPrvUpdateMultiStep(struct PxaTimr *timr, uint32_t oscrOld,
                                      uint32_t batchedTicks) {
    uint_fast8_t i;

    for (i = 0; i < 4; i++) pxaTimrPrvCheckMatchMultiStep(timr, i, oscrOld, batchedTicks);

    pxaTimrPrvRaiseLowerInts(timr);
}

static bool pxaTimrPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                 void *buf) {
    struct PxaTimr *timr = (struct PxaTimr *)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;  // we do not support non-word accesses
    }

    pa = (pa - PXA_TIMR_BASE) >> 2;

    if (write) {
        val = *(uint32_t *)buf;

        switch (pa) {
            case 0:
            case 1:
            case 2:
            case 3:
                timr->OSMR[pa] = val;
                pxaTimrPrvUpdateSingleStep(timr);
                break;

            case 4:
                timr->OSCR = val;
                break;

            case 5:
                timr->OSSR = timr->OSSR & ~val;
                pxaTimrPrvUpdateSingleStep(timr);
                break;

            case 6:
                timr->OWER = val;
                break;

            case 7:
                timr->OIER = val;
                pxaTimrPrvUpdateSingleStep(timr);
                break;
        }
    } else {
        switch (pa) {
            case 0:
            case 1:
            case 2:
            case 3:
                val = timr->OSMR[pa];
                break;

            case 4:
                val = timr->OSCR;
                break;

            case 5:
                val = timr->OSSR;
                break;

            case 6:
                val = timr->OWER;
                break;

            case 7:
                val = timr->OIER;
                break;
        }
        *(uint32_t *)buf = val;
    }

    return true;
}

struct PxaTimr *pxaTimrInit(struct ArmMem *physMem, struct SocIc *ic) {
    struct PxaTimr *timr = (struct PxaTimr *)malloc(sizeof(*timr));

    if (!timr) ERR("cannot alloc OSTIMER");

    memset(timr, 0, sizeof(*timr));
    timr->ic = ic;

    if (!memRegionAdd(physMem, PXA_TIMR_BASE, PXA_TIMR_SIZE, pxaTimrPrvMemAccessF, timr))
        ERR("cannot add OSTIMER to MEM\n");

    return timr;
}

void pxaTimrTick(struct PxaTimr *timr, uint32_t batchedTicks) {
    if (batchedTicks == 1) {
        timr->OSCR++;
        pxaTimrPrvUpdateSingleStep(timr);
    } else {
        const uint32_t oscrOld = timr->OSCR;
        timr->OSCR += batchedTicks;

        pxaTimrPrvUpdateMultiStep(timr, oscrOld, batchedTicks);
    }
}

uint32_t pxaTimrTicksToNextInterrupt(struct PxaTimr *timr) {
    uint32_t ticksToNextInterrupt = ~0;

    for (uint8_t i = 0; i < 4; i++) {
        const uint8_t v = 1UL << i;

        if ((timr->OIER & v) == 0) continue;
        if ((timr->OSSR & v) != 0) return 1;

        const uint32_t delta = timr->OSMR[i] - timr->OSCR;

        if (delta < ticksToNextInterrupt && delta != 0) ticksToNextInterrupt = delta;
    }

    return ticksToNextInterrupt;
}
