#include "memcpy.h"

#include "CPU.h"
#include "MMU.h"
#include "MPU.h"
#include "mem.h"

namespace {
    template <int size, bool write>
    bool transfer_loop_pa(uint8_t*& host, uint32_t& armPa, uint32_t& sizeTotal, struct ArmMem* mem,
                          MemcpyResult* result) {
        while (sizeTotal >= size) {
            if (!memAccess<size, write>(mem, armPa, host)) {
                result->ok = false;
                result->faultAddr = armPa;

                return false;
            }

            host += size;
            armPa += size;
            sizeTotal -= size;
        }

        return true;
    }

    template <int align, bool write>
    void transfer_pa(uint8_t* host, uint32_t armPa, uint32_t size, struct ArmMem* mem,
                     MemcpyResult* result) {
        switch (align) {
            case 3:
                if (!transfer_loop_pa<64, write>(host, armPa, size, mem, result)) return;
                if (!transfer_loop_pa<32, write>(host, armPa, size, mem, result)) return;
                if (!transfer_loop_pa<16, write>(host, armPa, size, mem, result)) return;
                if (!transfer_loop_pa<8, write>(host, armPa, size, mem, result)) return;
                [[fallthrough]];

            case 2:
                if (!transfer_loop_pa<4, write>(host, armPa, size, mem, result)) return;
                [[fallthrough]];

            case 1:
                if (!transfer_loop_pa<2, write>(host, armPa, size, mem, result)) return;
                [[fallthrough]];

            case 0:
                if (!transfer_loop_pa<1, write>(host, armPa, size, mem, result)) return;
                break;
        }
    }

    template <bool write>
    void transfer(uint8_t* host, uint32_t arm, uint32_t size, bool privileged, struct ArmMem* mem,
                  struct ArmMmu* mmu, MemcpyResult* result) {
        result->ok = true;
        result->wasWrite = write;

        uint8_t align = __builtin_ctz(static_cast<unsigned long>(arm) |
                                      reinterpret_cast<unsigned long>(host) | 0x08);

        while (size > 0) {
            MMUTranslateResult translateResult = mmuTranslate(mmu, arm, privileged, write);
            if (!MMU_TRANSLATE_RESULT_OK(translateResult)) {
                result->ok = false;
                result->fsr = MMU_TRANSLATE_RESULT_FSR(translateResult);
                result->faultAddr = arm;

                return;
            }

            // break loop at 1k boundaries (smallest possible page size) and
            // consult MMU for each chunk
            const uint32_t pa = MMU_TRANSLATE_RESULT_PA(translateResult);
            const uint32_t pageBoundary = pa | 0x03ff;
            const uint32_t chunkSize = pa + size > pageBoundary ? pageBoundary - pa + 1 : size;

            switch (align) {
                case 0:
                    transfer_pa<0, write>(host, pa, chunkSize, mem, result);
                    break;

                case 1:
                    transfer_pa<1, write>(host, pa, chunkSize, mem, result);
                    break;

                case 2:
                    transfer_pa<2, write>(host, pa, chunkSize, mem, result);
                    break;

                case 3:
                    transfer_pa<3, write>(host, pa, chunkSize, mem, result);
                    break;
            }

            if (!result->ok) {
                result->faultAddr += (arm - pa);
                result->fsr = 10;
                return;
            }

            host += chunkSize;
            arm += chunkSize;
            size -= chunkSize;
        }
    }

    template <bool write>
    void transfer(uint8_t* host, uint32_t arm, uint32_t size, bool privileged, struct ArmMem* mem,
                  struct ArmMpu* mpu, MemcpyResult* result) {
        result->ok = true;
        result->wasWrite = write;

        uint8_t align = __builtin_ctz(static_cast<unsigned long>(arm) |
                                      reinterpret_cast<unsigned long>(host) | 0x08);

        while (size > 0) {
            if (!MPU_TEST_RESULT_OK(mpuTestAddress(mpu, arm, privileged, write))) {
                result->ok = false;
                result->fsr = 1;
                result->faultAddr = arm;

                return;
            }

            // break loop at 4k boundaries (smallest possible page size for MPU) and
            // consult MMU for each chunk
            const uint32_t pageBoundary = arm | 0x0fff;
            const uint32_t chunkSize = arm + size > pageBoundary ? pageBoundary - arm + 1 : size;

            switch (align) {
                case 0:
                    transfer_pa<0, write>(host, arm, chunkSize, mem, result);
                    break;

                case 1:
                    transfer_pa<1, write>(host, arm, chunkSize, mem, result);
                    break;

                case 2:
                    transfer_pa<2, write>(host, arm, chunkSize, mem, result);
                    break;

                case 3:
                    transfer_pa<3, write>(host, arm, chunkSize, mem, result);
                    break;
            }

            if (!result->ok) {
                result->fsr = 1;
                return;
            }

            host += chunkSize;
            arm += chunkSize;
            size -= chunkSize;
        }
    }

}  // namespace

template <typename T>
void memcpy_armToHost(uint8_t* dest, uint32_t src, uint32_t size, bool privileged,
                      struct ArmMem* mem, T* msys, MemcpyResult* result) {
    transfer<false>(dest, src, size, privileged, mem, msys, result);
}

void memcpy_armToHost(uint8_t* dest, uint32_t src, uint32_t size, bool privileged,
                      struct ArmCpu* cpu, struct MemcpyResult* result) {
    if (cpuGetMemorySystemKind(cpu) == ARM_MEMORY_SYSTEM_MMU) {
        memcpy_armToHost(dest, src, size, privileged, cpuGetMem(cpu), cpuGetMMU(cpu), result);
    } else {
        memcpy_armToHost(dest, src, size, privileged, cpuGetMem(cpu), cpuGetMPU(cpu), result);
    }
}

template <typename T>
void memcpy_hostToArm(uint32_t dest, const uint8_t* src, uint32_t size, bool privileged,
                      struct ArmMem* mem, T* msys, MemcpyResult* result) {
    transfer<true>(const_cast<uint8_t*>(src), dest, size, privileged, mem, msys, result);
}

void memcpy_hostToArm(uint32_t dest, const uint8_t* src, uint32_t size, bool privileged,
                      struct ArmCpu* cpu, struct MemcpyResult* result) {
    if (cpuGetMemorySystemKind(cpu) == ARM_MEMORY_SYSTEM_MMU) {
        memcpy_hostToArm(dest, src, size, privileged, cpuGetMem(cpu), cpuGetMMU(cpu), result);
    } else {
        memcpy_hostToArm(dest, src, size, privileged, cpuGetMem(cpu), cpuGetMPU(cpu), result);
    }
}

template <typename T>
void memcpy_armToArm(uint32_t dest, uint32_t src, uint32_t size, bool privileged,
                     struct ArmMem* mem, T* msys, struct MemcpyResult* result) {
    static uint64_t scratch[512];

    result->ok = true;

    while (size > 0 && result->ok) {
        uint32_t chunkSize = size > sizeof(scratch) ? sizeof(scratch) : size;

        memcpy_armToHost(reinterpret_cast<uint8_t*>(scratch), src, chunkSize, privileged, mem, msys,
                         result);

        if (result->ok)
            memcpy_hostToArm(dest, reinterpret_cast<uint8_t*>(scratch), chunkSize, privileged, mem,
                             msys, result);

        size -= chunkSize;
        src += chunkSize;
        dest += chunkSize;
    }
}

void memcpy_armToArm(uint32_t dest, uint32_t src, uint32_t size, bool privileged,
                     struct ArmCpu* cpu, struct MemcpyResult* result) {
    if (cpuGetMemorySystemKind(cpu) == ARM_MEMORY_SYSTEM_MMU) {
        memcpy_armToArm(dest, src, size, privileged, cpuGetMem(cpu), cpuGetMMU(cpu), result);
    } else {
        memcpy_armToArm(dest, src, size, privileged, cpuGetMem(cpu), cpuGetMPU(cpu), result);
    }
}

template void memcpy_armToHost<ArmMmu>(uint8_t* dest, uint32_t src, uint32_t size, bool privileged,
                                       struct ArmMem* mem, ArmMmu* msys, MemcpyResult* result);
template void memcpy_armToHost<ArmMpu>(uint8_t* dest, uint32_t src, uint32_t size, bool privileged,
                                       struct ArmMem* mem, ArmMpu* msys, MemcpyResult* result);

template void memcpy_hostToArm<ArmMmu>(uint32_t dest, const uint8_t* src, uint32_t size,
                                       bool privileged, struct ArmMem* mem, ArmMmu* msys,
                                       MemcpyResult* result);
template void memcpy_hostToArm<ArmMpu>(uint32_t dest, const uint8_t* src, uint32_t size,
                                       bool privileged, struct ArmMem* mem, ArmMpu* msys,
                                       MemcpyResult* result);

template void memcpy_armToArm<ArmMmu>(uint32_t dest, uint32_t src, uint32_t size, bool privileged,
                                      struct ArmMem* mem, struct ArmMmu* msys,
                                      struct MemcpyResult* result);
template void memcpy_armToArm<ArmMpu>(uint32_t dest, uint32_t src, uint32_t size, bool privileged,
                                      struct ArmMem* mem, struct ArmMpu* msys,
                                      struct MemcpyResult* result);
