#include "MemoryRegion.h"

MemoryRegionMap::MemoryRegionMap() {
    regionMap.fill(0);
    AllocateRegion(MemoryRegion::metadata, 1024);
}

void MemoryRegionMap::AllocateRegion(MemoryRegion region, uint32 size) {
    EmAssert(static_cast<uint8>(region) < regionMap.size());

    // Work around bogus GCC warning
    if (static_cast<uint8>(region) >= regionMap.size()) return;

    EmAssert(regionMap[static_cast<uint8>(region)] == 0);
    EmAssert(size % (region == MemoryRegion::metadata ? 1024 : 8192) == 0);

    regionMap[static_cast<uint8>(region)] = size;
}

uint32 MemoryRegionMap::GetRegionSize(MemoryRegion region) const {
    EmAssert(static_cast<uint8>(region) < regionMap.size());

    // Work around bogus GCC warning
    if (static_cast<uint8>(region) >= regionMap.size()) return 0;

    return regionMap[static_cast<uint8>(region)];
}

uint32 MemoryRegionMap::GetTotalSize() const {
    uint32 totalSize = 0;

    for (uint32 region = 0; region < N_MEMORY_REGIONS; region++) totalSize += regionMap[region];

    return totalSize;
}
