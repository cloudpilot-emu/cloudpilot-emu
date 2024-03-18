//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "ROM.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "uarm_endian.h"
#include "util.h"

struct ArmRom {
    uint32_t base, size;
    uint32_t *data;
    uint32_t *dataPeephole;
};

static inline bool access(uint8_t *source, uint_fast8_t size, void *bufP) {
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

bool romAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write, void *bufP) {
    if (write) return false;

    struct ArmRom *rom = (struct ArmRom *)userData;

    return access((uint8_t *)rom->data + (pa - rom->base), size, bufP);
}

bool romInstructionFetch(void *userData, uint32_t pa, uint_fast8_t size, void *bufP) {
    struct ArmRom *rom = (struct ArmRom *)userData;

    return access((uint8_t *)rom->dataPeephole + (pa - rom->base), size, bufP);
}

struct ArmRom *romInit(struct ArmMem *mem, uint32_t adr, void *data, const uint32_t size) {
    struct ArmRom *rom = (struct ArmRom *)malloc(sizeof(*rom));
    if (!rom) ERR("cannot alloc ROM at 0x%08x", adr);

    memset(rom, 0, sizeof(*rom));

    rom->base = adr;
    rom->data = (uint32_t *)data;
    rom->size = size;

    rom->dataPeephole = malloc(size);
    if (!rom->dataPeephole) ERR("failed to allocate shadow buffer for peephole optimizations");

    memcpy(rom->dataPeephole, rom->data, rom->size);

    if (!memRegionAddRom(mem, adr, size, romAccessF, rom))
        ERR("cannot add RAM at 0x%08x to MEM\n", adr);

    return rom;
}

void *romGetPeepholeBuffer(struct ArmRom *rom) { return rom->dataPeephole; }