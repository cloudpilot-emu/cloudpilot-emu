#include "pv_rtc.h"

#include <cstdint>
#include <cstdio>

#include "mem.h"
#include "pv_ic.h"
#include "timeutil.h"

#define RTC_BASE 0x30000400
#define RTC_SIZE 8
#define RTC_OFFSET_TIME 0
#define RTC_OFFSET_CLEAR_IRQ 4

#define IRQ_NO_RTC 2

struct PvRtc {
    uint32_t currentSeconds{0};

    PvIc* ic{nullptr};
};

static void pvRtcUpdate(PvRtc* rtc) { rtc->currentSeconds = palmEpochSeconds(); }

static bool pvRtcPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                               void* buf) {
    if (size != 4) {
        fprintf(stderr, "invalid RTC access\n");
        return false;
    }

    auto rtc = reinterpret_cast<PvRtc*>(userData);
    uint32_t& value = *reinterpret_cast<uint32_t*>(buf);

    switch ((pa - RTC_BASE) >> 2) {
        case RTC_OFFSET_TIME >> 2:
            if (!write) {
                value = rtc->currentSeconds;
            }

            break;

        case RTC_OFFSET_CLEAR_IRQ >> 2:
            if (write) {
                pvIcInt(rtc->ic, IRQ_NO_RTC, false);
            } else {
                value = 0;
            }

            break;

        default:
            return false;
    }

    return true;
}

void pvRtcTick(PvRtc* rtc) {
    pvRtcUpdate(rtc);
    pvIcInt(rtc->ic, IRQ_NO_RTC, true);
}

PvRtc* pvRtcInit(ArmMem* mem, PvIc* ic) {
    auto rtc = new PvRtc();

    pvRtcUpdate(rtc);
    rtc->ic = ic;

    memRegionAdd(mem, RTC_BASE, RTC_SIZE, pvRtcPrvMemAccessF, rtc);

    return rtc;
}
