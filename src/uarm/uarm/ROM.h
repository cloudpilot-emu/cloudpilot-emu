//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _ROM_H_
#define _ROM_H_

#include <cstdint>

#include "mem.h"

struct ArmRom;

struct ArmRom *romInit(struct ArmMem *mem, uint32_t adr, void *data, const uint32_t size);

void *romGetPeepholeBuffer(struct ArmRom *rom);

template <int size, bool write>
bool romAccessF(void *userData, uint32_t pa, void *bufP);

bool romAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write, void *bufP);

template <int size>
bool romInstructionFetch(void *userData, uint32_t pa, void *bufP);

uint32_t romGetSize(struct ArmRom *rom);

void *romGetData(struct ArmRom *rom);

void *romResolveAddress(struct ArmRom *rom, uint32_t pa, uint32_t size);

#endif
