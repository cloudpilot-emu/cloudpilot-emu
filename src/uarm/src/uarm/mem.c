//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "mem.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

#define NUM_MEM_REGIONS 128

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

bool memRegionAdd(struct ArmMem *mem, uint32_t pa, uint32_t sz, ArmMemAccessF aF, void *uD) {
    uint_fast8_t i;

    // check for intersection with another region

    for (i = 0; i < NUM_MEM_REGIONS; i++) {
        if (!mem->regions[i].sz) continue;
        if ((mem->regions[i].pa <= pa && mem->regions[i].pa + mem->regions[i].sz > pa) ||
            (pa <= mem->regions[i].pa && pa + sz > mem->regions[i].pa))
            return false;  // intersection -> fail
    }

    // find a free region and put it there

    for (i = 0; i < NUM_MEM_REGIONS; i++) {
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

bool memAccess(struct ArmMem *mem, uint32_t addr, uint_fast8_t size, uint_fast8_t accessType,
               void *buf) {
    bool ret = false, wantWrite = !!(accessType & ~MEM_ACCCESS_FLAG_NOERROR);
    uint_fast8_t i;

    for (i = 0; i < NUM_MEM_REGIONS; i++) {
        if (mem->regions[i].pa <= addr && mem->regions[i].pa + mem->regions[i].sz > addr) {
            ret = mem->regions[i].aF(mem->regions[i].uD, addr, size, wantWrite, buf);
            break;
        }
    }

    if (!ret && !(accessType & MEM_ACCCESS_FLAG_NOERROR)) {
        fprintf(stderr, "Memory %s of %u bytes to PA 0x%08lx fails\n", wantWrite ? "write" : "read",
                size, (unsigned long)addr);
        while (1)
            ;  // make debugging easier
    }

    return ret;
}
