#include "memcpy.h"

#include "MMU.h"
#include "mem.h"

namespace {
    template <int size>
    bool transfer_loop_pa(uint8_t*& host, uint32_t& armPa, uint32_t& sizeTotal, bool write,
                          struct ArmMem* mem, MemcpyResult* result) {
        while (sizeTotal >= size) {
            if (!memAccess(mem, armPa, size, write, host)) {
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

    template <int align>
    void transfer_pa(uint8_t* host, uint32_t armPa, uint32_t size, bool write, struct ArmMem* mem,
                     MemcpyResult* result) {
        switch (align) {
            case 3:
                if (!transfer_loop_pa<64>(host, armPa, size, write, mem, result)) return;
                if (!transfer_loop_pa<32>(host, armPa, size, write, mem, result)) return;
                if (!transfer_loop_pa<16>(host, armPa, size, write, mem, result)) return;
                if (!transfer_loop_pa<8>(host, armPa, size, write, mem, result)) return;

            case 2:
                if (!transfer_loop_pa<4>(host, armPa, size, write, mem, result)) return;

            case 1:
                if (!transfer_loop_pa<2>(host, armPa, size, write, mem, result)) return;

            case 0:
                if (!transfer_loop_pa<1>(host, armPa, size, write, mem, result)) return;
                break;
        }
    }

    void transfer(uint8_t* host, uint32_t arm, uint32_t size, bool write, bool privileged,
                  struct ArmMem* mem, struct ArmMmu* mmu, MemcpyResult* result) {
        result->ok = true;
        result->wasWrite = write;

        uint8_t align = __builtin_ctz(static_cast<unsigned long>(arm) |
                                      reinterpret_cast<unsigned long>(host) | 0x08);

        while (size > 0) {
            MMUTranslateResult translateResult = mmuTranslate(mmu, arm, privileged, false);
            if (!MMU_TRANSLATE_RESULT_OK(translateResult)) {
                result->ok = false;
                result->fsr = MMU_TRANSLATE_RESULT_FSR(translateResult);
                result->faultAddr = arm;

                return;
            }

            // break loop at 1024k boundaries (smallest possible page size) and
            // consult MMU for each chunk
            const uint32_t pa = MMU_TRANSLATE_RESULT_PA(translateResult);
            const uint32_t pageBoundary = pa | 0x03ff;
            const uint32_t chunkSize = pa + size > pageBoundary ? pageBoundary - pa + 1 : size;

            switch (align) {
                case 0:
                    transfer_pa<0>(host, pa, chunkSize, write, mem, result);
                    break;

                case 1:
                    transfer_pa<1>(host, pa, chunkSize, write, mem, result);
                    break;

                case 2:
                    transfer_pa<2>(host, pa, chunkSize, write, mem, result);
                    break;

                case 3:
                    transfer_pa<3>(host, pa, chunkSize, write, mem, result);
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

}  // namespace

void memcpy_armToHost(uint8_t* dest, uint32_t src, uint32_t size, bool privileged,
                      struct ArmMem* mem, struct ArmMmu* mmu, MemcpyResult* result) {
    transfer(dest, src, size, false, privileged, mem, mmu, result);
}

void memcpy_hostToArm(uint32_t dest, uint8_t* src, uint32_t size, bool privileged,
                      struct ArmMem* mem, struct ArmMmu* mmu, MemcpyResult* result) {
    transfer(src, dest, size, true, privileged, mem, mmu, result);
}

void memcpy_armToArm(uint32_t dest, uint32_t src, uint32_t size, bool privileged,
                     struct ArmMem* mem, struct ArmMmu* mmu, struct MemcpyResult* result) {
    static uint64_t scratch[512];

    result->ok = true;

    while (size > 0 && result->ok) {
        uint32_t chunkSize = size > sizeof(scratch) ? sizeof(scratch) : size;

        memcpy_armToHost(reinterpret_cast<uint8_t*>(scratch), src, chunkSize, privileged, mem, mmu,
                         result);

        if (result->ok)
            memcpy_hostToArm(dest, reinterpret_cast<uint8_t*>(scratch), chunkSize, privileged, mem,
                             mmu, result);

        size -= chunkSize;
        src += chunkSize;
        dest += chunkSize;
    }
}
