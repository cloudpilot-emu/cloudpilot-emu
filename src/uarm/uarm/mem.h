//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _MEM_H_
#define _MEM_H_

#include <cstdint>

struct ArmMem;

typedef bool (*ArmMemAccessF)(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                              void* buf);

struct ArmMem* memInit(void);
void memDeinit(struct ArmMem* mem);

bool memRegionAdd(struct ArmMem* mem, uint32_t pa, uint32_t sz, ArmMemAccessF af, void* uD);
bool memRegionAddRam(struct ArmMem* mem, uint32_t pa, uint32_t sz, ArmMemAccessF af, void* uD);
bool memRegionAddRom(struct ArmMem* mem, uint32_t pa, uint32_t sz, ArmMemAccessF af, void* uD);

template <int size, bool write>
bool memAccess(struct ArmMem* mem, uint32_t addr, void* buf);

bool memAccess(struct ArmMem* mem, uint32_t addr, uint_fast8_t size, bool write, void* buf);

template <int size>
bool memInstructionFetch(struct ArmMem* mem, uint32_t addr, void* buf);

#endif
