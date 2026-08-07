//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "mem.h"

#include <cstdlib>
#include <cstring>

#include "RAM.h"
#include "ROM.h"
#include "cputil.h"

#define NUM_MEM_REGIONS 128

#define REGION_RAM 0
#define REGION_ROM 1
#define REGION_BASE 2

struct ArmMemRegion {
    uint32_t pa;
    uint32_t ub;
    ArmMemAccessF aF;
    void *uD;
};

struct ArmMem {
    struct ArmMemRegion regions[NUM_MEM_REGIONS];
};

struct ArmMem *memInit(void) {
    struct ArmMem *mem = (struct ArmMem *)malloc(sizeof(*mem));

    if (!mem) ERR("cannot alloc MEM");

    memset(mem, 0, sizeof(*mem));

    return mem;
}

void memDeinit(struct ArmMem *mem) { (void)mem; }

static bool checkForIntersection(struct ArmMem *mem, uint32_t pa, uint32_t sz) {
    uint_fast8_t i;

    for (i = 0; i < NUM_MEM_REGIONS; i++) {
        if (!mem->regions[i].ub) continue;
        if ((mem->regions[i].pa <= pa && mem->regions[i].ub > pa) ||
            (pa <= mem->regions[i].pa && pa + sz > mem->regions[i].pa))
            return true;  // intersection -> fail
    }

    return false;
}

static bool memRegionAddFixed(struct ArmMem *mem, uint8_t region, uint32_t pa, uint32_t sz,
                              ArmMemAccessF af, void *uD) {
    mem->regions[region].pa = pa;
    mem->regions[region].aF = af;
    mem->regions[region].uD = uD;
    mem->regions[region].ub = pa + sz;

    return true;
}

bool memRegionAdd(struct ArmMem *mem, uint32_t pa, uint32_t sz, ArmMemAccessF aF, void *uD) {
    uint_fast8_t i;

    if (checkForIntersection(mem, pa, sz)) return false;

    // find a free region and put it there

    for (i = REGION_BASE; i < NUM_MEM_REGIONS; i++) {
        if (mem->regions[i].ub == 0) {
            mem->regions[i].pa = pa;
            mem->regions[i].aF = aF;
            mem->regions[i].uD = uD;
            mem->regions[i].ub = pa + sz;

            return true;
        }
    }

    // fail miserably

    return false;
}

bool memRegionAddRam(struct ArmMem *mem, uint32_t pa, uint32_t sz, ArmMemAccessF af, void *uD) {
    return memRegionAddFixed(mem, REGION_RAM, pa, sz, af, uD);
}

bool memRegionAddRom(struct ArmMem *mem, uint32_t pa, uint32_t sz, ArmMemAccessF af, void *uD) {
    return memRegionAddFixed(mem, REGION_ROM, pa, sz, af, uD);
}

template <int size, bool write>
bool memAccess(struct ArmMem *mem, uint32_t addr, void *buf) {
    const uint32_t ub = addr + size;

    if (mem->regions[REGION_RAM].pa <= addr && mem->regions[REGION_RAM].ub >= ub) {
        return ramAccessF<size, write>(mem->regions[REGION_RAM].uD, addr, buf);
    }

    if (mem->regions[REGION_ROM].pa <= addr && mem->regions[REGION_ROM].ub >= ub)
        return romAccessF<size, write>(mem->regions[REGION_ROM].uD, addr, buf);

    bool ret = false;
    uint_fast8_t i;

    for (i = REGION_BASE; i < NUM_MEM_REGIONS; i++) {
        if (mem->regions[i].pa <= addr && mem->regions[i].ub >= ub) {
            ret = mem->regions[i].aF(mem->regions[i].uD, addr, size, write, buf);
            break;
        }
    }

    return ret;
}

#define DECLARE_MEM_ACCESS_SIZE(sz)                                                   \
    template bool memAccess<sz, true>(struct ArmMem * mem, uint32_t addr, void *buf); \
    template bool memAccess<sz, false>(struct ArmMem * mem, uint32_t addr, void *buf);

DECLARE_MEM_ACCESS_SIZE(1)
DECLARE_MEM_ACCESS_SIZE(2)
DECLARE_MEM_ACCESS_SIZE(4)
DECLARE_MEM_ACCESS_SIZE(8)
DECLARE_MEM_ACCESS_SIZE(16)
DECLARE_MEM_ACCESS_SIZE(32)
DECLARE_MEM_ACCESS_SIZE(64)

bool memAccess(struct ArmMem *mem, uint32_t addr, uint_fast8_t size, bool write, void *buf) {
    const uint32_t ub = addr + size;

    if (mem->regions[REGION_RAM].pa <= addr && mem->regions[REGION_RAM].ub >= ub) {
        return ramAccessF(mem->regions[REGION_RAM].uD, addr, size, write, buf);
    }

    if (mem->regions[REGION_ROM].pa <= addr && mem->regions[REGION_ROM].ub >= ub)
        return romAccessF(mem->regions[REGION_ROM].uD, addr, size, write, buf);

    bool ret = false;
    uint_fast8_t i;

    for (i = REGION_BASE; i < NUM_MEM_REGIONS; i++) {
        if (mem->regions[i].pa <= addr && mem->regions[i].ub >= ub) {
            ret = mem->regions[i].aF(mem->regions[i].uD, addr, size, write, buf);
            break;
        }
    }

    return ret;
}

template <int size>
bool memInstructionFetch(struct ArmMem *mem, uint32_t addr, void *buf) {
    const uint32_t ub = addr + size;

    if (mem->regions[REGION_RAM].pa <= addr && mem->regions[REGION_RAM].ub >= ub)
        return ramAccessF<size, false>(mem->regions[REGION_RAM].uD, addr, buf);

    if (mem->regions[REGION_ROM].pa <= addr && mem->regions[REGION_ROM].ub >= ub)
        return romInstructionFetch<size>(mem->regions[REGION_ROM].uD, addr, buf);

    bool ret = false;
    uint_fast8_t i;

    for (i = REGION_BASE; i < NUM_MEM_REGIONS; i++) {
        if (mem->regions[i].pa <= addr && mem->regions[i].ub >= ub) {
            ret = mem->regions[i].aF(mem->regions[i].uD, addr, size, false, buf);
            break;
        }
    }

    return ret;
}

template bool memInstructionFetch<1>(struct ArmMem *mem, uint32_t addr, void *buf);
template bool memInstructionFetch<2>(struct ArmMem *mem, uint32_t addr, void *buf);
template bool memInstructionFetch<4>(struct ArmMem *mem, uint32_t addr, void *buf);
template bool memInstructionFetch<8>(struct ArmMem *mem, uint32_t addr, void *buf);
template bool memInstructionFetch<16>(struct ArmMem *mem, uint32_t addr, void *buf);
template bool memInstructionFetch<32>(struct ArmMem *mem, uint32_t addr, void *buf);
template bool memInstructionFetch<64>(struct ArmMem *mem, uint32_t addr, void *buf);
