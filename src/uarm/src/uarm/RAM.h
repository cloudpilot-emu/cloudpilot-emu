//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _RAM_H_
#define _RAM_H_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ArmRam;

struct ArmRam* ramInit(struct ArmMem* mem, uint32_t adr, uint32_t sz, uint32_t* buf, bool primary);

bool ramAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write, void* bufP);

#ifdef __cplusplus
}
#endif

#endif
