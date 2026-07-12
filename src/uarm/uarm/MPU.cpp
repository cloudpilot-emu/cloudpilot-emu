#include "MPU.h"

#include <cstddef>
#include <cstring>

#include "cputil.h"
#include "savestate/savestateAll.h"

#define MPU_NUM_REGIONS 8
#define CACHE_EMPTY_VALUE (MPU_NUM_REGIONS | (MPU_NUM_REGIONS << 4))

#define SAVESTATE_VERSION 0

struct MpuRegion {
    bool enabled{false};
    uint32_t base{0};
    uint32_t size{0};

    uint32_t config;
    uint8_t cacheable;
    uint8_t ap{0};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do32(base).Do32(size).Do32(config).Do(typename T::Pack8() << cacheable << ap);
    }
};

struct ArmMpu {
    bool enabled{false};

    // 4k pages, 8 pages per cache entry -> 32 - 12 - 3 = 17
    uint32_t regionCache[1 << 17];

    // last region is a dummy and always disabled
    MpuRegion regions[MPU_NUM_REGIONS + 1];
    uint8_t cacheable{0};
    uint8_t bufferable{0};
    uint16_t ap{0};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do(typename T::Pack8() << cacheable << bufferable).Do16(ap);
        for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) regions[i].DoSaveLoad(chunkHelper);
    }
};

namespace {
    void rebuildCache(ArmMpu* mpu) {
        memset(mpu->regionCache, CACHE_EMPTY_VALUE, sizeof(mpu->regionCache));

        for (int8_t iRegion = MPU_NUM_REGIONS - 1; iRegion >= 0; iRegion--) {
            const MpuRegion& region = mpu->regions[iRegion];
            const size_t pages = region.size >> 12;

            for (size_t page = region.base >> 12; page < pages + (region.base >> 12); page++) {
                uint32_t& cacheEntry = mpu->regionCache[page >> 3];
                const uint32_t shift = (page & 0x07) << 2;

                cacheEntry &= ~(0x0f << shift);
                cacheEntry |= iRegion << shift;
            }
        }
    }
}  // namespace

ArmMpu* mpuCreate() {
    ArmMpu* mpu = new ArmMpu();
    mpuReset(mpu);

    return mpu;
}

void mpuReset(ArmMpu* mpu) {
    mpu->enabled = false;
    mpu->cacheable = 0;
    mpu->bufferable = 0;
    mpu->ap = 0;

    for (MpuRegion& region : mpu->regions) {
        region.enabled = false;
        region.ap = 0;
        region.cacheable = 0;
        region.config = 0;
        region.base = 0;
        region.size = 0;
    }

    memset(mpu->regionCache, CACHE_EMPTY_VALUE, sizeof(mpu->regionCache));
}

MPUTestResult mpuTestAddress(ArmMpu* mpu, uint32_t pa, bool write, bool privileged) {
    if (!mpu->enabled) return 1;

    // 4k pages, 8 regions per entry -> 12 + 3 = 15
    uint32_t regionIndex = mpu->regionCache[pa >> 15];
    regionIndex >>= ((regionIndex >> 12) & 0x07) << 2;
    regionIndex &= 0x0f;

    const MpuRegion& region = mpu->regions[regionIndex];
    if (!region.enabled) return 0;

    switch (region.ap) {
        case 0:
            return 0;

        case 1:
            return privileged | region.cacheable;

        case 2:
            return (privileged || !write) | region.cacheable;

        case 3:
            return 1 | region.cacheable;

        default:
            __builtin_unreachable();
    }
}

void mpuSetEnabled(ArmMpu* mpu, bool enabled) { mpu->enabled = enabled; }

void mpuSetCacheable(ArmMpu* mpu, uint8_t cacheable) {
    mpu->cacheable = cacheable;

    for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) {
        mpu->regions[i].cacheable = ((cacheable >> i) & 0x01) ? MPU_TEST_RESULT_BIT_CACHEABLE : 0;
    }
}

void mpuSetBufferable(ArmMpu* mpu, uint8_t bufferable) { mpu->bufferable = bufferable; }

void mpuSetAP(ArmMpu* mpu, uint16_t ap) {
    mpu->ap = ap;

    for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) {
        mpu->regions[i].ap = (ap >> (i << 1)) & 0x03;
    }
}

void mpuSetRegionConfig(ArmMpu* mpu, uint8_t iRegion, uint32_t config) {
    MpuRegion& region = mpu->regions[iRegion];

    region.config = config;
    region.enabled = config & 0x01;
    region.base = config & 0xfffff000;
    region.size = 1 << (((config >> 1) & 0x1f) + 1);

    rebuildCache(mpu);
}

bool mpuIsEnabled(ArmMpu* mpu) { return mpu->enabled; }

uint8_t mpuGetCacheable(ArmMpu* mpu) { return mpu->cacheable; }

uint8_t mpuGetBufferable(ArmMpu* mpu) { return mpu->bufferable; }

uint16_t mpuGetAP(ArmMpu* mpu) { return mpu->ap; }

uint32_t mpuGetRegionConfig(ArmMpu* mpu, uint8_t region) { return mpu->regions[region].config; }

template <typename T>
void mpuSave(struct ArmMpu* mpu, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::mpu, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    mpu->DoSaveLoad(helper);
}

template <typename T>
void mpuLoad(struct ArmMpu* mpu, T& loader) {
    auto chunk = loader.GetChunkOrFail(ChunkType::mpu, SAVESTATE_VERSION, "mpu");
    if (!chunk) return;

    mpuReset(mpu);

    LoadChunkHelper helper(*chunk);
    mpu->DoSaveLoad(helper);

    rebuildCache(mpu);
}

template void mpuSave<Savestate<ChunkType>>(ArmMpu* mpu, Savestate<ChunkType>& savestate);
template void mpuSave<SavestateProbe<ChunkType>>(ArmMpu* mpu, SavestateProbe<ChunkType>& savestate);
template void mpuLoad<SavestateLoader<ChunkType>>(ArmMpu* mpu, SavestateLoader<ChunkType>& loader);
