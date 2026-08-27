//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "ROM.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "CPEndian.h"
#include "cputil.h"
#include "mem.h"

struct ArmRom {
    uint32_t base, size;
    uint32_t *data;
    uint32_t *dataPeephole;
};

template <int size>
static bool access(uint8_t *source, void *bufP) {
    switch (size) {
        case 1:

            *(uint8_t *)bufP = *((uint8_t *)source);
            break;

        case 2:

            *(uint16_t *)bufP = le16toh(*((uint16_t *)source));
            break;

        case 4:

            *(uint32_t *)bufP = le32toh(*((uint32_t *)source));
            break;

        case 64:
            ((uint64_t *)bufP)[4] = le64toh(*((uint64_t *)(source + 32)));
            ((uint64_t *)bufP)[5] = le64toh(*((uint64_t *)(source + 40)));
            ((uint64_t *)bufP)[6] = le64toh(*((uint64_t *)(source + 48)));
            ((uint64_t *)bufP)[7] = le64toh(*((uint64_t *)(source + 56)));
            // fallthrough
        case 32:

            ((uint64_t *)bufP)[2] = le64toh(*((uint64_t *)(source + 16)));
            ((uint64_t *)bufP)[3] = le64toh(*((uint64_t *)(source + 24)));
            // fallthrough
        case 16:

            ((uint64_t *)bufP)[1] = le64toh(*((uint64_t *)(source + 8)));
            // fallthrough
        case 8:
            ((uint64_t *)bufP)[0] = le64toh(*((uint64_t *)(source + 0)));
            break;

        default:

            return false;
    }

    return true;
}

template <int size, bool write>
bool romAccessF(void *userData, uint32_t pa, void *bufP) {
    if constexpr (write) return false;

    struct ArmRom *rom = (struct ArmRom *)userData;

    return access<size>((uint8_t *)rom->data + (pa - rom->base), bufP);
}

#define DECLARE_ACCESS_F_SIZE(sz)                                                \
    template bool romAccessF<sz, true>(void *userData, uint32_t pa, void *bufP); \
    template bool romAccessF<sz, false>(void *userData, uint32_t pa, void *bufP);

DECLARE_ACCESS_F_SIZE(1)
DECLARE_ACCESS_F_SIZE(2)
DECLARE_ACCESS_F_SIZE(4)
DECLARE_ACCESS_F_SIZE(8)
DECLARE_ACCESS_F_SIZE(16)
DECLARE_ACCESS_F_SIZE(32)
DECLARE_ACCESS_F_SIZE(64)

bool romAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write, void *bufP) {
    if (write) return false;

    switch (size) {
        case 1:
            return romAccessF<1, false>(userData, pa, bufP);

        case 2:
            return romAccessF<2, false>(userData, pa, bufP);

        case 4:
            return romAccessF<4, false>(userData, pa, bufP);

        case 8:
            return romAccessF<8, false>(userData, pa, bufP);

        case 16:
            return romAccessF<16, false>(userData, pa, bufP);

        case 32:
            return romAccessF<32, false>(userData, pa, bufP);

        case 64:
            return romAccessF<64, false>(userData, pa, bufP);

        default:
            return false;
    }
}

template <int size>
bool romInstructionFetch(void *userData, uint32_t pa, void *bufP) {
    struct ArmRom *rom = (struct ArmRom *)userData;

    return access<size>((uint8_t *)rom->dataPeephole + (pa - rom->base), bufP);
}

template bool romInstructionFetch<1>(void *userData, uint32_t pa, void *bufP);
template bool romInstructionFetch<2>(void *userData, uint32_t pa, void *bufP);
template bool romInstructionFetch<4>(void *userData, uint32_t pa, void *bufP);
template bool romInstructionFetch<8>(void *userData, uint32_t pa, void *bufP);
template bool romInstructionFetch<16>(void *userData, uint32_t pa, void *bufP);
template bool romInstructionFetch<32>(void *userData, uint32_t pa, void *bufP);
template bool romInstructionFetch<64>(void *userData, uint32_t pa, void *bufP);

uint32_t romGetSize(struct ArmRom *rom) { return rom->size; }

void *romGetData(struct ArmRom *rom) { return rom->data; }

void *romResolveAddress(struct ArmRom *rom, uint32_t pa, uint32_t size) {
    if (pa < rom->base || pa + size > rom->base + rom->size) return nullptr;

    return reinterpret_cast<uint8_t *>(rom->data) + (pa - rom->base);
}

struct ArmRom *romInit(struct ArmMem *mem, uint32_t adr, void *data, const uint32_t size) {
    struct ArmRom *rom = (struct ArmRom *)malloc(sizeof(*rom));
    if (!rom) ERR("cannot alloc ROM at 0x%08x", adr);

    memset(rom, 0, sizeof(*rom));

    rom->base = adr;
    rom->data = (uint32_t *)data;
    rom->size = size;

    rom->dataPeephole = reinterpret_cast<uint32_t *>(malloc(size));
    if (!rom->dataPeephole) ERR("failed to allocate shadow buffer for peephole optimizations");

    memcpy(rom->dataPeephole, rom->data, rom->size);

    if (!memRegionAddRom(mem, adr, size, romAccessF, rom))
        ERR("cannot add RAM at 0x%08x to MEM\n", adr);

    return rom;
}

void *romGetPeepholeBuffer(struct ArmRom *rom) { return rom->dataPeephole; }
