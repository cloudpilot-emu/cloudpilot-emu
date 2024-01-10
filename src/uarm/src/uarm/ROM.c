//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "ROM.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "uarm_endian.h"
#include "util.h"

struct ArmRomPiece {
    struct ArmRomPiece *next;
    struct ArmRom *rom;
    uint32_t base, size;
    uint32_t *buf;
};

struct ArmRom {
    uint32_t start, opAddr;
    struct ArmRomPiece *pieces;
    enum RomChipType chipType;
};

bool romAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write, void *bufP) {
    if (write) return false;

    struct ArmRomPiece *piece = (struct ArmRomPiece *)userData;
    uint8_t *addr = (uint8_t *)piece->buf + (pa - piece->base);

    switch (size) {
        case 1:

            *(uint8_t *)bufP = *((uint8_t *)addr);
            break;

        case 2:

            *(uint16_t *)bufP = le16toh(*((uint16_t *)addr));
            break;

        case 4:

            *(uint32_t *)bufP = le32toh(*((uint32_t *)addr));
            break;

        case 64:
            ((uint64_t *)bufP)[4] = le64toh(*((uint64_t *)(addr + 32)));
            ((uint64_t *)bufP)[5] = le64toh(*((uint64_t *)(addr + 40)));
            ((uint64_t *)bufP)[6] = le64toh(*((uint64_t *)(addr + 48)));
            ((uint64_t *)bufP)[7] = le64toh(*((uint64_t *)(addr + 56)));
            // fallthrough
        case 32:

            ((uint64_t *)bufP)[2] = le64toh(*((uint64_t *)(addr + 16)));
            ((uint64_t *)bufP)[3] = le64toh(*((uint64_t *)(addr + 24)));
            // fallthrough
        case 16:

            ((uint64_t *)bufP)[1] = le64toh(*((uint64_t *)(addr + 8)));
            // fallthrough
        case 8:
            ((uint64_t *)bufP)[0] = le64toh(*((uint64_t *)(addr + 0)));
            break;

        default:

            return false;
    }

    return true;
}

struct ArmRom *romInit(struct ArmMem *mem, uint32_t adr, void **pieces, const uint32_t *pieceSizes,
                       uint32_t numPieces, enum RomChipType chipType) {
    struct ArmRom *rom = (struct ArmRom *)malloc(sizeof(*rom));
    struct ArmRomPiece *prev = NULL, *t, *piece = NULL;
    uint32_t i;

    if (!rom) ERR("cannot alloc ROM at 0x%08x", adr);

    memset(rom, 0, sizeof(*rom));

    if (numPieces > 1 && chipType != RomWriteIgnore && chipType != RomWriteError)
        ERR("piecewise roms cannot be writeable\n");

    rom->start = adr;

    for (i = 0; i < numPieces; i++) {
        piece = (struct ArmRomPiece *)malloc(sizeof(*piece));
        if (!piece) ERR("cannot alloc ROM piece at 0x%08x", adr);

        memset(piece, 0, sizeof(*piece));
        piece->next = prev;  // we'll reverse the list later

        if (adr & 0x1f) ERR("rom piece cannot start at 0x%08x\n", adr);

        piece->base = adr;
        piece->size = *pieceSizes++;
        piece->buf = (uint32_t *)*pieces++;
        piece->rom = rom;

        adr += piece->size;

        if (!memRegionAdd(mem, piece->base, piece->size, romAccessF, piece))
            ERR("cannot add ROM piece at 0x%08x to MEM\n", adr);
    }

    // we linked the list in reverse. fix this
    while (piece) {
        t = piece->next;
        piece->next = rom->pieces;
        rom->pieces = piece;
        piece = t;
    }

    rom->chipType = chipType;

    return rom;
}
