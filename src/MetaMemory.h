/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef _METAMEMORY_H_
#define _METAMEMORY_H_

#include "EmMemory.h"  // EmMemGetMetaAddress

class MetaMemory {
   public:
    static void Save(SessionFile&);
    static void Load(SessionFile&);

    static void MarkTotalAccess(emuptr begin, emuptr end);
    static void SetAccess(emuptr begin, emuptr end, uint8 bits);

    static void MarkScreen(emuptr begin, emuptr end);
    static void UnmarkScreen(emuptr begin, emuptr end);

    static void MarkInstructionBreak(emuptr opcodeLocation);
    static void UnmarkInstructionBreak(emuptr opcodeLocation);
    static Bool IsCPUBreak(emuptr opcodeLocation);
    static Bool IsCPUBreak(uint8* metaLocation);

    static Bool IsScreenBuffer8(uint8* metaAddress);   // Inlined, defined below
    static Bool IsScreenBuffer16(uint8* metaAddress);  // Inlined, defined below
    static Bool IsScreenBuffer32(uint8* metaAddress);  // Inlined, defined below
    static Bool IsScreenBuffer(uint8* metaAddress, uint32 size);

   private:
    static void MarkRange(emuptr start, emuptr end, uint8 v);
    static void UnmarkRange(emuptr start, emuptr end, uint8 v);
    static void MarkUnmarkRange(emuptr start, emuptr end, uint8 andValue, uint8 orValue);

    enum {
        kNoAppAccess = 0x0001,
        kNoSystemAccess = 0x0002,
        kNoMemMgrAccess = 0x0004,
        kUnusedMeta1 = 0x0008,
        kStackBuffer = 0x0010,   // Stack buffer; check to see if below-SP access is made.
        kScreenBuffer = 0x0020,  // Screen buffer; update host screen if these bytes are changed.
        kInstructionBreak = 0x0040,  // Halt CPU emulation and check to see why.
        kDataBreak = 0x0080,         // Halt CPU emulation and check to see why.

        kLowMemoryBits = kNoAppAccess | kNoSystemAccess | kNoMemMgrAccess,
        kGlobalsBits = kNoAppAccess,
        kMPTBits = kNoAppAccess,
        kMemStructBits = kNoAppAccess | kNoSystemAccess,
        kLowStackBits = kNoAppAccess | kNoSystemAccess | kNoMemMgrAccess,
        kFreeChunkBits = kNoAppAccess | kNoSystemAccess,
        kUnlockedChunkBits = kNoAppAccess | kNoSystemAccess,
        kScreenBits = kNoAppAccess | kScreenBuffer,
        kUIObjectBits = kNoAppAccess,
        kAccessBitMask = kNoAppAccess | kNoSystemAccess | kNoMemMgrAccess,
        kFreeAccessBits = 0
    };
};

// Macros to take a byte of bits and produce an
// 8-bit, 16-bit, or 32-bit version of it.

#define META_BITS_32(bits)                                                          \
    (((uint32)(bits)) << 24) | (((uint32)(bits)) << 16) | (((uint32)(bits)) << 8) | \
        (((uint32)(bits)))

#define META_BITS_16(bits) (((uint16)(bits)) << 8) | (((uint16)(bits)))

#define META_BITS_8(bits) (((uint8)(bits)))

// Macros to fetch the appropriate 8-bit, 16-bit, or
// 32-bit value from meta-memory.
//
// Fetch the values with byte operations in order to
// support CPUs that don't allow, say, 16-bit fetches
// on odd boundaries or 32-bit fetches on odd 16-bit
// boundaries.

#define META_VALUE_32(p)                                                   \
    ((((uint32)((uint8*)p)[0]) << 24) | (((uint32)((uint8*)p)[1]) << 16) | \
     (((uint32)((uint8*)p)[2]) << 8) | (((uint32)((uint8*)p)[3])))

#define META_VALUE_16(p) ((((uint16)((uint8*)p)[0]) << 8) | (((uint16)((uint8*)p)[1])))

#define META_VALUE_8(p) (*(uint8*)p)

inline Bool MetaMemory::IsScreenBuffer(uint8* metaAddress, uint32 size) {
    if (size == 1) {
        const uint8 kMask = META_BITS_8(kScreenBuffer);

        return (META_VALUE_8(metaAddress) & kMask) != 0;
    } else if (size == 2) {
        const uint16 kMask = META_BITS_16(kScreenBuffer);

        return (META_VALUE_16(metaAddress) & kMask) != 0;
    } else if (size == 4) {
        const uint32 kMask = META_BITS_32(kScreenBuffer);

        return (META_VALUE_32(metaAddress) & kMask) != 0;
    }

    const uint8 kMask = META_BITS_8(kScreenBuffer);

    for (uint32 ii = 0; ii < size; ++ii) {
        if (((*(uint8*)(metaAddress + ii)) & kMask) != 0) {
            return true;
        }
    }

    return false;
}

inline Bool MetaMemory::IsScreenBuffer8(uint8* metaAddress) {
    const uint8 kMask = META_BITS_8(kScreenBuffer);

    return (META_VALUE_8(metaAddress) & kMask) != 0;
}

inline Bool MetaMemory::IsScreenBuffer16(uint8* metaAddress) {
    const uint16 kMask = META_BITS_16(kScreenBuffer);

    return (META_VALUE_16(metaAddress) & kMask) != 0;
}

inline Bool MetaMemory::IsScreenBuffer32(uint8* metaAddress) {
    const uint32 kMask = META_BITS_32(kScreenBuffer);

    return (META_VALUE_32(metaAddress) & kMask) != 0;
}

inline Bool MetaMemory::IsCPUBreak(emuptr opcodeLocation) {
    EmAssert((opcodeLocation & 1) == 0);
    return IsCPUBreak(EmMemGetMetaAddress(opcodeLocation));
}

inline Bool MetaMemory::IsCPUBreak(uint8* metaLocation) {
    return ((*metaLocation) & kInstructionBreak) != 0;
}

// ---------------------------------------------------------------------------
//		� MetaMemory::MarkTotalAccess
// ---------------------------------------------------------------------------

inline void MetaMemory::MarkTotalAccess(emuptr begin, emuptr end) {
    UnmarkRange(begin, end, kAccessBitMask);
}

// ---------------------------------------------------------------------------
//		� MetaMemory::SetAccess
// ---------------------------------------------------------------------------

inline void MetaMemory::SetAccess(emuptr begin, emuptr end, uint8 bits) {
    MarkUnmarkRange(begin, end, ~kAccessBitMask, bits);
}

inline void MetaMemory::MarkScreen(emuptr begin, emuptr end) { SetAccess(begin, end, kScreenBits); }

// ---------------------------------------------------------------------------
//		� MetaMemory::UnmarkScreen
// ---------------------------------------------------------------------------

inline void MetaMemory::UnmarkScreen(emuptr begin, emuptr end) {
    MarkTotalAccess(begin, end);
    UnmarkRange(begin, end, kScreenBuffer);
}

// ---------------------------------------------------------------------------
//		� MetaMemory::MarkInstructionBreak
// ---------------------------------------------------------------------------

inline void MetaMemory::MarkInstructionBreak(emuptr opcodeLocation) {
    EmAssert((opcodeLocation & 1) == 0);

    uint8* ptr = EmMemGetMetaAddress(opcodeLocation);

    *ptr |= kInstructionBreak;
}

// ---------------------------------------------------------------------------
//		� MetaMemory::UnmarkInstructionBreak
// ---------------------------------------------------------------------------

inline void MetaMemory::UnmarkInstructionBreak(emuptr opcodeLocation) {
    EmAssert((opcodeLocation & 1) == 0);

    uint8* ptr = EmMemGetMetaAddress(opcodeLocation);

    *ptr &= ~kInstructionBreak;
}

#endif  // _METAMEMORY_H_
