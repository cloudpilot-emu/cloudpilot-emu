#ifndef _MEMORY_REGION_H_
#define _MEMORY_REGION_H_

#include <array>

#include "EmCommon.h"

constexpr size_t N_MEMORY_REGIONS = 6;

enum class MemoryRegion : uint8 {
    metadata = 0,
    ram = 1,
    framebuffer = 2,
    memorystick = 3,
    sonyDsp = 4,
    eSRAM = 5
};

class MemoryRegionMap {
   public:
    MemoryRegionMap();

    void AllocateRegion(MemoryRegion region, uint32 size);
    uint32 GetRegionSize(MemoryRegion region) const;

    uint32 GetTotalSize() const;

   private:
    array<uint32, N_MEMORY_REGIONS> regionMap;
};

#endif  // _MEMORY_REGION_H_
