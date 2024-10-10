//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _ROM_H_
#define _ROM_H_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ArmRom;

struct ArmRom *romInit(struct ArmMem *mem, uint32_t adr, void *data, const uint32_t size);

void *romGetPeepholeBuffer(struct ArmRom *rom);

bool romAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write, void *bufP);

bool romInstructionFetch(void *userData, uint32_t pa, uint_fast8_t size, void *bufP);

#ifdef __cplusplus
}
#endif

#endif
