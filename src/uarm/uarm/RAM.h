//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _RAM_H_
#define _RAM_H_

#include <cstdint>

#include "mem.h"

struct ArmRam;
class SoC;
struct MemoryBuffer;

struct ArmRam* ramInit(struct ArmMem* mem, class SoC* soc, uint32_t adr, uint32_t sz,
                       const struct MemoryBuffer* buf, bool primary);

template <int size, bool write>
bool ramAccessF(void* userData, uint32_t pa, void* bufP);

bool ramAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write, void* bufP);

void ramSetFramebuffer(struct ArmRam* ram, uint32_t base, uint32_t size);

#endif
