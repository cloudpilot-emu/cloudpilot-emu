#include "MPU.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "cputil.h"
#include "savestate/savestateAll.h"

#define MPU_NUM_REGIONS 8
#define CACHE_EMPTY_VALUE (MPU_NUM_REGIONS | (MPU_NUM_REGIONS << 4))

#define SAVESTATE_VERSION 0

#define MPU_FORCE_CACHEABLE

struct MpuRegion {
    bool enabled{false};
    uint32_t base{0};
    uint32_t mask{0};
    uint8_t sizeTag{0};

    uint32_t config{0};
    uint8_t cacheable{0};
    uint8_t ap{0};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do32(config);
    }
};

struct ArmMpu {
    bool enabled{false};

    // 4k pages, 8 pages per cache entry -> 32 - 12 - 3 = 17
    uint32_t regionCache[1 << 17];

    // last region is a dummy and always disabled
    MpuRegion regions[MPU_NUM_REGIONS + 1]{};
    uint8_t cacheable{0};
    uint8_t bufferable{0};
    uint16_t ap{0};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do(typename T::Pack8() << cacheable << bufferable).Do16(ap).DoBool(enabled);

        for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) regions[i].DoSaveLoad(chunkHelper);
    }
};

namespace {
    void clearCache(ArmMpu* mpu) {
        memset(mpu->regionCache, CACHE_EMPTY_VALUE, sizeof(mpu->regionCache));
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

    clearCache(mpu);
    for (MpuRegion& region : mpu->regions) region = MpuRegion{};
}

static FORCE_INLINE uint8_t getRegionIndex(ArmMpu* mpu, uint32_t pa) {
    // 4k pages, 8 regions per entry -> 12 + 3 = 15
    uint32_t regionIndex = mpu->regionCache[pa >> 15];
    regionIndex >>= ((regionIndex >> 12) & 0x07) << 2;
    regionIndex &= 0x0f;

    return regionIndex;
}

MPUTestResult mpuTestAddress(ArmMpu* mpu, uint32_t pa, bool privileged, bool write) {
    if (!mpu->enabled) return 1;
    return 3;

    const MpuRegion& region = mpu->regions[getRegionIndex(mpu, pa)];
#ifdef MPU_FORCE_CACHEABLE
    const uint32_t cacheable = MPU_TEST_RESULT_BIT_CACHEABLE;
#else
    const uint32_t cacheable = region.cacheable;
#endif

    if (!region.enabled) return 0;

    switch (region.ap) {
        case 0:
            return 0;

        case 1:
            return privileged | cacheable;

        case 2:
            return (privileged || !write) | cacheable;

        case 3:
            return 1 | cacheable;

        default:
            __builtin_unreachable();
    }
}

void mpuSetEnabled(ArmMpu* mpu, bool enabled) { mpu->enabled = enabled; }

void mpuSetCacheable(ArmMpu* mpu, uint8_t cacheable) {
    mpu->cacheable = cacheable;

    for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) {
        // mind the gap: we reorder regions internally (see below)
        mpu->regions[i].cacheable = ((cacheable << i) & 0x80) ? MPU_TEST_RESULT_BIT_CACHEABLE : 0;
    }
}

void mpuSetBufferable(ArmMpu* mpu, uint8_t bufferable) { mpu->bufferable = bufferable; }

void mpuSetAP(ArmMpu* mpu, uint16_t ap) {
    mpu->ap = ap;

    for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) {
        // mind the gap: we reorder regions internally (see below)
        mpu->regions[i].ap = (ap >> ((MPU_NUM_REGIONS - 1 - i) << 1)) & 0x03;
    }
}

void mpuSetRegionConfig(ArmMpu* mpu, uint8_t iRegion, uint32_t config) {
    printf("configure MPU 0x%08x\n", config);

    // we order the regions internally from 0 to 8 (0 = highest priorty, 8 = fallback)
    iRegion = MPU_NUM_REGIONS - 1 - (iRegion & 0x07);
    MpuRegion& region = mpu->regions[iRegion];

    const uint32_t oldSizeTag = region.sizeTag;
    const uint32_t oldBase = region.base;
    const bool oldEnabled = region.enabled;

    region.config = config;
    region.enabled = config & 0x01;
    region.sizeTag = (config >> 1) & 0x1f;
    region.mask = region.sizeTag == 31 ? 0 : (0xffffffffu << (region.sizeTag + 1));
    region.base = config & region.mask;

    // no changes to the cache if the region was disabled and stays disabled
    if (!region.enabled && !oldEnabled) return;

    const uint32_t regionSizeTag = region.enabled ? region.sizeTag : oldSizeTag;
    const uint32_t regionBase = region.enabled ? region.base : oldBase;
    const uint32_t pages = 1 << (regionSizeTag - 11);

    for (uint32_t page = regionBase >> 12; page < pages + (regionBase >> 12); page++) {
        const uint32_t shift = (page & 0x07) << 2;
        const uint32_t mask = ~(0x0f << shift);
        const uint32_t pageIndex = page >> 3;

        const uint8_t pageRegion = (mpu->regionCache[pageIndex] >> shift) & 0x0f;

        if (region.enabled && pageRegion > iRegion) {
            // region enable and page belongs to lower priority region?
            // -> page now belongs to this region
            mpu->regionCache[pageIndex] &= mask;
            mpu->regionCache[pageIndex] |= (iRegion << shift);
        } else if (!region.enabled && pageRegion == iRegion) {
            // region disabled and page belonged to this region? look for lower priority
            // region to map page to
            uint8_t iNewRegion = iRegion + 1;
            for (; iNewRegion < MPU_NUM_REGIONS; iNewRegion++) {
                MpuRegion& newRegion = mpu->regions[iNewRegion];

                if (newRegion.enabled && newRegion.base == ((page << 12) & newRegion.mask)) break;
            }

            mpu->regionCache[pageIndex] &= mask;
            mpu->regionCache[pageIndex] |= (iNewRegion << shift);
        }
    }
}

bool mpuIsEnabled(ArmMpu* mpu) { return mpu->enabled; }

uint8_t mpuGetCacheable(ArmMpu* mpu) { return mpu->cacheable; }

uint8_t mpuGetBufferable(ArmMpu* mpu) { return mpu->bufferable; }

uint16_t mpuGetAP(ArmMpu* mpu) { return mpu->ap; }

uint32_t mpuGetRegionConfig(ArmMpu* mpu, uint8_t region) { return mpu->regions[region].config; }

void mpuDump(ArmMpu* mpu) {
    printf("enabled %u AP 0x%04x cacheable 0x%02x\n", mpu->enabled, mpu->ap, mpu->cacheable);

    for (int8_t i = MPU_NUM_REGIONS - 1; i >= 0; i--) {
        const MpuRegion& region = mpu->regions[i];

        printf("region %u config 0x%08x enabled %u base 0x%08x mask 0x%08x ap %u cacheable %u\n",
               (MPU_NUM_REGIONS - 1 - i), region.config, region.enabled, region.base, region.mask,
               region.ap, region.cacheable);
    }
}

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

    uint32_t oldConfig[MPU_NUM_REGIONS];
    for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) oldConfig[i] = mpu->regions[i].config;

    LoadChunkHelper helper(*chunk);
    mpu->DoSaveLoad(helper);

    bool configChanged = false;
    for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) {
        configChanged = configChanged || oldConfig[i] != mpu->regions[i].config;
        if (configChanged) break;
    }

    if (configChanged) {
        printf("restore cache\n");
        clearCache(mpu);
        for (uint8_t i = 0; i < MPU_NUM_REGIONS; i++) {
            mpuSetRegionConfig(mpu, MPU_NUM_REGIONS - 1 - i, mpu->regions[i].config);
        }
    }

    mpuSetAP(mpu, mpu->ap);
    mpuSetCacheable(mpu, mpu->bufferable);
}

template void mpuSave<Savestate<ChunkType>>(ArmMpu* mpu, Savestate<ChunkType>& savestate);
template void mpuSave<SavestateProbe<ChunkType>>(ArmMpu* mpu, SavestateProbe<ChunkType>& savestate);
template void mpuLoad<SavestateLoader<ChunkType>>(ArmMpu* mpu, SavestateLoader<ChunkType>& loader);
