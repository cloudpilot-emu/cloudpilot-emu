//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _MEM_H_
#define _MEM_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ArmMem;

typedef bool (*ArmMemAccessF)(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                              void* buf);

struct ArmMem* memInit(void);
void memDeinit(struct ArmMem* mem);

bool memRegionAdd(struct ArmMem* mem, uint32_t pa, uint32_t sz, ArmMemAccessF af, void* uD);

bool memRegionAdd2(struct ArmMem* mem, uint32_t pa, uint32_t sz, ArmMemAccessF af,
                   ArmMemAccessF afCode, void* uD);

bool memAccess(struct ArmMem* mem, uint32_t addr, uint_fast8_t size, bool write, void* buf);

bool memAccessCode(struct ArmMem* mem, uint32_t addr, uint_fast8_t size, bool write, void* buf);

#ifdef __cplusplus
}
#endif

#endif
