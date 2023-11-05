//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "icache.h"

#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "uarm_endian.h"
#include "util.h"

struct icacheline {
    uint8_t data[32];
    uint32_t revision;
};

struct icache {
    struct ArmMem* mem;
    struct ArmMmu* mmu;

    uint32_t revision;
    struct icacheline* cache[4096];
};

void icacheInval(struct icache* ic) {
    ic->revision++;

    if (ic->revision == 0) {
        ic->revision = 1;

        for (size_t i = 0; i < 4096; i++) {
            struct icacheline* lvl1 = ic->cache[i];
            if (!lvl1) continue;

            for (size_t j = 0; j < 32768; j++) lvl1[j].revision = 0;
        }
    }
}

struct icache* icacheInit(struct ArmMem* mem, struct ArmMmu* mmu) {
    struct icache* ic = (struct icache*)malloc(sizeof(*ic));

    if (!ic) ERR("cannot alloc icache");

    memset(ic, 0, sizeof(*ic));

    ic->mem = mem;
    ic->mmu = mmu;

    icacheInval(ic);

    return ic;
}

void icacheInvalAddr(struct icache* ic, uint32_t va) {
    const size_t i = va >> 20;
    struct icacheline* lvl1 = ic->cache[i];
    if (!lvl1) return;

    const size_t j = (va >> 5) & 0x7fff;
    if (lvl1[j].revision != ic->revision) return;

    lvl1[j].revision--;
}

bool icacheFetch(struct icache* ic, uint32_t va, uint_fast8_t sz, uint_fast8_t* fsrP, void* buf) {
    uint32_t pa;
    size_t i, j;
    struct icacheline *lvl1, *line;
    uint8_t mappingInfo;

    if (va & (sz - 1)) {  // alignment issue

        if (fsrP) *fsrP = 3;
        return false;
    }

    i = va >> 20;
    j = (va >> 5) & 0x7fff;

    lvl1 = ic->cache[i];
    if (!lvl1) {
        lvl1 = ic->cache[i] = (struct icacheline*)malloc(32768 * sizeof(struct icacheline));
        memset(lvl1, 0, 32768 * sizeof(struct icacheline));
    }

    line = lvl1 + j;

    if (line->revision != ic->revision) {
        if (!mmuTranslate(ic->mmu, va, true, false, &pa, fsrP, &mappingInfo)) return false;

        if ((mappingInfo & MMU_MAPPING_CACHEABLE) == 0) {
            if (!memAccess(ic->mem, pa, sz, MEM_ACCESS_TYPE_READ, buf)) {
                if (fsrP) *fsrP = 0x0d;  // perm error
                return false;
            }

            return true;
        }

        if (!memAccess(ic->mem, pa & 0xffffffe0, 32, MEM_ACCESS_TYPE_READ, line->data)) {
            if (fsrP) *fsrP = 0x0d;  // perm error
            return false;
        };

        line->revision = ic->revision;
    }

    switch (sz) {
        case 4:
            *(uint32_t*)buf = *(uint32_t*)(line->data + (va & 0x1f));
            break;

        case 2:
            *(uint16_t*)buf = *(uint16_t*)(line->data + (va & 0x1f));
            break;

        default:
            memcpy(buf, line->data + (va & 0x1f), sz);
    }

    return true;
}
