//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _ICACHE_H_
#define _ICACHE_H_

#include <stdbool.h>
#include <stdint.h>

#include "CPU.h"
#include "MMU.h"
#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

struct icache;

struct icache* icacheInit(struct ArmMem* mem, struct ArmMmu* mmu);
void icacheInval(struct icache* ic);
void icacheInvalAddr(struct icache* ic, uint32_t addr);

#ifdef __cplusplus
}

template <int sz>
bool icacheFetch(struct icache* ic, uint32_t va, uint_fast8_t* fsr, void* buf);

#endif

#endif
