#include "pv_timer.h"

#include <cstdint>
#include <cstdio>

#include "cputil.h"
#include "mem.h"
#include "pv_ic.h"
#include "savestate/savestateAll.h"

#define TIMER_BASE 0x30000600
#define TIMER_SIZE 12
#define TIMER_OFFSET_TIMER_LO 0x00
#define TIMER_OFFSET_TIMER_HI 0x04
#define TIMER_OFFSET_ALARM 0x08

#define IRQ_NO_TIMER 4

#define SAVESTATE_VERSION 0

struct PvTimer {
    uint64_t timer{0};
    uint32_t alarm{0};
    bool signalling{false};

    uint32_t interruptsSuspendCount{0};

    PvIc* ic;

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do64(timer).Do32(alarm).DoBool(signalling).Do32(interruptsSuspendCount);
    }
};

static void pvTimerUpdateInterrupt(PvTimer* timer) {
    if (timer->interruptsSuspendCount > 0 && timer->signalling) return;

    pvIcInt(timer->ic, IRQ_NO_TIMER, timer->signalling);
}

static bool pvIcPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                              void* buf) {
    if (size != 4) {
        fprintf(stderr, "invalid write to IC\n");
        return false;
    }

    auto timer = reinterpret_cast<PvTimer*>(userData);
    pa >>= 2;

    switch (pa - (TIMER_BASE >> 2)) {
        case (TIMER_OFFSET_TIMER_LO >> 2):
            if (write) {
                timer->signalling = false;
                pvTimerUpdateInterrupt(timer);
            } else {
                *reinterpret_cast<uint32_t*>(buf) = timer->timer;
            }

            break;

        case (TIMER_OFFSET_TIMER_HI >> 2):
            if (write) {
                return false;
            } else {
                *reinterpret_cast<uint32_t*>(buf) = timer->timer >> 32;
            }

            break;

        case (TIMER_OFFSET_ALARM >> 2):
            if (write) {
                timer->alarm = *reinterpret_cast<uint32_t*>(buf);
            } else {
                *reinterpret_cast<uint32_t*>(buf) = timer->alarm;
            }

            break;

        default:
            return false;
    }

    return true;
}

PvTimer* pvTimerInit(ArmMem* mem, PvIc* ic) {
    auto timer = new PvTimer();

    timer->ic = ic;

    memRegionAdd(mem, TIMER_BASE, TIMER_SIZE, pvIcPrvMemAccessF, ic);

    return timer;
}

void pvTimerTick(PvTimer* timer) {
    timer->timer++;

    if (timer->alarm == 0) return;

    if (--timer->alarm == 0) {
        timer->signalling = true;
        pvTimerUpdateInterrupt(timer);
    }
}

void pvTimerSuspendInterrupts(PvTimer* timer, bool suspend) {
    if (suspend) {
        timer->interruptsSuspendCount++;
    } else if (timer->interruptsSuspendCount > 0) {
        timer->interruptsSuspendCount--;
    }

    pvTimerUpdateInterrupt(timer);
}

template <typename T>
void pvTimerSave(struct PvTimer* timer, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pvTimer, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    timer->DoSaveLoad(helper);
}

template <typename T>
void pvTimerLoad(struct PvTimer* timer, T& loader) {
    auto chunk = loader.GetChunkOrFail(ChunkType::pvTimer, SAVESTATE_VERSION, "pvTimer");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    timer->DoSaveLoad(helper);
}

template void pvTimerSave<Savestate<ChunkType>>(PvTimer* ic, Savestate<ChunkType>& savestate);
template void pvTimerSave<SavestateProbe<ChunkType>>(PvTimer* ic,
                                                     SavestateProbe<ChunkType>& savestate);
template void pvTimerLoad<SavestateLoader<ChunkType>>(PvTimer* ic,
                                                      SavestateLoader<ChunkType>& loader);
