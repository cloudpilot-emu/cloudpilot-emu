//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_RTC.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_IC.h"
#include "util.h"

#define PXA_RTC_BASE 0x40900000UL
#define PXA_RTC_SIZE 0x00001000UL

struct PxaRtc {
    struct SocIc *ic;

    uint32_t lastSeenTime;
    uint32_t RCNR;  // RTC counter offset from our local time
    uint32_t RTAR;  // RTC alarm
    uint32_t RTTR;  // RTC trim - we ignore this alltogether

    uint8_t RTSR;  // RTC status
};

void pxaRtcPrvUpdate(struct PxaRtc *rtc) {
    if (rtc->lastSeenTime != rtc->RCNR) {  // do not triger alarm more than once per second please

        if (rtc->RTSR & 0x4)  // check alarm
            rtc->RTSR |= 1;

        if (rtc->RTSR & 0x8)  // send HZ interrupt
            rtc->RTSR |= 2;

        rtc->lastSeenTime = rtc->RCNR;
    }
    socIcInt(rtc->ic, PXA_I_RTC_ALM, !!(rtc->RTSR & 1));
    socIcInt(rtc->ic, PXA_I_RTC_HZ, !!(rtc->RTSR & 2));
}

static bool pxaRtcPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                void *buf) {
    struct PxaRtc *rtc = (struct PxaRtc *)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_RTC_BASE) >> 2;

    if (write) {
        val = *(uint32_t *)buf;

        switch (pa) {
            case 0:
                rtc->RCNR = val;
                break;

            case 1:
                rtc->RTAR = val;
                pxaRtcPrvUpdate(rtc);
                break;

            case 2:
                rtc->RTSR = (val & ~3UL) | ((rtc->RTSR & ~val) & 3UL);
                pxaRtcPrvUpdate(rtc);
                break;

            case 3:
                if (!(rtc->RTTR & 0x80000000UL)) rtc->RTTR = val;
                break;
        }
    } else {
        switch (pa) {
            case 0:
                val = rtc->RCNR;
                break;

            case 1:
                val = rtc->RTAR;
                break;

            case 2:
                val = rtc->RTSR;
                break;

            case 3:
                val = rtc->RTTR;
                break;
        }
        *(uint32_t *)buf = val;
    }

    return true;
}

struct PxaRtc *pxaRtcInit(struct ArmMem *physMem, struct SocIc *ic) {
    struct PxaRtc *rtc = (struct PxaRtc *)malloc(sizeof(*rtc));

    if (!rtc) ERR("cannot alloc RTC");

    memset(rtc, 0, sizeof(*rtc));
    rtc->ic = ic;
    rtc->RTTR = 0x7FFF;  // nice default value

    if (!memRegionAdd(physMem, PXA_RTC_BASE, PXA_RTC_SIZE, pxaRtcPrvMemAccessF, rtc))
        ERR("cannot add RTC to MEM\n");

    // clockRegisterConsumer(clock, 1000000000ULL, pxaRtcUpdate, rtc);

    return rtc;
}

void pxaRtcTick(struct PxaRtc *rtc) {
    rtc->RCNR++;
    pxaRtcPrvUpdate(rtc);
}
