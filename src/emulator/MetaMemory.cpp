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

#include "MetaMemory.h"

#include <cstddef>

#include "EmBankSRAM.h"
#include "EmCommon.h"
#include "EmMemory.h"

void MetaMemory::Save(SessionFile&) {
    // CSTODO
}

void MetaMemory::Load(SessionFile&) {
    // CSTODO
}

void MetaMemory::MarkRange(emuptr start, emuptr end, uint8 v) {
    // If there's no meta-memory (not needed for dedicated framebuffers)
    // just leave.

    if (EmMemGetBank(start).xlatemetaaddr == NULL) return;

    // If the beginning and end of the buffer are not in the same address
    // space, just leave.  This can happen while initializing the Dragonball's
    // LCD -- for a while, the LCD framebuffer range falls off the end
    // of the dynamic heap.

    if (start >= 0 && start < 0 + gRAMBank_Size && end >= 0 + gRAMBank_Size) {
        end = gRAMBank_Size - 1;
    }

    if (start >= gMemoryStart && start < gMemoryStart + gRAMBank_Size &&
        end >= gMemoryStart + gRAMBank_Size) {
        end = gMemoryStart + gRAMBank_Size - 1;
    }

    uint8* startP = EmMemGetMetaAddress(start);
    uint8* endP = startP + (end - start);  // EmMemGetMetaAddress (end);
    uint8* end4P = (uint8*)(((long)endP) & ~3);
    uint8* p = startP;

    EmAssert(end >= start);
    EmAssert(endP >= startP);
    EmAssert(endP - startP == (ptrdiff_t)(end - start));

#if 1
    // Optimization: if there are no middle longs to fill, just
    // do everything a byte at a time.

    if (end - start < 12) {
        while (p < endP) {
            *p++ &= v;
        }
    } else {
        uint32 longValue = v;
        longValue |= (longValue << 8);
        longValue |= (longValue << 16);

        while (((long)p) & 3)  // while there are leading bytes
        {
            *p++ |= v;
        }

        while (p < end4P)  // while there are middle longs
        {
            *(uint32*)p |= longValue;
            p += 4;
        }

        while (p < endP)  // while there are trailing bytes
        {
            *p++ |= v;
        }
    }
#else
    for (p = startP; p < endP; ++p) {
        *p |= v;
    }
#endif
}

// ---------------------------------------------------------------------------
//		� MetaMemory::UnmarkRange
// ---------------------------------------------------------------------------

void MetaMemory::UnmarkRange(emuptr start, emuptr end, uint8 v) {
    // If there's no meta-memory (not needed for dedicated framebuffers)
    // just leave.

    if (EmMemGetBank(start).xlatemetaaddr == NULL) return;

    // If the beginning and end of the buffer are not in the same address
    // space, just leave.  This can happen while initializing the Dragonball's
    // LCD -- for a while, the LCD framebuffer range falls off the end
    // of the dynamic heap.

    if (start >= 0 && start < 0 + gRAMBank_Size && end >= 0 + gRAMBank_Size) {
        end = gRAMBank_Size - 1;
    }

    if (start >= gMemoryStart && start < gMemoryStart + gRAMBank_Size &&
        end >= gMemoryStart + gRAMBank_Size) {
        end = gMemoryStart + gRAMBank_Size - 1;
    }

    uint8* startP = EmMemGetMetaAddress(start);
    uint8* endP = startP + (end - start);  // EmMemGetMetaAddress (end);
    uint8* end4P = (uint8*)(((long)endP) & ~3);
    uint8* p = startP;

    EmAssert(end >= start);
    EmAssert(endP >= startP);
    EmAssert(endP - startP == (ptrdiff_t)(end - start));

    v = ~v;

#if 1
    // Optimization: if there are no middle longs to fill, just
    // do everything a byte at a time.

    if (end - start < 12) {
        while (p < endP) {
            *p++ &= v;
        }
    } else {
        uint32 longValue = v;
        longValue |= (longValue << 8);
        longValue |= (longValue << 16);

        while (((long)p) & 3)  // while there are leading bytes
        {
            *p++ &= v;
        }

        while (p < end4P)  // while there are middle longs
        {
            *(uint32*)p &= longValue;
            p += 4;
        }

        while (p < endP)  // while there are trailing bytes
        {
            *p++ &= v;
        }
    }
#else
    for (p = startP; p < endP; ++p) {
        *p &= v;
    }
#endif
}

// ---------------------------------------------------------------------------
//		� MetaMemory::MarkUnmarkRange
// ---------------------------------------------------------------------------

void MetaMemory::MarkUnmarkRange(emuptr start, emuptr end, uint8 andValue, uint8 orValue) {
    // If there's no meta-memory (not needed for dedicated framebuffers)
    // just leave.

    if (EmMemGetBank(start).xlatemetaaddr == NULL) return;

    // If the beginning and end of the buffer are not in the same address
    // space, just leave.  This can happen while initializing the Dragonball's
    // LCD -- for a while, the LCD framebuffer range falls off the end
    // of the dynamic heap.

    if (start >= 0 && start < 0 + gRAMBank_Size && end >= 0 + gRAMBank_Size) {
        end = gRAMBank_Size - 1;
    }

    if (start >= gMemoryStart && start < gMemoryStart + gRAMBank_Size &&
        end >= gMemoryStart + gRAMBank_Size) {
        end = gMemoryStart + gRAMBank_Size - 1;
    }

    uint8* startP = EmMemGetMetaAddress(start);
    uint8* endP = startP + (end - start);  // EmMemGetMetaAddress (end);
    uint8* end4P = (uint8*)(((long)endP) & ~3);
    uint8* p = startP;

    EmAssert(end >= start);
    EmAssert(endP >= startP);
    EmAssert(endP - startP == (ptrdiff_t)(end - start));

    if (andValue == 0xFF) {
        while (p < endP) {
            *p++ |= orValue;
        }
    } else if (orValue == 0x00) {
        while (p < endP) {
            *p++ &= andValue;
        }
    } else {
#if 1
        // Optimization: if there are no middle longs to fill, just
        // do everything a byte at a time.

        if (end - start < 12) {
            while (p < endP)  // while there are trailing bytes
            {
                *p = (*p & andValue) | orValue;
                p++;
            }
        } else {
            uint32 longAnd = andValue;
            longAnd |= (longAnd << 8);
            longAnd |= (longAnd << 16);

            uint32 longOr = orValue;
            longOr |= (longOr << 8);
            longOr |= (longOr << 16);

            while (((long)p) & 3)  // while there are leading bytes
            {
                *p = (*p & andValue) | orValue;
                p++;
            }

            while (p < end4P)  // while there are middle longs
            {
                *(uint32*)p = ((*(uint32*)p) & longAnd) | longOr;
                p += 4;
            }

            while (p < endP)  // while there are trailing bytes
            {
                *p = (*p & andValue) | orValue;
                p++;
            }
        }
#else
        for (p = startP; p < endP; ++p) {
            *p = (*p & andValue) | orValue;
        }
#endif
    }
}
