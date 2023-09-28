//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _MEM_H_
#define _MEM_H_

#include <stdbool.h>
#include <stdint.h>

struct ArmMem;

typedef bool (*ArmMemAccessF)(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                              void* buf);

#define MEM_ACCESS_TYPE_READ 0
#define MEM_ACCESS_TYPE_WRITE 1
#define MEM_ACCCESS_FLAG_NOERROR 0x80  // for debugger use

struct ArmMem* memInit(void);
void memDeinit(struct ArmMem* mem);

bool memRegionAdd(struct ArmMem* mem, uint32_t pa, uint32_t sz, ArmMemAccessF af, void* uD);

bool memAccess(struct ArmMem* mem, uint32_t addr, uint_fast8_t size, uint_fast8_t accessType,
               void* buf);

#endif
