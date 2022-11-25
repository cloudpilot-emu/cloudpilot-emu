/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmBankMapped.h"

#include <vector>

#include "EmCPU68K.h"  // gCPU68K
#include "EmMemory.h"  // Memory::InitializeBanks

// ===========================================================================
//		� Dummy Bank Accessors
// ===========================================================================
// Dummy banks are non-existent blocks of memory.  Dummy bank accessors do
// not do anything.

static EmAddressBank gAddressBank = {EmBankMapped::GetLong,        EmBankMapped::GetWord,
                                     EmBankMapped::GetByte,        EmBankMapped::SetLong,
                                     EmBankMapped::SetWord,        EmBankMapped::SetByte,
                                     EmBankMapped::GetRealAddress, EmBankMapped::ValidAddress,
                                     EmBankMapped::GetMetaAddress, EmBankMapped::AddOpcodeCycles};

struct MapRange {
    Bool Contains(const void* addr) {
        const char* begin = (const char*)this->realAddress;
        const char* end = begin + this->size;

        return ((addr >= begin) && (addr < end));
    }

    Bool Contains(emuptr addr) {
        emuptr begin = this->mappedAddress;
        emuptr end = begin + this->size;

        return ((addr >= begin) && (addr < end));
    }

    const void* realAddress;  // Address in host's space
    emuptr mappedAddress;     // Address that emulated code sees
    uint32 size;
};
typedef vector<MapRange> MapRangeList;

static MapRangeList gMappedRanges;
static MapRangeList::iterator gLastIter;

// Map in blocks starting at this address.  I used to have it way out of
// the way at 0x60000000.  However, there's a check in SysGetAppInfo to
// make sure that certain addresses are less than 0x20000000.  So set
// kMemoryStart lower than that.
//
// OK...Let's try again.  I had changed this to manage 0x18000000 to
// 0x1FFFFFFF.  However, the SED 1375 is mapped to 0x1F000000.  So let's
// make sure we stay out of that range, too.
//
// Urm...TRG is mapping some stuff into 0x18000000.  Now we have to stay
// out of its way, too.

const emuptr kMemoryStart = 0x13000000;
const emuptr kMemoryFinish = 0x18000000;
const int32 kMemorySize = kMemoryFinish - kMemoryStart;  // 80MB

static MapRangeList::iterator PrvGetMappingInfo(const void* addr);
static MapRangeList::iterator PrvGetMappingInfo(emuptr addr);
static emuptr PrvEnsureAligned(emuptr candidate, const void* addr);
static void PrvInvalidateCache(void);
static void PrvCheckRanges(void);

/***********************************************************************
 *
 * FUNCTION:	EmBankMapped::Initialize
 *
 * DESCRIPTION: Standard initialization function.  Responsible for
 *				initializing this sub-system when a new session is
 *				created.  Will be followed by at least one call to
 *				Reset or Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankMapped::Initialize(void) {
    gMappedRanges.clear();
    ::PrvInvalidateCache();
}

/***********************************************************************
 *
 * FUNCTION:	EmBankMapped::Reset
 *
 * DESCRIPTION:	Standard reset function.  Sets the sub-system to a
 *				default state.  This occurs not only on a Reset (as
 *				from the menu item), but also when the sub-system
 *				is first initialized (Reset is called after Initialize)
 *				as well as when the system is re-loaded from an
 *				insufficient session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankMapped::Reset(Bool /*hardwareReset*/) {
    // Note: I used to clear out the mapped ranges in this function.
    // However, this didn't work out too well.  While manually resetting
    // the device (as occurs at the end of a debugging session), there
    // would be several StMemoryMapper objects instantiated.  After the
    // reset process, these StMemoryMappers would try to destruct
    // themselves, calling EmBankMapped::UnmapPhysicalMemory in the
    // process.  But since the mapped ranges had been cleared, that
    // function would throw an assert.  So....I can't think of a reason
    // for clearing out the ranges en masse,  so let's no do that anymore.
}

/***********************************************************************
 *
 * FUNCTION:	EmBankMapped::Dispose
 *
 * DESCRIPTION:	Standard dispose function.  Completely release any
 *				resources acquired or allocated in Initialize and/or
 *				Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankMapped::Dispose(void) {
    gMappedRanges.clear();
    ::PrvInvalidateCache();
}

/***********************************************************************
 *
 * FUNCTION:    EmBankMapped::SetBankHandlers
 *
 * DESCRIPTION: Set the bank handlers UAE uses to dispatch memory
 *				access operations.
 *
 * PARAMETERS:  None
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void EmBankMapped::SetBankHandlers(void) {
    Memory::InitializeBanks(gAddressBank, EmMemBankIndex(kMemoryStart), kMemorySize >> 16);
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::GetLong
// ---------------------------------------------------------------------------

uint32 EmBankMapped::GetLong(emuptr address) {
    if (CHECK_FOR_ADDRESS_ERROR && (address & 1) != 0) {
        AddressError(address, sizeof(uint32), true);
    }

    uint8* p = GetRealAddressForSize(address, 4);

    if (p == NULL) {
        return ~0;
    }

    return (((uint32)p[0]) << 24) | (((uint32)p[1]) << 16) | (((uint32)p[2]) << 8) | p[3];
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::GetWord
// ---------------------------------------------------------------------------

uint32 EmBankMapped::GetWord(emuptr address) {
    if (CHECK_FOR_ADDRESS_ERROR && (address & 1) != 0) {
        AddressError(address, sizeof(uint16), true);
    }

    uint8* p = GetRealAddressForSize(address, 2);

    if (p == NULL) {
        return ~0;
    }

    return (((uint32)p[0]) << 8) | p[1];
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::GetByte
// ---------------------------------------------------------------------------

uint32 EmBankMapped::GetByte(emuptr address) {
    uint8* p = GetRealAddressForSize(address, 1);

    if (p == NULL) {
        return ~0;
    }

    return p[0];
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::SetLong
// ---------------------------------------------------------------------------

void EmBankMapped::SetLong(emuptr address, uint32 value) {
    if (CHECK_FOR_ADDRESS_ERROR && (address & 1) != 0) {
        AddressError(address, sizeof(uint32), false);
    }

    uint8* p = GetRealAddressForSize(address, 4);

    if (p == NULL) {
        return;
    }

    p[0] = (uint8)(value >> 24);
    p[1] = (uint8)(value >> 16);
    p[2] = (uint8)(value >> 8);
    p[3] = (uint8)(value >> 0);
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::SetWord
// ---------------------------------------------------------------------------

void EmBankMapped::SetWord(emuptr address, uint32 value) {
    if (CHECK_FOR_ADDRESS_ERROR && (address & 1) != 0) {
        AddressError(address, sizeof(uint16), false);
    }

    uint8* p = GetRealAddressForSize(address, 2);

    if (p == NULL) {
        return;
    }

    p[0] = (uint8)(value >> 8);
    p[1] = (uint8)(value >> 0);
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::SetByte
// ---------------------------------------------------------------------------

void EmBankMapped::SetByte(emuptr address, uint32 value) {
    uint8* p = GetRealAddressForSize(address, 1);

    if (p == NULL) {
        return;
    }

    p[0] = (uint8)(value >> 0);
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::ValidAddress
// ---------------------------------------------------------------------------

int EmBankMapped::ValidAddress(emuptr address, uint32 size) {
    uint8* realAddress = GetRealAddressForSize(address, size);
    int result = realAddress != NULL;

    return result;
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmBankMapped::GetRealAddress(emuptr address) {
    MapRangeList::iterator iter = ::PrvGetMappingInfo(address);

    if (iter == gMappedRanges.end()) return NULL;

    return ((uint8*)iter->realAddress) + (address - iter->mappedAddress);
}

uint8* EmBankMapped::GetRealAddressForSize(emuptr address, uint32 size) {
    MapRangeList::iterator iter = ::PrvGetMappingInfo(address);

    if (iter == gMappedRanges.end()) return NULL;
    if (size > iter->size - (address - iter->mappedAddress)) return NULL;

    return ((uint8*)iter->realAddress) + (address - iter->mappedAddress);
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::GetMetaAddress
// ---------------------------------------------------------------------------

uint8* EmBankMapped::GetMetaAddress(emuptr address) {
    UNUSED_PARAM(address)

    static uint8 dummyBits[4] = {0, 0, 0, 0};

    return dummyBits;
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::GetEmulatedAddress
// ---------------------------------------------------------------------------

emuptr EmBankMapped::GetEmulatedAddress(const void* address) {
    MapRangeList::iterator iter = ::PrvGetMappingInfo(address);

    if (iter == gMappedRanges.end()) {
        EmAssert(false);
        return EmMemNULL;
    }

    return iter->mappedAddress + ((char*)address - (char*)iter->realAddress);
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::AddOpcodeCycles
// ---------------------------------------------------------------------------

void EmBankMapped::AddOpcodeCycles(void) {}

// ---------------------------------------------------------------------------
//		� EmBankMapped::AddressError
// ---------------------------------------------------------------------------

void EmBankMapped::AddressError(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->AddressError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::InvalidAccess
// ---------------------------------------------------------------------------

void EmBankMapped::InvalidAccess(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->BusError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::MapPhysicalMemory
// ---------------------------------------------------------------------------
// Maps a range of physical memory to appear at the same location of the
// emulated Palm OS's virtual memory.

void EmBankMapped::MapPhysicalMemory(const void* addr, uint32 size) {
    if (addr == NULL) return;

    emuptr candidate = ::PrvEnsureAligned(kMemoryStart, addr);

    // If the list is empty, add the item

    if (gMappedRanges.size() == 0) {
        MapRange range;

        range.realAddress = addr;
        range.mappedAddress = candidate;
        range.size = size;

        gLastIter = gMappedRanges.insert(gMappedRanges.begin(), range);
        ::PrvCheckRanges();

        //		::PrvInvalidateCache ();

        return;
    }

    // Find an available space for the range.

    MapRangeList::iterator iter = gMappedRanges.begin();

    while (iter != gMappedRanges.end()) {
        // If there's room in front of the current block...

        if (iter->mappedAddress >= candidate + size) {
            // ...insert a map record in front of the current block.

            MapRange range;

            range.realAddress = addr;
            range.mappedAddress = candidate;
            range.size = size;

            gLastIter = gMappedRanges.insert(iter, range);
            ::PrvCheckRanges();

            //			::PrvInvalidateCache ();

            return;
        }

        // No room at the inn...move onto the next block.

        candidate = ::PrvEnsureAligned(iter->mappedAddress + iter->size, addr);
        ++iter;
    }

    // Couldn't find room at the beginning or the middle of the list. Add to the end.

    MapRangeList::iterator last = gMappedRanges.end();
    iter = last - 1;

    MapRange range;

    range.realAddress = addr;
    range.mappedAddress = ::PrvEnsureAligned(iter->mappedAddress + iter->size, addr);
    range.size = size;

    gLastIter = gMappedRanges.insert(last, range);
    ::PrvCheckRanges();

    //	::PrvInvalidateCache ();
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::UnmapPhysicalMemory
// ---------------------------------------------------------------------------
// Unmaps a range of physical memory from appearing at the same location of
// the emulated Palm OS's virtual memory.

void EmBankMapped::UnmapPhysicalMemory(const void* addr) {
    if (addr == NULL) return;

    MapRangeList::iterator iter = ::PrvGetMappingInfo(addr);

    // Take out this assert.  Without it, it's possible to unmap ranges
    // of memory that we may not have actually mapped in.  Being able to
    // do that makes things like Reset and Dispose methods easier to write.
    //	EmAssert (iter != gMappedRanges.end ());

    if (iter != gMappedRanges.end()) {
        gMappedRanges.erase(iter);
        ::PrvCheckRanges();

        ::PrvInvalidateCache();
    }
}

// ---------------------------------------------------------------------------
//		� EmBankMapped::GetMappingInfo
// ---------------------------------------------------------------------------

void EmBankMapped::GetMappingInfo(emuptr addr, void** start, uint32* len) {
    MapRangeList::iterator iter = ::PrvGetMappingInfo(addr);

    if (iter != gMappedRanges.end()) {
        if (start) *start = (void*)iter->realAddress;

        if (len) *len = iter->size;
    } else {
        if (start) *start = NULL;

        if (len) *len = 0;
    }
}

// ---------------------------------------------------------------------------
//		� PrvGetMappingInfo
// ---------------------------------------------------------------------------

MapRangeList::iterator PrvGetMappingInfo(const void* addr) {
    if (gLastIter != gMappedRanges.end() && gLastIter->Contains(addr)) return gLastIter;

    MapRangeList::iterator iter = gMappedRanges.begin();
    while (iter != gMappedRanges.end()) {
        if (iter->Contains(addr)) break;

        ++iter;
    }

    gLastIter = iter;

    return iter;
}

MapRangeList::iterator PrvGetMappingInfo(emuptr addr) {
    if (gLastIter != gMappedRanges.end() && gLastIter->Contains(addr)) return gLastIter;

    MapRangeList::iterator iter = gMappedRanges.begin();
    while (iter != gMappedRanges.end()) {
        if (iter->Contains(addr)) break;

        ++iter;
    }

    gLastIter = iter;

    return iter;
}

emuptr PrvEnsureAligned(emuptr candidate, const void* addr) {
    // Make sure that "candidate" -- which is the candidate
    // mapped address -- maintains the same alignment as
    // the incoming address.

    while ((candidate & 0x03) != ((long)addr & 0x03)) ++candidate;

    return candidate;
}

void PrvInvalidateCache(void) { gLastIter = gMappedRanges.end(); }

void PrvCheckRanges(void) {
#if _DEBUG
    /*
            Check for overlaps.  We iterate over the container with
            two iterators: outerIter and innerIter.  These two iterators
            will allow us to compare all possible combinations of
            elements in the container.

            When checking for overlaps, there are six cases to consider:

                                    *-----------*			Segment A

                    *---*							Possible segment
       B's.
                    *----------*
                    *---------------------------*
                                            *---*
                                            *---------------*
                                                                    *---*

            An overlap occurs if any part of segment B crosses segment A.
            From the drawing, we can see that the first and last possible
            segments B's are OK; the others overlap with segment A in
            some fashion.  Thus, Segment B is OK if it's end is less than
            Segment A's start, or its start is greater than segment A's end.
    */

    MapRangeList::iterator outerIter = gMappedRanges.begin();
    while (outerIter != gMappedRanges.end()) {
        MapRangeList::iterator innerIter = outerIter + 1;
        while (innerIter != gMappedRanges.end()) {
            char* outerStart = (char*)outerIter->realAddress;
            char* innerStart = (char*)innerIter->realAddress;

            char* outerEnd = outerStart + outerIter->size;
            char* innerEnd = innerStart + innerIter->size;

            if (innerEnd > outerStart && innerStart < outerEnd) {
                EmAssert(false);
            }

            ++innerIter;
        }

        ++outerIter;
    }
#endif
}
