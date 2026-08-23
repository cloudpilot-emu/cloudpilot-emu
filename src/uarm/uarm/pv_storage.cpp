#include "pv_storage.h"

#include <cstdint>
#include <cstdio>

#include "RAM.h"
#include "cputil.h"
#include "mem.h"
#include "pv_ic.h"
#include "savestate/savestateAll.h"
#include "sdcard.h"

struct PvStorage {
    uint32_t pa{0};
    bool inserted{false};

    ArmRam* ram{nullptr};
    PvIc* ic{nullptr};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do32(pa).DoBool(inserted);
    }
};

#define STORAGE_BASE 0x30000800
#define STORAGE_SIZE 12
#define STORAGE_OFFSET_NUM_SEC 0
#define STORAGE_OFFSET_OP 4
#define STORAGE_OFFSET_PA 8

#define IRQ_NO_STORAGE 8

#define ERR_NONE 0
#define ERR_OUT_OF_RANGE 1
#define ERR_BUS_READ 4
#define ERR_WRITE_FAILED 5
#define ERR_READ_FAILED 6
#define ERR_BUS_WRITE 7

#define SAVESTATE_VERSION 0

static void pvStoragePrvRead(PvStorage* storage, uint32_t sec) {
    void* mappedBuffer = ramResolveAddress(storage->ram, storage->pa, SD_SECTOR_SIZE);
    if (!mappedBuffer) {
        storage->pa = ERR_BUS_READ;
        return;
    }

    if (sec >= sdCardSectorCount()) {
        storage->pa = ERR_OUT_OF_RANGE;
        return;
    }

    storage->pa = sdCardRead(sec, mappedBuffer) ? ERR_NONE : ERR_READ_FAILED;
}

static void pvStoragePrvWrite(PvStorage* storage, uint32_t sec) {
    void* mappedBuffer = ramResolveAddress(storage->ram, storage->pa, SD_SECTOR_SIZE);
    if (!mappedBuffer) {
        storage->pa = ERR_BUS_WRITE;
        return;
    }

    if (sec >= sdCardSectorCount()) {
        storage->pa = ERR_OUT_OF_RANGE;
        return;
    }

    storage->pa = sdCardWrite(sec, mappedBuffer) ? ERR_NONE : ERR_WRITE_FAILED;
}

static bool pvStoragePrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                   void* buf) {
    if (size != 4) {
        fprintf(stderr, "invalid access to storage interface\n");
        return false;
    }

    auto storage = reinterpret_cast<PvStorage*>(userData);
    uint32_t& value = *reinterpret_cast<uint32_t*>(buf);

    switch ((pa - STORAGE_BASE) >> 2) {
        case (STORAGE_OFFSET_NUM_SEC >> 2):
            if (write) {
                pvIcInt(storage->ic, IRQ_NO_STORAGE, false);
            } else {
                value = storage->inserted ? (0x80000000 | sdCardSectorCount()) : 0;
            }

            break;

        case (STORAGE_OFFSET_OP >> 2):
            if (!write) return false;

            if (storage->inserted) {
                if (value >> 31) {
                    pvStoragePrvWrite(storage, value & 0x7fffffff);
                } else {
                    pvStoragePrvRead(storage, value & 0x7fffffff);
                }

            } else {
                storage->pa = ERR_OUT_OF_RANGE;
            }

            break;

        case (STORAGE_OFFSET_PA >> 2):
            if (write) {
                storage->pa = value;
            } else {
                value = storage->pa;
            }

            break;

        default:
            return false;
    }

    return true;
}

PvStorage* pvStorageInit(ArmMem* mem, PvIc* ic, ArmRam* ram) {
    auto storage = new PvStorage();

    storage->ic = ic;
    storage->ram = ram;

    memRegionAdd(mem, STORAGE_BASE, STORAGE_SIZE, pvStoragePrvMemAccessF, storage);

    return storage;
}

void pvStorageInsert(PvStorage* storage) {
    if (storage->inserted) return;
    storage->inserted = true;

    pvIcInt(storage->ic, IRQ_NO_STORAGE, true);
}

void pvStorageEject(PvStorage* storage) {
    if (!storage->inserted) return;
    storage->inserted = false;

    pvIcInt(storage->ic, IRQ_NO_STORAGE, true);
}

template <typename T>
void pvStorageSave(struct PvStorage* storage, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pvStorage, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    storage->DoSaveLoad(helper);
}

template <typename T>
void pvStorageLoad(struct PvStorage* storage, T& loader) {
    auto chunk = loader.GetChunkOrFail(ChunkType::pvStorage, SAVESTATE_VERSION, "pvStorage");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    storage->DoSaveLoad(helper);
}

template void pvStorageSave<Savestate<ChunkType>>(PvStorage* storage,
                                                  Savestate<ChunkType>& savestate);
template void pvStorageSave<SavestateProbe<ChunkType>>(PvStorage* storage,
                                                       SavestateProbe<ChunkType>& savestate);
template void pvStorageLoad<SavestateLoader<ChunkType>>(PvStorage* timer,
                                                        SavestateLoader<ChunkType>& loader);
