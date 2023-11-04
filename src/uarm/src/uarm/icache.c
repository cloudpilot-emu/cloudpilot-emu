//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "icache.h"

#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "uarm_endian.h"
#include "util.h"

struct icacheline {
    uint8_t data[32];
    uint32_t revision : 31;
    uint32_t cacheable : 1;
};

struct icache {
    struct ArmMem* mem;
    struct ArmMmu* mmu;

    uint32_t revision;
    struct icacheline** cache[256];
};

void icacheInval(struct icache* ic) {
    ic->revision = (ic->revision + 1) & 0x80000000;

    if (ic->revision == 0) {
        ic->revision = 1;

        for (size_t i = 0; i < 256; i++) {
            struct icacheline** lvl1 = ic->cache[i];
            if (!lvl1) continue;

            for (size_t j = 0; j < 16; j++) {
                struct icacheline* lvl2 = lvl1[j];
                if (!lvl2) continue;

                for (size_t k = 0; k < 32768; k++) lvl2[k].revision = 0;
            }
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
    const size_t i = va >> 24;
    struct icacheline** lvl1 = ic->cache[i];
    if (!lvl1) return;

    const size_t j = (va >> 20) & 0xf;
    struct icacheline* lvl2 = lvl1[j];
    if (lvl2) return;

    const size_t k = (va >> 5) & 0x7fff;
    if (lvl2[k].revision == 0) return;

    lvl2[k].revision--;
}

bool icacheFetch(struct icache* ic, uint32_t va, uint_fast8_t sz, uint_fast8_t* fsrP, void* buf) {
    uint32_t pa;
    size_t i, j, k;
    struct icacheline** lvl1;
    struct icacheline *lvl2, *line;
    uint8_t mappingInfo;

    if (va & (sz - 1)) {  // alignment issue

        if (fsrP) *fsrP = 3;
        return false;
    }

    i = va >> 24;
    j = (va >> 20) & 0xf;
    k = (va >> 5) & 0x7fff;

    lvl1 = ic->cache[i];
    if (!lvl1) goto allocate;

    lvl2 = lvl1[j];
    if (!lvl2) goto allocate;

    goto check;

allocate:
    if (!mmuTranslate(ic->mmu, va, true, false, &pa, fsrP, NULL)) return false;

    if (!lvl1) {
        lvl1 = ic->cache[i] = (struct icacheline**)malloc(16 * sizeof(struct icacheline*));
        memset(lvl1, 0, 16 * sizeof(struct icacheline*));

        lvl2 = lvl1[j];
    }

    if (!lvl2) {
        lvl2 = lvl1[j] = (struct icacheline*)malloc(32768 * sizeof(struct icacheline));
        memset(lvl2, 0, 32768 * sizeof(struct icacheline));
    }

check:
    line = lvl2 + k;

    if (line->revision != ic->revision) {
        line->revision = ic->revision;
        line->cacheable = false;

        if (!mmuTranslate(ic->mmu, va & 0xffffffe0, true, false, &pa, fsrP, &mappingInfo))
            goto read;
        if ((mappingInfo & MMU_MAPPING_CACHEABLE) == 0) goto read;
        if (!memAccess(ic->mem, pa, 32, MEM_ACCESS_TYPE_READ, line->data)) goto read;

        line->cacheable = true;
    }

read:
    if (!line->cacheable) {
        if (!mmuTranslate(ic->mmu, va, true, false, &pa, fsrP, NULL)) return false;

        if (!memAccess(ic->mem, pa, sz, MEM_ACCESS_TYPE_READ, buf)) {
            if (fsrP) *fsrP = 0x0d;  // perm error
            return false;
        }

        return true;
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
