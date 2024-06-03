#ifndef _MEMCPY_H_
#define _MEMCPY_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ArmMmu;
struct ArmMem;

struct MemcpyResult {
    bool ok;
    uint8_t fsr;
    uint32_t faultAddr;
    bool wasWrite;
};

void memcpy_armToHost(uint8_t* dest, uint32_t src, uint32_t size, bool privileged,
                      struct ArmMem* mem, struct ArmMmu* mmu, struct MemcpyResult* result);

void memcpy_hostToArm(uint32_t dest, uint8_t* src, uint32_t size, bool privileged,
                      struct ArmMem* mem, struct ArmMmu* mmu, struct MemcpyResult* result);

void memcpy_armToArm(uint32_t dest, uint32_t src, uint32_t size, bool privileged,
                     struct ArmMem* mem, struct ArmMmu* mmu, struct MemcpyResult* result);

#ifdef __cplusplus
}
#endif

#endif  // _MEMCPY_H_