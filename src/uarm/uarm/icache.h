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

typedef uint32_t (*DecodeFn)(uint32_t opcode);

struct icache* icacheInit(struct ArmMem* mem, struct ArmMmu* mmu);
void icacheInval(struct icache* ic);
void icacheInvalAddr(struct icache* ic, uint32_t addr);
void icacheInvalRange(struct icache* ic, uint32_t addr, uint32_t size);

#ifdef __cplusplus
}

template <int sz, int tier = 0>
bool icacheFetch(struct icache* ic, uint32_t va, uint_fast8_t* fsr, void* buf, uint32_t* decoded);

#endif

#endif
