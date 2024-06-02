//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "icache.h"

#include <stdlib.h>
#include <string.h>

#include "../util.h"
#include "CPU.h"
#include "uarm_endian.h"

#define CACHE_LINE_WIDTH_BITS 5
#define CACHE_INDEX_BITS 17

#if (32 - CACHE_LINE_WIDTH_BITS - CACHE_INDEX_BITS <= 8)
    #define CACHE_TAG_TYPE uint_fast8_t
#else
    #define CACHE_TAG_TYPE uint_fast16_t
#endif

#define calculateIndex(va) ((va >> CACHE_LINE_WIDTH_BITS) & ~(0xffffffff << CACHE_INDEX_BITS))
#define calculateTag(va) (va >> (CACHE_LINE_WIDTH_BITS + CACHE_INDEX_BITS))
#define calculateLineIndex(va) (va & ~(0xffffffff << CACHE_LINE_WIDTH_BITS))
#define maskLine(va) (va & (0xffffffff << CACHE_LINE_WIDTH_BITS))

#ifdef __EMSCRIPTEN__
    #define DECODED_INSTRUCTION_TYPE uint16_t
    #define DECODED_BITS 0xc000
    #define DECODED_BITS_ARM 0x4000
    #define DECODED_BITS_THUMB 0xc000
    #define DECODED_BITS_SHIFT 0
#else
    #define DECODED_INSTRUCTION_TYPE uint32_t
    #define DECODED_BITS 0x03
    #define DECODED_BITS_ARM 0x02
    #define DECODED_BITS_THUMB 0x03
    #define DECODED_BITS_SHIFT 2
#endif

struct icacheline {
    uint8_t data[1 << CACHE_LINE_WIDTH_BITS];
    DECODED_INSTRUCTION_TYPE decoded[1 << (CACHE_LINE_WIDTH_BITS - 1)];

    CACHE_TAG_TYPE tag;
    uint32_t revision;
} __attribute__((aligned(64)));

struct icache {
    struct ArmMem* mem;
    struct ArmMmu* mmu;

    uint32_t revision;
    struct icacheline cache[1 << CACHE_INDEX_BITS];
};

void icacheInval(struct icache* ic) {
    ic->revision++;

    if (ic->revision == 0) {
        ic->revision = 1;
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

void icacheInvalRange(struct icache* ic, uint32_t addr, uint32_t size) {
    for (uint32_t line = maskLine(addr); line < addr + size; line += (1 << CACHE_LINE_WIDTH_BITS)) {
        icacheInvalAddr(ic, line);
    }
}

template <int sz>
bool icacheFetch(struct icache* ic, uint32_t va, uint_fast8_t* fsrP, void* buf, uint32_t* decoded) {
    if (va & (sz - 1)) {  // alignment issue

        *fsrP = 3;
        return false;
    }

    struct icacheline* line = ic->cache + calculateIndex(va);
    const uint_fast8_t tag = calculateTag(va);

    if (line->revision != ic->revision || line->tag != tag) {
        uint8_t data[sizeof(line->data)];
        bool cacheable = mmuIsOn(ic->mmu);
        uint32_t pa = va;

        if (cacheable) {
            MMUTranslateResult translateResult = mmuTranslate(ic->mmu, va, true, false);
            if (!MMU_TRANSLATE_RESULT_OK(translateResult)) {
                *fsrP = MMU_TRANSLATE_RESULT_FSR(translateResult);
                return false;
            }

            pa = MMU_TRANSLATE_RESULT_PA(translateResult);
            cacheable = MMU_TRANSLATE_RESULT_CACHEABLE(translateResult);
        }

        if (!cacheable) {
            bool ok = memInstructionFetch(ic->mem, pa, sz, buf);

            if (!ok) {
                *fsrP = 0x0d;  // perm error
                return false;
            }

            *decoded = sz == 4 ? cpuDecodeArm(*(uint32_t*)buf) : cpuDecodeThumb(*(uint16_t*)buf);
            return true;
        }

        bool ok = memInstructionFetch(ic->mem, pa & ((0xffffffff << CACHE_LINE_WIDTH_BITS)),
                                      sizeof(data), data);
        if (!ok) {
            if (fsrP) *fsrP = 0x0d;  // perm error
            return false;
        };

        for (size_t i = 0; i < sizeof(data); i += 8) {
            const uint64_t d = *(uint64_t*)(data + i);
            if ((uint32_t)d != *(uint32_t*)(line->data + i))
#ifdef __EMSCRIPTEN__
                *((uint32_t*)(line->decoded + (i >> 1))) = 0;
#else
                *((uint64_t*)(line->decoded + (i >> 1))) = 0;
#endif

            if ((d >> 32) != *(uint32_t*)(line->data + i + 4))
#ifdef __EMSCRIPTEN__
                *((uint32_t*)(line->decoded + (i >> 1) + 2)) = 0;
#else
                *((uint64_t*)(line->decoded + (i >> 1) + 2)) = 0;
#endif

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
            if ((line->decoded[iInst] & DECODED_BITS) != DECODED_BITS_ARM) {
                // fprintf(stderr, "decode cache miss ARM\n");
                *decoded = cpuDecodeArm(inst);
                line->decoded[iInst] = (*decoded << DECODED_BITS_SHIFT) | DECODED_BITS_ARM;
            } else {
#ifdef __EMSCRIPTEN__
                *decoded = line->decoded[iInst] & ~DECODED_BITS;
#else
                *decoded = ((int32_t)line->decoded[iInst]) >> DECODED_BITS_SHIFT;
#endif
            }

            break;
        }

        case 2: {
            const size_t i = calculateLineIndex(va);
            const uint16_t inst = *(uint16_t*)(line->data + i);
            *(uint16_t*)buf = inst;

            const size_t iInst = i >> 1;
            if ((line->decoded[iInst] & DECODED_BITS) != DECODED_BITS_THUMB) {
                // fprintf(stderr, "decode cache miss thumb\n");
                *decoded = cpuDecodeThumb(inst);
                line->decoded[iInst] = (*decoded << DECODED_BITS_SHIFT) | DECODED_BITS_THUMB;
            } else {
#ifdef __EMSCRIPTEN__
                *decoded = line->decoded[iInst] & ~DECODED_BITS;
#else
                *decoded = ((int32_t)line->decoded[iInst]) >> DECODED_BITS_SHIFT;
#endif
            }

            break;
        }

        default:
            __builtin_unreachable();
    }

    return true;
}

template bool icacheFetch<2>(struct icache* ic, uint32_t va, uint_fast8_t* fsrP, void* buf,
                             uint32_t* decoded);
template bool icacheFetch<4>(struct icache* ic, uint32_t va, uint_fast8_t* fsrP, void* buf,
                             uint32_t* decoded);
