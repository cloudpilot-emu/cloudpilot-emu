#include "pv_touch.h"

#include <cstdio>

#include "cputil.h"
#include "mem.h"
#include "pv_ic.h"
#include "savestate/savestateAll.h"

#define TOUCH_BASE 0x30000300
#define TOUCH_SIZE 4

#define IRQ_NO_TOUCH 1

#define SAVESTATE_VERSION 0

struct PvTouch {
    uint32_t state{0};

    PvIc* ic{nullptr};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do32(state);
    }
};

static bool pvTouchPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                 void* buf) {
    if (size != 4 || write) {
        fprintf(stderr, "invalid access to touch interface\n");
        return false;
    }

    auto touch = reinterpret_cast<PvTouch*>(userData);

    pvIcInt(touch->ic, IRQ_NO_TOUCH, false);
    *reinterpret_cast<uint32_t*>(buf) = touch->state;

    return true;
}

PvTouch* pvTouchInit(ArmMem* mem, PvIc* ic) {
    auto touch = new PvTouch();

    touch->ic = ic;

    memRegionAdd(mem, TOUCH_BASE, TOUCH_SIZE, pvTouchPrvMemAccessF, touch);

    return touch;
}

void pvTouchUpdate(PvTouch* touch, int16_t x, int16_t y) {
    const uint32_t newState = static_cast<uint16_t>(x) | (static_cast<uint16_t>(y) << 16);
    if (newState == touch->state) return;

    touch->state = newState;

    pvIcInt(touch->ic, IRQ_NO_TOUCH, true);
}

template <typename T>
void pvTouchSave(struct PvTouch* touch, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pvTouch, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    touch->DoSaveLoad(helper);
}

template <typename T>
void pvTouchLoad(struct PvTouch* touch, T& loader) {
    auto chunk = loader.GetChunkOrFail(ChunkType::pvTouch, SAVESTATE_VERSION, "pvTouch");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    touch->DoSaveLoad(helper);
}

template void pvTouchSave<Savestate<ChunkType>>(PvTouch* touch, Savestate<ChunkType>& savestate);
template void pvTouchSave<SavestateProbe<ChunkType>>(PvTouch* touch,
                                                     SavestateProbe<ChunkType>& savestate);
template void pvTouchLoad<SavestateLoader<ChunkType>>(PvTouch* touch,
                                                      SavestateLoader<ChunkType>& loader);
