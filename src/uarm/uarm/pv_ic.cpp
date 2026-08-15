#include "pv_ic.h"

#include <cstdint>
#include <cstdio>

#include "CPU.h"
#include "cputil.h"
#include "mem.h"
#include "savestate/savestateAll.h"

#define IC_BASE 0x30000000
#define IC_SIZE 0x08
#define IC_OFFSET_MASK 0x00
#define IC_OFFSET_STATE 0x04

#define SAVESTATE_VERSION 0

struct PvIc {
    uint32_t mask{0};
    uint32_t state{0};
    bool signalling{false};

    ArmCpu* cpu{nullptr};
    ArmMem* mem{nullptr};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do32(mask).Do32(state).DoBool(signalling);
    }
};

static void pvIcUpdate(PvIc* pv) {
    const bool signalling = pv->state & pv->mask;
    if (signalling == pv->signalling) return;

    cpuIrq(pv->cpu, false, signalling);
    pv->signalling = signalling;
}

static bool pvIcPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                              void* buf) {
    if (size != 4) {
        fprintf(stderr, "invalid write to IC\n");
        return false;
    }

    auto ic = reinterpret_cast<PvIc*>(userData);
    pa >>= 2;

    switch (pa - (IC_BASE >> 2)) {
        case (IC_OFFSET_MASK >> 2):
            if (write) {
                ic->mask = *reinterpret_cast<uint32_t*>(buf);
                pvIcUpdate(ic);
            } else {
                *reinterpret_cast<uint32_t*>(buf) = ic->mask;
            }

            break;

        case (IC_OFFSET_STATE):
            if (write) {
                return false;
            } else {
                *reinterpret_cast<uint32_t*>(buf) = ic->state;
            }

            break;

        default:
            return false;
    }

    return true;
}

PvIc* pvIcInit(ArmCpu* cpu, ArmMem* mem) {
    auto ic = new PvIc();

    ic->cpu = cpu;
    ic->mem = mem;

    memRegionAdd(mem, IC_BASE, IC_SIZE, pvIcPrvMemAccessF, ic);

    return ic;
}

void pvIcInt(PvIc* ic, uint8_t intNo, bool raise) {
    if (intNo > 31) ERR("invalid interrupt");

    if (raise) {
        ic->state |= (1 << intNo);
    } else {
        ic->state &= ~(1 << intNo);
    }

    pvIcUpdate(ic);
}

template <typename T>
void pvIcSave(struct PvIc* ic, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pvIc, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    ic->DoSaveLoad(helper);
}

template <typename T>
void pvIcLoad(struct PvIc* ic, T& loader) {
    auto chunk = loader.GetChunkOrFail(ChunkType::pvIc, SAVESTATE_VERSION, "pvIc");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    ic->DoSaveLoad(helper);
}

template void pvIcSave<Savestate<ChunkType>>(PvIc* ic, Savestate<ChunkType>& savestate);
template void pvIcSave<SavestateProbe<ChunkType>>(PvIc* ic, SavestateProbe<ChunkType>& savestate);
template void pvIcLoad<SavestateLoader<ChunkType>>(PvIc* ic, SavestateLoader<ChunkType>& loader);
