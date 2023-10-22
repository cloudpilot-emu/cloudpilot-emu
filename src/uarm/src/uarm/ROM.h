//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _ROM_H_
#define _ROM_H_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

enum RomChipType {
    RomWriteIgnore,
    RomWriteError,
    RomStrataFlash16x,
    RomStrataflash16x2x,
};

struct ArmRom;

struct ArmRom *romInit(struct ArmMem *mem, uint32_t adr, void **pieces, const uint32_t *pieceSizes,
                       uint32_t numPieces, enum RomChipType chipType);

#ifdef __cplusplus
}
#endif

#endif
