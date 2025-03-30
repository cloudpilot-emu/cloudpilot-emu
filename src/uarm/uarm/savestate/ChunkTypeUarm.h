#ifndef _CHUNK_TYPE_H_
#define _CHUNK_TYPE_H_

#include <cstdint>

enum class ChunkType : uint32_t {
    pxaSoc = 0x0010,
    pxaUart = 0x0030,
    pxaSsp = 0x0040,
    pxaGpio = 0x0050,

    scheduler = 0x10000,
    cpu = 0x10010,
    pace = 0x10020,
    mmu = 0x10030,
    cp15 = 0x10040
};

#endif  // _CHUNK_TYPE_H_