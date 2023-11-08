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

struct ArmMemRegion {
    uint32_t pa;
    uint32_t sz;
    ArmMemAccessF aF;
    void* uD;
};

#define MEM_ACCESS_TYPE_READ 0
#define MEM_ACCESS_TYPE_WRITE 1

struct ArmMem* memInit(void);
void memDeinit(struct ArmMem* mem);

bool memRegionAdd(struct ArmMem* mem, uint32_t pa, uint32_t sz, ArmMemAccessF af, void* uD);

bool memAccess(struct ArmMem* mem, uint32_t addr, uint_fast8_t size, uint_fast8_t accessType,
               void* buf);

struct ArmMemRegion* memRegionFind(struct ArmMem* mem, uint32_t start, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
