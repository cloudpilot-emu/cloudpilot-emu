#ifndef _MEMCPY_H_
#define _MEMCPY_H_

#include <cstdint>

struct MemcpyResult {
    bool ok;
    uint8_t fsr;
    uint32_t faultAddr;
    bool wasWrite;
};

template <typename T>
void memcpy_armToHost(uint8_t* dest, uint32_t src, uint32_t size, bool privileged,
                      struct ArmMem* mem, T* msys, struct MemcpyResult* result);

template <typename T>
void memcpy_hostToArm(uint32_t dest, const uint8_t* src, uint32_t size, bool privileged,
                      struct ArmMem* mem, T* msys, struct MemcpyResult* result);

template <typename T>
void memcpy_armToArm(uint32_t dest, uint32_t src, uint32_t size, bool privileged,
                     struct ArmMem* mem, T* msys, struct MemcpyResult* result);

#endif  // _MEMCPY_H_
