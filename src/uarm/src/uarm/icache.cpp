//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "icache.h"

#include <stdlib.h>
#include <string.h>

#include "../util.h"
#include "CPU.h"
#include "uarm_endian.h"

#define CACHE_LINE_WIDTH_BITS 5
#define CACHE_INDEX_BITS 20

#define calculateIndex(va) ((va >> CACHE_LINE_WIDTH_BITS) & ~(0xffffffff << CACHE_INDEX_BITS))
#define calculateTag(va) (va >> (CACHE_LINE_WIDTH_BITS + CACHE_INDEX_BITS))
#define calculateLineIndex(va) (va & ~(0xffffffff << CACHE_LINE_WIDTH_BITS))

struct icacheline {
    uint8_t data[1 << CACHE_LINE_WIDTH_BITS];
    uint32_t decoded[1 << (CACHE_LINE_WIDTH_BITS - 1)];

    uint_fast8_t tag;
    uint32_t revision;
} __attribute__((aligned(8)));

struct icache {
    struct ArmMem* mem;
    struct ArmMmu* mmu;

    uint32_t revision;
    struct icacheline cache[1 << CACHE_INDEX_BITS];
};

void icacheInval(struct icache* ic) {
    ic->revision++;

    if (ic->revision == 0) {
        for (size_t i = 0; i < (1 << CACHE_INDEX_BITS); i++) ic->cache[i].revision = 0;
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
    struct icacheline* line = ic->cache + calculateIndex(va);

    if (line->revision != ic->revision || line->tag != calculateTag(va)) return;

    line->revision = ic->revision - 1;
}

template <int sz>
bool icacheFetch(struct icache* ic, DecodeFn decode, uint32_t va, uint_fast8_t* fsrP, void* buf,
                 uint32_t* decoded) {
    if (va & (sz - 1)) {  // alignment issue

        *fsrP = 3;
        return false;
    }

    struct icacheline* line = ic->cache + calculateIndex(va);
    const uint_fast8_t tag = calculateTag(va);

    if (line->revision != ic->revision || line->tag != tag) {
        uint8_t data[sizeof(line->data)];

        MMUTranslateResult translateResult = mmuTranslate(ic->mmu, va, true, false);
        if (!MMU_TRANSLATE_RESULT_OK(translateResult)) {
            *fsrP = MMU_TRANSLATE_RESULT_FSR(translateResult);
            return false;
        }

        uint32_t pa = MMU_TRANSLATE_RESULT_PA(translateResult);
        if (!MMU_TRANSLATE_RESULT_CACHEABLE(translateResult)) {
            bool ok = memAccess(ic->mem, pa, sz, false, buf);

            if (!ok) {
                *fsrP = 0x0d;  // perm error
                return false;
            }

            *decoded = decode(sz == 4 ? *(uint32_t*)buf : *(uint16_t*)buf);
            return true;
        }

        bool ok = memAccess(ic->mem, pa & ((0xffffffff << CACHE_LINE_WIDTH_BITS)), sizeof(data),
                            false, data);
        if (!ok) {
            if (fsrP) *fsrP = 0x0d;  // perm error
            return false;
        };

        for (size_t i = 0; i < sizeof(data); i += 8) {
            const uint64_t d = *(uint64_t*)(data + i);
            if ((uint32_t)d != *(uint32_t*)(line->data + i))
                line->decoded[i >> 1] = line->decoded[(i >> 1) + 1] = 0;

            if ((d >> 32) != *(uint32_t*)(line->data + i + 4))
                line->decoded[(i >> 1) + 2] = line->decoded[(i >> 1) + 3] = 0;

            *(uint64_t*)(line->data + i) = d;
        }

        line->revision = ic->revision;
        line->tag = tag;
    }

    switch (sz) {
        case 4: {
            const size_t i = calculateLineIndex(va);
            const uint32_t inst = *(uint32_t*)(line->data + i);
            *(uint32_t*)buf = inst;

            const size_t iInst = i >> 1;
            if ((line->decoded[iInst] & 0x03) != 0x02) {
                // fprintf(stderr, "decode cache miss ARM\n");
                *decoded = decode(inst);
                line->decoded[iInst] = (*decoded << 2) | 0x02;
            } else {
#ifdef __EMSCRIPTEN__
                *decoded = line->decoded[iInst] >> 2;
#else
                *decoded = ((int32_t)line->decoded[iInst]) >> 2;
#endif
            }

            break;
        }

        case 2: {
            const size_t i = calculateLineIndex(va);
            const uint16_t inst = *(uint16_t*)(line->data + i);
            *(uint16_t*)buf = inst;

            const size_t iInst = i >> 1;
            if ((line->decoded[iInst] & 0x03) != 0x03) {
                // fprintf(stderr, "decode cache miss thumb\n");
                *decoded = decode(inst);
                line->decoded[iInst] = (*decoded << 2) | 0x03;
            } else {
#ifdef __EMSCRIPTEN__
                *decoded = line->decoded[iInst] >> 2;
#else
                *decoded = ((int32_t)line->decoded[iInst]) >> 2;
#endif
            }

            break;
        }

        default:
            __builtin_unreachable();
    }

    return true;
}

template bool icacheFetch<2>(struct icache* ic, DecodeFn decode, uint32_t va, uint_fast8_t* fsrP,
                             void* buf, uint32_t* decoded);
template bool icacheFetch<4>(struct icache* ic, DecodeFn decode, uint32_t va, uint_fast8_t* fsrP,
                             void* buf, uint32_t* decoded);
