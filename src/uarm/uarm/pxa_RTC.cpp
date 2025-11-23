//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_RTC.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cputil.h"
#include "mem.h"
#include "pxa_IC.h"
#include "savestate/savestateAll.h"
#include "timeutil.h"

#define PXA_RTC_BASE 0x40900000UL
#define PXA_RTC_SIZE 0x00001000UL

#define SAVESTATE_VERSION 1

struct PxaRtc {
    struct SocIc *ic;

    uint64_t lastAlarmCheck;

    uint32_t RTAR;  // RTC alarm
    uint32_t RTTR;  // RTC trim - we ignore this alltogether
    uint8_t RTSR;   // RTC status

    template <typename T>
    void DoSaveLoad(T &chunkHelper, uint32_t version) {
        if (version == 0) {
            uint32_t lastSeenTime = 0, RCNR = 0;
            chunkHelper.Do32(lastSeenTime).Do32(RCNR).Do32(RTAR).Do32(RTTR).Do8(RTSR);
        } else {
            chunkHelper.Do32(RTAR).Do32(RTTR).Do8(RTSR).Do64(lastAlarmCheck);
        }
    }
};

static void pxaRtcPrvUpdateInterrupts(struct PxaRtc *rtc) {
    socIcInt(rtc->ic, PXA_I_RTC_ALM, !!(rtc->RTSR & 1));
    socIcInt(rtc->ic, PXA_I_RTC_HZ, !!(rtc->RTSR & 2));
}

static void pxaRtcPrvUpdateAlarm(struct PxaRtc *rtc) {
    if (rtc->lastAlarmCheck < rtc->RTAR) {
        const uint64_t now = palmEpochSeconds();

        if ((rtc->RTSR & 0x04) && now >= rtc->RTAR) rtc->RTSR |= 1;

        rtc->lastAlarmCheck = now;
    }

    socIcInt(rtc->ic, PXA_I_RTC_ALM, !!(rtc->RTSR & 1));
}

static void pxaRtcTickHz(struct PxaRtc *rtc) {
    if (rtc->RTSR & 0x8)  // send HZ interrupt
        rtc->RTSR |= 2;

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
                break;

            case 1:
                rtc->RTAR = val;
                rtc->lastAlarmCheck = palmEpochSeconds() - 1;
                pxaRtcPrvUpdateAlarm(rtc);

                break;

            case 2:
                rtc->RTSR = (val & ~3UL) | ((rtc->RTSR & ~val) & 3UL);
                pxaRtcPrvUpdateInterrupts(rtc);

                break;

            case 3:
                if (!(rtc->RTTR & 0x80000000UL)) rtc->RTTR = val;
                break;
        }
    } else {
        switch (pa) {
            case 0:
                val = palmEpochSeconds();
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

    return rtc;
}

void pxaRtcTick(struct PxaRtc *rtc) {
    pxaRtcPrvUpdateAlarm(rtc);
    pxaRtcTickHz(rtc);
}

template <typename T>
void pxaRtcSave(struct PxaRtc *rtc, T &savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pxaRtc, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    rtc->DoSaveLoad(helper, SAVESTATE_VERSION);
}

template <typename T>
void pxaRtcLoad(struct PxaRtc *rtc, T &loader) {
    uint32_t gotVersion;
    auto chunk = loader.GetChunkOrFail(ChunkType::pxaRtc, SAVESTATE_VERSION, "pxa rtc", gotVersion);
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    rtc->DoSaveLoad(helper, gotVersion);

    if (gotVersion == 0) rtc->lastAlarmCheck = palmEpochSeconds();
}

template void pxaRtcSave<Savestate<ChunkType>>(PxaRtc *rtc, Savestate<ChunkType> &savestate);
template void pxaRtcSave<SavestateProbe<ChunkType>>(PxaRtc *rtc,
                                                    SavestateProbe<ChunkType> &savestate);
template void pxaRtcLoad<SavestateLoader<ChunkType>>(PxaRtc *rtc,
                                                     SavestateLoader<ChunkType> &loader);