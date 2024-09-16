//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _RAM_H_
#define _RAM_H_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"
#include "ram_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ArmRam;
struct SoC;

struct ArmRam* ramInit(struct ArmMem* mem, struct SoC* soc, uint32_t adr, uint32_t sz,
                       struct RamBuffer* buf, bool primary);

bool ramAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write, void* bufP);

void ramSetFramebuffer(struct ArmRam* ram, uint32_t base, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
