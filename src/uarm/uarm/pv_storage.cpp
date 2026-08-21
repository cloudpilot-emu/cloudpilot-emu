#include "pv_storage.h"

#include <cstdint>
#include <cstdio>

#include "mem.h"
#include "pv_ic.h"

struct PvStorage {
    PvIc* ic{nullptr};
};

#define STORAGE_BASE 0x30000800
#define STORAGE_SIZE 12
#define STORAGE_OFFSET_NUM_SEC 0
#define STORAGE_OFFSET_OP 4
#define STORAGE_OFFSET_SEC 8

static bool pvStoragePrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                   void* buf) {
    if (size != 4) {
        fprintf(stderr, "invalid access to storage interface\n");
        return false;
    }

    uint32_t& value = *reinterpret_cast<uint32_t*>(buf);

    switch ((pa - STORAGE_BASE) >> 2) {
        case (STORAGE_OFFSET_NUM_SEC >> 2):
            value = 0;
            break;

        case (STORAGE_OFFSET_OP >> 2):
            fprintf(stderr, "hit storage op stub\n");
            break;

        case (STORAGE_OFFSET_SEC >> 2):
            fprintf(stderr, "hit storage sec stub\n");
            break;

        default:
            return false;
    }

    return true;
}

PvStorage* pvStorageInit(ArmMem* mem, PvIc* ic) {
    auto storage = new PvStorage();

    storage->ic = ic;

    memRegionAdd(mem, STORAGE_BASE, STORAGE_SIZE, pvStoragePrvMemAccessF, storage);

    return storage;
}
