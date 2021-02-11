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
    regsSED1376 = 0x00b0
};

#endif  // _CHUNK_TYPE_H_
