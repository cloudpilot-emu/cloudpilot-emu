//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "mem.h"

#include <stdlib.h>
#include <string.h>

#include "RAM.h"
#include "ROM.h"
#include "util.h"

#define NUM_MEM_REGIONS 128

#define REGION_RAM 0
#define REGION_ROM 1
#define REGION_BASE 2

struct ArmMemRegion {
    uint32_t pa;
    uint32_t sz;
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
        if (!mem->regions[i].sz) continue;
        if ((mem->regions[i].pa <= pa && mem->regions[i].pa + mem->regions[i].sz > pa) ||
            (pa <= mem->regions[i].pa && pa + sz > mem->regions[i].pa))
            return true;  // intersection -> fail
    }

    return false;
}

static bool memRegionAddFixed(struct ArmMem *mem, uint8_t region, uint32_t pa, uint32_t sz,
                              ArmMemAccessF af, void *uD) {
    mem->regions[region].pa = pa;
    mem->regions[region].sz = sz;
    mem->regions[region].aF = af;
    mem->regions[region].uD = uD;

    return true;
}

bool memRegionAdd(struct ArmMem *mem, uint32_t pa, uint32_t sz, ArmMemAccessF aF, void *uD) {
    uint_fast8_t i;

    if (checkForIntersection(mem, pa, sz)) return false;

    // find a free region and put it there

    for (i = REGION_BASE; i < NUM_MEM_REGIONS; i++) {
        if (mem->regions[i].sz == 0) {
            mem->regions[i].pa = pa;
            mem->regions[i].sz = sz;
            mem->regions[i].aF = aF;
            mem->regions[i].uD = uD;

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

bool memAccess(struct ArmMem *mem, uint32_t addr, uint_fast8_t size, bool write, void *buf) {
    if (mem->regions[REGION_RAM].pa <= addr &&
        mem->regions[REGION_RAM].pa + mem->regions[REGION_RAM].sz > addr) {
        return ramAccessF(mem->regions[REGION_RAM].uD, addr, size, write, buf);
    }

    if (mem->regions[REGION_ROM].pa <= addr &&
        mem->regions[REGION_ROM].pa + mem->regions[REGION_ROM].sz > addr)
        return romAccessF(mem->regions[REGION_ROM].uD, addr, size, write, buf);

    bool ret = false;
    uint_fast8_t i;

    for (i = REGION_BASE; i < NUM_MEM_REGIONS; i++) {
        if (mem->regions[i].pa <= addr && mem->regions[i].pa + mem->regions[i].sz > addr) {
            ret = mem->regions[i].aF(mem->regions[i].uD, addr, size, write, buf);
            break;
        }
    }

    return ret;
}

bool memInstructionFetch(struct ArmMem *mem, uint32_t addr, uint_fast8_t size, void *buf) {
    if (mem->regions[REGION_RAM].pa <= addr &&
        mem->regions[REGION_RAM].pa + mem->regions[REGION_RAM].sz > addr)
        return ramAccessF(mem->regions[REGION_RAM].uD, addr, size, false, buf);

    if (mem->regions[REGION_ROM].pa <= addr &&
        mem->regions[REGION_ROM].pa + mem->regions[REGION_ROM].sz > addr)
        return romInstructionFetch(mem->regions[REGION_ROM].uD, addr, size, buf);

    bool ret = false;
    uint_fast8_t i;

    for (i = REGION_BASE; i < NUM_MEM_REGIONS; i++) {
        if (mem->regions[i].pa <= addr && mem->regions[i].pa + mem->regions[i].sz > addr) {
            ret = mem->regions[i].aF(mem->regions[i].uD, addr, size, false, buf);
            break;
        }
    }

    return ret;
}
