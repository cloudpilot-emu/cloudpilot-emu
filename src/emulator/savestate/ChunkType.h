#ifndef _CHUNK_TYPE_H_
#define _CHUNK_TYPE_H_

#include "EmCommon.h"

enum class ChunkType : uint32 {
    cpu68k = 0x0010,
    regsEZ = 0x0020,
    session = 0x0030,
    systemState = 0x0040,
    patchMgr = 0x0080,
    regsVZ = 0x0090,
    regsFrameBuffer = 0x00a0,
    regsSED1376 = 0x00b0,
    regsSED1375 = 0x00c0,
    regsMQ1xx = 0x00d0,
    regsPLLDAtlantiC = 0x00e0,
    regs328 = 0x00f0,
    regsVZHandera330 = 0x0110,
    spiSlave330Current = 0x0120,
    regs330CPLD = 0x0130,
    spiSlaveADS784 = 0x140,
    regsUsbClie = 0x150,
    regsUsbClieN700C = 0x160,
    regsSZ = 0x170,
    regsMQ1168 = 0x180,
    memoryStick = 0x190,
    regsSonyDsp = 0x1a0,
    regsMB86189 = 0x1b0,
    externalStorage = 0x1c0
};

#endif  // _CHUNK_TYPE_H_
