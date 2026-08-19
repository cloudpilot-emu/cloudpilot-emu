#include "pv_keys.h"

#include <cstdint>
#include <cstdio>

#include "cputil.h"
#include "keys.h"
#include "mem.h"
#include "pv_ic.h"
#include "savestate/savestateAll.h"

#define KEYS_BASE 0x30000500
#define KEYS_SIZE 20

#define IRQ_NO_KEYS 3

#define SAVESTATE_VERSION 0

struct PvKeys {
    uint32_t state[5]{};

    PvIc* ic;

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        for (auto& state : state) {
            chunkHelper.Do32(state);
        }
    }
};

static uint64_t mapKey(KeyId key) {
    switch (key) {
        case keyIdPower:
            return 0x00000001;

        case keyIdUp:
            return 0x00000002;

        case keyIdDown:
            return 0x00000004;

        case keyIdHard1:
            return 0x00000008;

        case keyIdHard2:
            return 0x00000010;

        case keyIdHard3:
            return 0x00000020;

        case keyIdHard4:
            return 0x00000040;

        case keyIdLeft:
            return 0x01000000;

        case keyIdRight:
            return 0x02000000;

        case keyIdSelect:
            return 0x08000000;

        default:
            return 0;
    }
}

static bool pvKeysPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                void* buf) {
    const auto keys = reinterpret_cast<PvKeys*>(userData);

    if (write) {
        pvIcInt(keys->ic, IRQ_NO_KEYS, false);
        return true;
    }

    if (size != 4) {
        fprintf(stderr, "invalid access to keys\n");
        return false;
    }

    *reinterpret_cast<uint32_t*>(buf) = keys->state[(pa - KEYS_BASE) >> 2];

    return true;
}

PvKeys* pvKeysInit(ArmMem* mem, PvIc* ic) {
    auto keys = new PvKeys();
    keys->ic = ic;

    memRegionAdd(mem, KEYS_BASE, KEYS_SIZE, pvKeysPrvMemAccessF, keys);

    return keys;
}

void pvKeysEngage(PvKeys* keys, KeyId key, bool down) {
    const auto mapped = mapKey(key);

    const uint8_t index = mapped >> 32;
    const uint32_t mask = mapped;

    if (down) {
        if (keys->state[index] & mask) return;
        keys->state[index] |= mask;
    } else {
        if (!(keys->state[index] & mask)) return;
        keys->state[index] &= ~mask;
    }

    pvIcInt(keys->ic, IRQ_NO_KEYS, true);
}

template <typename T>
void pvKeysSave(struct PvKeys* keys, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pvKeys, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    keys->DoSaveLoad(helper);
}

template <typename T>
void pvKeysLoad(struct PvKeys* keys, T& loader) {
    auto chunk = loader.GetChunkOrFail(ChunkType::pvKeys, SAVESTATE_VERSION, "pvKeys");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    keys->DoSaveLoad(helper);
}

template void pvKeysSave<Savestate<ChunkType>>(PvKeys* keys, Savestate<ChunkType>& savestate);
template void pvKeysSave<SavestateProbe<ChunkType>>(PvKeys* keys,
                                                    SavestateProbe<ChunkType>& savestate);
template void pvKeysLoad<SavestateLoader<ChunkType>>(PvKeys* keys,
                                                     SavestateLoader<ChunkType>& loader);
