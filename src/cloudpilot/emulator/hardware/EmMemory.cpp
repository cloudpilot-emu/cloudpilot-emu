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

#include "EmMemory.h"

#include <array>

#include "EmBankDRAM.h"    // EmBankDRAM::Initialize
#include "EmBankDummy.h"   // EmBankDummy::Initialize
#include "EmBankMapped.h"  // EmBankMapped::Initialize
#include "EmBankROM.h"     // EmBankROM::Initialize
#include "EmBankRegs.h"    // EmBankRegs::Initialize
#include "EmBankSRAM.h"    // EmBankSRAM::Initialize
#include "EmCommon.h"
#include "EmDevice.h"
#include "EmSession.h"  // gSession, GetDevice
#include "MemoryRegion.h"
#include "MetaMemory.h"  // MetaMemory::Initialize
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

/*
        Hitchhiker's Guide To Accessing Memory

        When emulating CPU instructions, all memory access must be emulated as
        well.  Additionally, the memory location that the emulated instruction
        is accessing is not necessarily the same as the memory location in the
        emulator's address space.  That is, memory location 0x00001234 in the
        emulated process's address space is not the same as memory location
        0x00001234 in the emulator's address space.  Instead, the emulated
        0x00001234 is actually 0x00001234 bytes into some buffer that we've
        allocate for the purpose of holding the emulated address space's contents.

        There are several ranges of Palm OS memory that we need to emulate:

                - Low-memory RAM (for the dynamic heap)
                - High-memory RAM (for the storage heap)
                - ROM
                - Dragonball and other memory-mapped registers
                - Everything else

        Each of these ranges (except for the last one) is managed by a set of
        functions for reading from and writing to that memory and a chunk of
        memory that we allocate as a "backing store" for that memory.

        Thus, for each of these ranges, we need to quickly determine what set of
        functions need to be called.  The method for doing this is to divide the
        entire 4GB address space into 64K banks, each 64K long.

        Each 64K bank is managed by a set of functions.  These functions handle
        reading from and writing to that 64K bank, handling 1-byte, 2-byte, and
        4-byte accesses.  They also manage mapping from the emulated addres space
        to the physical space as viewed by the emulator, and validate memory
        addresses.

        Because there are 64K of these 64K banks, the functions that manage each
        bank are stored in an array of 64K elements.  When we need to invoke a
        function for a particular bank, the upper word of the prospective memory
        address is used as a "bank index" into this array.  From that, the right
        set of functions is found, and the desired function from that set of
        functions is called.

        This 64K array is very sparse.  There are one to four entries for managing
        the dynamic heap (depending on whether the dynamic heap is 64K, 96K, 128K,
        or 256K), 2 to 128 entries for the storage heap (depending on whether the
        storage heap is 128K to 8Meg), 8 to 32 entries for the ROM (again,
        depending on its size), and 1 for the Dragonball registers. That leaves
        the remaining 64K - 12 entries to be managed by a set of functions that do
        little but signal an address error if the memory they manage is being
        accessed.

        Pictorally, it looks like this:

        Emulated 4G space:				Memory function array
   Address Bank functions

        0x00000000	+--------------+	+--------------------------------+
                                |      64K     |<---| Ptr to fns that mng this range |------+
        0x00010000	+--------------+	+--------------------------------+		|
                                |      64K     |<---| Ptr to fns that mng this range |------+
        0x00020000	+--------------+	+--------------------------------+		|
                                |      64K     |
   +----->	EmBankDRAM::GetLong, EmBankDRAM::SetLong
                                \/\/\/\/\/\/\/\/
   EmBankDRAM::GetWord, EmBankDRAM::SetWord
                                \/\/\/\/\/\/\/\/
   EmBankDRAM::GetByte, EmBankDRAM::SetByte |      64K     |
   etc. 0x10000000	+--------------+	+--------------------------------+ |      64K |<---|
   Ptr to fns that mng this range |------+ 0x10010000	+--------------+
   +--------------------------------+		| |      64K     |<---| Ptr to fns that mng this
   range |------+ 0x10020000	+--------------+	+--------------------------------+
   |
                                |      64K     |
   +----->	EmBankSRAM::GetLong, EmBankSRAM::SetLong
                                \/\/\/\/\/\/\/\/
   EmBankSRAM::GetWord, EmBankSRAM::SetWord
                                \/\/\/\/\/\/\/\/
   EmBankSRAM::GetByte, EmBankSRAM::SetByte |      64K     |
   etc. 0x10C00000	+--------------+	+--------------------------------+ |      64K |<---|
   Ptr to fns that mng this range |------+ 0x10C10000	+--------------+
   +--------------------------------+		| |      64K     |<---| Ptr to fns that mng this
   range |------+ 0x10C20000	+--------------+	+--------------------------------+
   |
                                |      64K     |
   +----->	EmBankROM::GetLong, EmBankROM::SetLong
                                \/\/\/\/\/\/\/\/
   EmBankROM::GetWord, EmBankROM::SetWord
                                \/\/\/\/\/\/\/\/
   EmBankROM::GetByte, EmBankROM::SetByte |      64K     |
   etc. 0xFFFE0000	+--------------+	+--------------------------------+ |      64K |<---|
   Ptr to fns that mng this range | 0xFFFF0000	+--------------+
   +--------------------------------+ |      64K     |<---| Ptr to fns that mng this range |------+
        0xFFFFFFFF	+--------------+	+--------------------------------+		|
                                                                                                                                                        +----->	EmRegs::GetLong, EmRegs::SetLong
                                                                                                                                                                        EmRegs::GetWord, EmRegs::SetWord
                                                                                                                                                                        EmRegs::GetByte, EmRegs::SetByte
                                                                                                                                                                        etc.

        All of the remaining memory function array entries are initialized with
        pointers to the EmBankDummy::GetLong, EmBankDummy::SetLong, etc., set of
        functions.

        Using EmBankMapped accessors, we also have the facility for mapping memory
        from the emulator's address space directly into the emulated address
        space.  This memory appears as additional RAM that magically appears out
        of nowhere.  This mechanism makes it easy for us to "inject" data into the
        emulated address space.  It also makes it easier to call emulated ROM
        functions as subroutines.  Some ROM functions take as parameters pointers
        to memory locations that are to receive values (for example, pointers to
        local variables).  For those functions, we just map the emulator's stack
        into the emulated address space.  After that's done, we can pass pointers
        to stack-based variables with impunity.

        A 64K bank can be further sub-divided into 1, 2, or 4 byte chunks
        spanning any range of the 64K chunk.  EmBankRegs manages a collection
        of EmRegs objects.  The EmRegs abstract base class is sub-classed into
        classes that specify the sub-range of the 64K for which they are
        responsible and dispatch memory accesses to individual functions
        that handle accesses at more-or-less a byte level.

        Mapping from emulated addresses to "real" addresses (that is, the
        corresponding memory locations within the buffers we've allocated to
        "stand in" for the emulated memory) is performed via macros and inline
        functions, described below.

        Executive summary:

                - use EmMemGetFoo if you want the address checked for even/oddness.
                - use EmMemDoGetFoo to actually get the value using a physical address.

        EmMemGet32, EmMemGet16, EmMemGet8
        EmMemPut32, EmMemPut16, EmMemPut8
                Inline functions that dispatch to the right bank of functions
                using EmMemCallGetFunc, EmMemGetBank, and EmMemBankIndex.

        EmMemBankIndex
                Returns a bank index for the given address.

        EmMemGetBank
                Gets the right EmAddressBank object.

        EmMemCallGetFunc
                Merely calls the given function through the EmAddressBank fn ptr.

        EmMemDoGet32, EmMemDoGet16, EmMemDoGet8
        EmMemDoPut32, EmMemDoPut16, EmMemDoPut8
                Very low-level memory access.  They return the value at the
                given physical location, byte-swapping if needed.
*/

// Types.

#pragma mark Types

// Globals.

#pragma mark Globals

EmAddressBank* gEmMemBanks[65536];  // (normally defined in memory.c)

Bool gPCInRAM;
Bool gPCInROM;

/*
uint32 gTotalMemorySize;
uint32 gRAMSize;
uint32 gFramebufferMemorySize;

uint8* gMemory;
uint8* gDirtyPages;

uint8* gFramebufferMemory;
uint8* gFramebufferDirtyPages;
*/

MemAccessFlags gMemAccessFlags = {
    MASTER_RUNTIME_VALIDATE_SWITCH, MASTER_RUNTIME_VALIDATE_SWITCH, MASTER_RUNTIME_VALIDATE_SWITCH,
    MASTER_RUNTIME_VALIDATE_SWITCH, MASTER_RUNTIME_VALIDATE_SWITCH, MASTER_RUNTIME_VALIDATE_SWITCH,
    MASTER_RUNTIME_VALIDATE_SWITCH, MASTER_RUNTIME_VALIDATE_SWITCH, MASTER_RUNTIME_VALIDATE_SWITCH,
    MASTER_RUNTIME_VALIDATE_SWITCH,

    // User prevention flags.

    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    false,  // SRAM-get
    true,   // SRAM-set
    false,  // ROM-get
    true,   // ROM-set (HACK: We really want it to be true!)
    MASTER_RUNTIME_PREVENT_SWITCH, MASTER_RUNTIME_PREVENT_SWITCH,

    //	false,	// fCheck_UserChunkGet
    //	false,	// fCheck_UserChunkSet
    //	false,	// fCheck_SysChunkGet
    //	false,	// fCheck_SysChunkSet

    // System prevention flags.

    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	false,	// SRAM-get
    //	true,	// SRAM-set
    false,  // ROM-get
    true,   // ROM-set (HACK: We really want it to be true!)
    //	MASTER_RUNTIME_PREVENT_SWITCH,
    //	MASTER_RUNTIME_PREVENT_SWITCH
};

MemAccessFlags kZeroMemAccessFlags;

namespace {
    unique_ptr<uint8[]> memory;
    unique_ptr<uint8[]> dirtyPages;
    MemoryRegionMap regionMap;

    array<uint8*, N_MEMORY_REGIONS> memoryRegionPointers;
    array<uint8*, N_MEMORY_REGIONS> dirtyPageRegionPointers;

    constexpr MemoryRegion ORDERED_REGIONS[N_MEMORY_REGIONS] = {
        MemoryRegion::ram,     MemoryRegion::framebuffer, MemoryRegion::memorystick,
        MemoryRegion::sonyDsp, MemoryRegion::eSRAM,       MemoryRegion::metadata};

    uint32 get32(uint8* address) {
        return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
    }
}  // namespace

// ===========================================================================
//		� Memory
// ===========================================================================

// ---------------------------------------------------------------------------
//		� Memory::Initialize
// ---------------------------------------------------------------------------
// Initializes the RAM, ROM, and special memory areas of the emulator. Takes
// a stream handle to the ROM.

bool Memory::Initialize(const uint8* romBuffer, size_t romSize, EmDevice& device) {
    bool success = true;

    regionMap = device.GetMemoryRegionMap();

    const uint32 dirtyPagesSize =
        regionMap.GetTotalSize() / 8192 + (regionMap.GetTotalSize() % 8192 == 0 ? 0 : 1);

    memory = make_unique<uint8[]>(regionMap.GetTotalSize());
    dirtyPages = make_unique<uint8[]>(dirtyPagesSize);

    uint8* regionPtr = memory.get();
    uint8* dirtyPagePtr = dirtyPages.get();
    uint32* toc = reinterpret_cast<uint32*>(regionPtr + regionMap.GetTotalSize() -
                                            regionMap.GetRegionSize(MemoryRegion::metadata));

    memset(memory.get(), 0, regionMap.GetTotalSize());

    for (const auto region : ORDERED_REGIONS) {
        uint32 size = regionMap.GetRegionSize(region);

        memoryRegionPointers[static_cast<uint8>(region)] = size == 0 ? nullptr : regionPtr;
        dirtyPageRegionPointers[static_cast<uint8>(region)] = size == 0 ? nullptr : dirtyPagePtr;

        toc[0] = static_cast<uint8>(region);
        toc[1] = size;
        toc += 2;

        regionPtr += size;
        dirtyPagePtr += size / 8192;
    }

    *toc = 0xffffffff;

    memset(dirtyPages.get(), 0, dirtyPagesSize);
    dirtyPages[dirtyPagesSize - 1] = 0x01;

    // Clear everything out.

    memset(gEmMemBanks, 0, sizeof(gEmMemBanks));

    // Initialize the valid memory banks.

    EmBankDummy::Initialize();

    // Initialize the Hardware registers memory bank.  Do this
    // *before* initializing the other RAM banks so that we
    // know how memory is laid out from the chip selects.

    EmBankRegs::Initialize();

    // Initialize EmBankDRAM after initializing the EmBankSRAM. The order is
    // important for DragonballEZ. On Dragonball devices, DRAM is
    // at 0x00000000, and SRAM is at 0x10000000. But on EZ devices,
    // both start at 0x00000000. By calling EmBankDRAM::Initialize
    // second, we allow it to overwrite the EmAddressBank handlers
    // for the part of memory where they overlap.

    EmBankSRAM::Initialize();
    EmBankDRAM::Initialize();

    success = success && EmBankROM::Initialize(romSize, romBuffer);

    EmBankMapped::Initialize();

    EmAssert(gSession);
    if (gSession->GetDevice().HasFlash()) EmBankFlash::Initialize();

    return success;
}

/***********************************************************************
 *
 * FUNCTION:	Memory::Reset
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

void Memory::Reset(Bool hardwareReset) {
    EmBankDummy::Reset(hardwareReset);
    EmBankRegs::Reset(hardwareReset);
    EmBankSRAM::Reset(hardwareReset);
    EmBankDRAM::Reset(hardwareReset);
    EmBankROM::Reset(hardwareReset);
    EmBankMapped::Reset(hardwareReset);

    EmAssert(gSession);
    if (gSession->GetDevice().HasFlash()) EmBankFlash::Reset(hardwareReset);

    Memory::ResetBankHandlers();
}

template <typename T>
void Memory::Save(T& savestate) {
    EmBankRegs::Save(savestate);
}

template void Memory::Save(Savestate& savestate);
template void Memory::Save(SavestateProbe& savestate);

void Memory::Load(SavestateLoader& loader) {
    EmBankRegs::Load(loader);

    Memory::ResetBankHandlers();
}

/***********************************************************************
 *
 * FUNCTION:	Memory::Dispose
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

void Memory::Dispose(void) {
    EmBankDummy::Dispose();
    EmBankRegs::Dispose();
    EmBankSRAM::Dispose();
    EmBankDRAM::Dispose();
    EmBankROM::Dispose();
    EmBankMapped::Dispose();

    // We can't reliably call GetDevice here.  That's because the
    // session may not have been initialized (we could be disposing
    // of everything because an error condition occurred), and so
    // there may be no assigned device yet.  So we can't ask that
    // device if there's flash.  However, EmBankFlash::Dispose
    // doesn't do anything, so we don'thave to worry about it
    // being called.

    //	EmAssert (gSession);
    //	if (gSession->GetDevice ().HasFlash ())
    //		EmBankFlash::Dispose ();
}

// ---------------------------------------------------------------------------
//		� Memory::InitializeBanks
// ---------------------------------------------------------------------------
// Initializes the specified memory banks with the given data.

void Memory::InitializeBanks(EmAddressBank& iBankInitializer, int32 iStartingBankIndex,
                             int32 iNumberOfBanks) {
    for (int32 aBankIndex = iStartingBankIndex; aBankIndex < iStartingBankIndex + iNumberOfBanks;
         aBankIndex++) {
        gEmMemBanks[aBankIndex] = &iBankInitializer;
    }
}

// ---------------------------------------------------------------------------
//		� Memory::ResetBankHandlers
// ---------------------------------------------------------------------------
// Reset the memory bank handlers in response to the chip selects changing.
// Also called on Initialize, Reset, and Load, just to make sure everything's
// nailed down.

void Memory::ResetBankHandlers(void) {
    EmBankDummy::SetBankHandlers();
    EmBankRegs::SetBankHandlers();
    EmBankSRAM::SetBankHandlers();
    EmBankDRAM::SetBankHandlers();
    EmBankROM::SetBankHandlers();
    EmBankMapped::SetBankHandlers();

    if (gSession->GetDevice().HardwareID() == 0x0a /*halModelIDVisorPrism*/) {
        // Run this one again.  Visor Prism has this thing where the USB
        // controller is at 0x10800000, but it sets up the ROM chip select
        // for 0x10000000 to 0x10FFFFFF.  In order to not have EmBankROM
        // take control over the USB chip, have EmBankRegs install *after*
        // EmBankROM.
        EmBankRegs::SetBankHandlers();
    }

    EmAssert(gSession);
    if (gSession->GetDevice().HasFlash()) EmBankFlash::SetBankHandlers();
}

// ---------------------------------------------------------------------------
//		� Memory::MapPhysicalMemory
// ---------------------------------------------------------------------------
// Maps a range of physical memory to appear at the same location of the
// emulated Palm OS's virtual memory.

void Memory::MapPhysicalMemory(const void* addr, uint32 size) {
    EmBankMapped::MapPhysicalMemory(addr, size);
}

// ---------------------------------------------------------------------------
//		� Memory::UnmapPhysicalMemory
// ---------------------------------------------------------------------------
// Unmaps a range of physical memory from appearing at the same location of
// the emulated Palm OS's virtual memory.

void Memory::UnmapPhysicalMemory(const void* addr) { EmBankMapped::UnmapPhysicalMemory(addr); }

// ---------------------------------------------------------------------------
//		� Memory::GetMappingInfo
// ---------------------------------------------------------------------------

void Memory::GetMappingInfo(emuptr addr, void** start, uint32* len) {
    EmBankMapped::GetMappingInfo(addr, start, len);
}

// ---------------------------------------------------------------------------
//		� Memory::CheckNewPC
// ---------------------------------------------------------------------------

void Memory::CheckNewPC(emuptr newPC) {}

uint8* Memory::GetDirtyPagesForRegion(MemoryRegion region) {
    return dirtyPageRegionPointers[static_cast<uint8>(region)];
}

uint8* Memory::GetForRegion(MemoryRegion region) {
    return memoryRegionPointers[static_cast<uint8>(region)];
}

uint32 Memory::GetRegionSize(MemoryRegion region) { return regionMap.GetRegionSize(region); }

uint32 Memory::GetTotalMemorySize() { return regionMap.GetTotalSize(); }

uint8* Memory::GetTotalMemory() { return memory.get(); }

uint8* Memory::GetTotalDirtyPages() { return dirtyPages.get(); }

bool Memory::LoadMemoryV1(void* ram, size_t size) {
    if (size != regionMap.GetRegionSize(MemoryRegion::ram)) return false;

    memcpy(memoryRegionPointers[static_cast<uint8>(MemoryRegion::ram)], ram, size);
    return true;
}

bool Memory::LoadMemoryV2(void* memory, size_t size) {
    const uint32 ramSize = regionMap.GetRegionSize(MemoryRegion::ram);
    const uint32 framebufferSize = regionMap.GetRegionSize(MemoryRegion::framebuffer);

    if (size != ramSize + framebufferSize) return false;

    memcpy(memoryRegionPointers[static_cast<uint8>(MemoryRegion::ram)], memory, ramSize);
    memcpy(memoryRegionPointers[static_cast<uint8>(MemoryRegion::framebuffer)],
           reinterpret_cast<uint8*>(memory) + ramSize, framebufferSize);
    return true;
}

bool Memory::LoadMemoryV4(void* memory, size_t size) {
    if (size < 1024 || size % 4 != 0) return false;

    uint8* toc = static_cast<uint8*>(memory) + size - 1024;
    uint32 offset = 0;

    toc = static_cast<uint8*>(memory) + size - 1024;
    offset = 0;

    while (toc < static_cast<uint8*>(memory) + size) {
        uint32 regionType = get32(toc);
        toc += 4;

        uint32 regionSize = get32(toc);
        toc += 4;

        if (regionType == 0xffffffff) break;
        if (static_cast<MemoryRegion>(regionType) == MemoryRegion::metadata) continue;

        if (regionType >= N_MEMORY_REGIONS) {
            cerr << "ignoring unknown memory region " << regionType << endl << flush;
            continue;
        }

        uint32 expectedSize = regionMap.GetRegionSize(static_cast<MemoryRegion>(regionType));
        if (regionSize != expectedSize) {
            cerr << "region " << regionType << " has size " << regionSize << " , expected "
                 << expectedSize << " -> ignoring" << endl
                 << flush;
            continue;
        }

        if (offset + regionSize > size) {
            cerr << "invalid memory image" << endl << flush;
            return false;
        }

        memcpy(memoryRegionPointers[static_cast<uint8>(regionType)],
               static_cast<uint8*>(memory) + offset, regionSize);

        offset += regionSize;
    }

    return true;
}

#pragma mark -

// ===========================================================================
//		� CEnableFullAccess
// ===========================================================================

long CEnableFullAccess::fgAccessCount = 0;

// ---------------------------------------------------------------------------
//		� CEnableFullAccess::CEnableFullAccess
// ---------------------------------------------------------------------------

CEnableFullAccess::CEnableFullAccess(void) : fOldMemAccessFlags(gMemAccessFlags) {
    gMemAccessFlags = kZeroMemAccessFlags;

    ++fgAccessCount;
}

// ---------------------------------------------------------------------------
//		� CEnableFullAccess::~CEnableFullAccess
// ---------------------------------------------------------------------------

CEnableFullAccess::~CEnableFullAccess(void) {
    gMemAccessFlags = fOldMemAccessFlags;

    --fgAccessCount;
}

// ---------------------------------------------------------------------------
//		� CEnableFullAccess::AccessOK
// ---------------------------------------------------------------------------

Bool CEnableFullAccess::AccessOK(void) { return fgAccessCount > 0; }

#pragma mark -

// ---------------------------------------------------------------------------
// These functions with two kinds of "pointers": emuptr's
// and void*'s.  Accessing memory for each kind of pointer is different.
// Since the functions are template functions, we can't know at the time we
// write the function what kind of pointer we'll be dealing with, so we can't
// hard-code the memory-accessing method.  Instead, we provide all of these
// inline accessors and let the compiler sort it out.
// ---------------------------------------------------------------------------

inline char _get_byte(const void* p) { return *(char*)p; }

inline char _get_byte(emuptr p) { return (char)EmMemGet8(p); }

inline void _put_byte(void* p, const uint8 v) { *(uint8*)p = v; }

inline void _put_byte(emuptr p, const uint8 v) { EmMemPut8(p, v); }

inline uint16 _get_word(const void* p) { return *(uint16*)p; }

inline uint16 _get_word(emuptr p) { return (uint16)EmMemGet16(p); }

inline void _put_word(void* p, const uint16 v) { *(uint16*)p = v; }

inline void _put_word(emuptr p, const uint16 v) { EmMemPut16(p, v); }

inline uint32 _get_long(const void* p) { return *(uint32*)p; }

inline uint32 _get_long(emuptr p) { return (uint32)EmMemGet32(p); }

inline void _put_long(void* p, const uint32 v) { *(uint32*)p = v; }

inline void _put_long(emuptr p, const uint32 v) { EmMemPut32(p, v); }

inline const void* _get_real_address(const void* p) { return p; }

inline const void* _get_real_address(emuptr p) { return EmMemGetRealAddress(p); }

template <class T>
inline void _add_delta(T*& v, long delta) {
    v = (T*)(((char*)v) + delta);
}

inline void _add_delta(emuptr& v, long delta) { v += delta; }

template <class T>
inline void _increment(T& v) {
    _add_delta(v, 1);
}

template <class T>
inline void _decrement(T& v) {
    _add_delta(v, -1);
}

#pragma mark -

/***********************************************************************
 *
 * FUNCTION:	EmMem_memset
 *
 * DESCRIPTION: Same as Std C Library memset function.
 *
 * PARAMETERS:	Same as Std C Library memset function.
 *
 * RETURNED:	Same as Std C Library memset function.
 *
 ***********************************************************************/

emuptr EmMem_memset(emuptr dst, int val, size_t len) {
    emuptr q = dst;

    uint32 longVal = val;
    longVal |= (longVal << 8);
    longVal |= (longVal << 16);

    EmMemPutFunc longPutter = EmMemGetBank(dst).lput;
    EmMemPutFunc bytePutter = EmMemGetBank(dst).bput;

    while ((q & 3) && len > 0)  // while there are leading bytes
    {
        bytePutter(q, val);
        q += sizeof(char);
        len -= sizeof(char);
    }

    while (len >= sizeof(long))  // while there are middle longs
    {
        longPutter(q, longVal);
        q += sizeof(long);
        len -= sizeof(long);
    }

    while (len > 0)  // while there are trailing bytes
    {
        bytePutter(q, val);
        q += sizeof(char);
        len -= sizeof(char);
    }

    return dst;
}

/***********************************************************************
 *
 * FUNCTION:	EmMem_memchr
 *
 * DESCRIPTION: Same as Std C Library memchr function.
 *
 * PARAMETERS:	Same as Std C Library memchr function.
 *
 * RETURNED:	Same as Std C Library memchr function.
 *
 ***********************************************************************/

emuptr EmMem_memchr(emuptr src, int val, size_t len) {
    emuptr p = src;

    ++len;

    while (--len) {
        if (_get_byte(p) == val) return p;

        _increment(p);
    }

    return EmMemNULL;
}

/***********************************************************************
 *
 * FUNCTION:	EmMem_memcmp
 *
 * DESCRIPTION: Same as Std C Library memcmp function.
 *
 * PARAMETERS:	Same as Std C Library memcmp function.
 *
 * RETURNED:	Same as Std C Library memcmp function.
 *
 ***********************************************************************/

template <class T1, class T2>
int EmMem_memcmp(T1 src1, T2 src2, size_t len) {
    T1 p1 = src1;
    T2 p2 = src2;

    ++len;

    while (--len) {
        unsigned char ch1 = _get_byte(p1);
        unsigned char ch2 = _get_byte(p2);

        if (ch1 != ch2) return (ch1 < ch2) ? -1 : 1;

        _increment(p1);
        _increment(p2);
    }

    return 0;
}

// Instantiate EmMem_memcmp's that work with:
//
//			dest		source
//			------		------
//			void*		emuptr
//			const void* emuptr
//			emuptr 		void*
//			emuptr 		const void*
//			emuptr 		emuptr

template int EmMem_memcmp<void*, emuptr>(void* dst, emuptr src, size_t len);
template int EmMem_memcmp<const void*, emuptr>(const void* dst, emuptr src, size_t len);
template int EmMem_memcmp<emuptr, void*>(emuptr dst, void* src, size_t len);
template int EmMem_memcmp<emuptr, const void*>(emuptr dst, const void* src, size_t len);
template int EmMem_memcmp<emuptr, emuptr>(emuptr dst, emuptr src, size_t len);

/***********************************************************************
 *
 * FUNCTION:	EmMem_memcpy
 *
 * DESCRIPTION: Same as Std C Library memcpy function.
 *
 * PARAMETERS:	Same as Std C Library memcpy function.
 *
 * RETURNED:	Same as Std C Library memcpy function.
 *
 ***********************************************************************/

template <class T1, class T2>
T1 EmMem_memcpy(T1 dst, T2 src, size_t len) {
    T1 q = dst;
    T2 p = src;

    while (len--) {
        _put_byte(q, _get_byte(p));
        _increment(q);
        _increment(p);
    }

    return dst;
}

// Instantiate EmMem_memcpy's that work with:
//
//			dest		source
//			------		------
//			void*		emuptr
//			emuptr 		void*
//			emuptr 		const void*
//			emuptr 		emuptr

template void* EmMem_memcpy<void*, emuptr>(void* dst, emuptr src, size_t len);
template emuptr EmMem_memcpy<emuptr, void*>(emuptr dst, void* src, size_t len);
template emuptr EmMem_memcpy<emuptr, const void*>(emuptr dst, const void* src, size_t len);
template emuptr EmMem_memcpy<emuptr, emuptr>(emuptr dst, emuptr src, size_t len);

/***********************************************************************
 *
 * FUNCTION:	EmMem_memmove
 *
 * DESCRIPTION: Same as Std C Library memmove function.
 *
 * PARAMETERS:	Same as Std C Library memmove function.
 *
 * RETURNED:	Same as Std C Library memmove function.
 *
 ***********************************************************************/

template <class T1, class T2>
T1 EmMem_memmove(T1 dst, T2 src, size_t len) {
    T1 q = dst;
    T2 p = src;

    Bool backward = _get_real_address(dst) <= _get_real_address(src);

    if (backward) {
        while (len--) {
            _put_byte(q, _get_byte(p));
            _increment(q);
            _increment(p);
        }
    } else {
        _add_delta(q, len);
        _add_delta(p, len);

        while (len--) {
            _decrement(q);
            _decrement(p);
            _put_byte(q, _get_byte(p));
        }
    }

    return dst;
}

// Instantiate EmMem_memmove's that work with:
//
//			dest		source
//			------		------
//			void*		emuptr
//			emuptr 		void*
//			emuptr 		const void*
//			emuptr 		emuptr

template void* EmMem_memmove<void*, emuptr>(void* dst, emuptr src, size_t len);
template emuptr EmMem_memmove<emuptr, void*>(emuptr dst, void* src, size_t len);
template emuptr EmMem_memmove<emuptr, const void*>(emuptr dst, const void* src, size_t len);
template emuptr EmMem_memmove<emuptr, emuptr>(emuptr dst, emuptr src, size_t len);

#pragma mark -

/***********************************************************************
 *
 * FUNCTION:	EmMem_strlen
 *
 * DESCRIPTION: Same as Std C Library strlen function.
 *
 * PARAMETERS:	Same as Std C Library strlen function.
 *
 * RETURNED:	Same as Std C Library strlen function.
 *
 ***********************************************************************/

size_t EmMem_strlen(emuptr str) {
    emuptr eos = str;

    while (_get_byte(eos)) _increment(eos);

    return ((size_t)(eos - str));
}

/***********************************************************************
 *
 * FUNCTION:	EmMem_strcpy
 *
 * DESCRIPTION: Same as Std C Library strcpy function.
 *
 * PARAMETERS:	Same as Std C Library strcpy function.
 *
 * RETURNED:	Same as Std C Library strcpy function.
 *
 ***********************************************************************/

template <class T1, class T2>
T1 EmMem_strcpy(T1 dst, T2 src) {
    T1 q = dst;
    T2 p = src;
    char ch;

    do {
        ch = _get_byte(p);
        _increment(p);
        _put_byte(q, ch);
        _increment(q);
    } while (ch);

    return dst;
}

// Instantiate EmMem_strcpy's that work with:
//
//			dest		source
//			------		------
//			char*		emuptr
//			emuptr		char*
//			emuptr		const char*
//			emuptr 		emuptr

template char* EmMem_strcpy<char*, emuptr>(char* dst, emuptr src);
template emuptr EmMem_strcpy<emuptr, char*>(emuptr dst, char* src);
template emuptr EmMem_strcpy<emuptr, const char*>(emuptr dst, const char* src);
template emuptr EmMem_strcpy<emuptr, emuptr>(emuptr dst, emuptr src);

/***********************************************************************
 *
 * FUNCTION:	EmMem_strncpy
 *
 * DESCRIPTION: Same as Std C Library strncpy function.
 *
 * PARAMETERS:	Same as Std C Library strncpy function.
 *
 * RETURNED:	Same as Std C Library strncpy function.
 *
 ***********************************************************************/

template <class T1, class T2>
T1 EmMem_strncpy(T1 dst, T2 src, size_t len) {
    T1 q = dst;
    T2 p = src;

    ++len;

    while (--len) {
        char ch = _get_byte(p);
        _increment(p);

        _put_byte(q, ch);
        _increment(q);

        if (!ch) {
            while (--len) {
                _put_byte(q, 0);
                _increment(q);
            }

            break;
        }
    }

    return dst;
}

// Instantiate EmMem_strncpy's that work with:
//
//			dest		source
//			------		------
//			char*		emuptr
//			emuptr		char*
//			emuptr		const char*
//			emuptr		emuptr

template char* EmMem_strncpy<char*, emuptr>(char* dst, emuptr src, size_t len);
template emuptr EmMem_strncpy<emuptr, char*>(emuptr dst, char* src, size_t len);
template emuptr EmMem_strncpy<emuptr, const char*>(emuptr dst, const char* src, size_t len);
template emuptr EmMem_strncpy<emuptr, emuptr>(emuptr dst, emuptr src, size_t len);

/***********************************************************************
 *
 * FUNCTION:	EmMem_strcat
 *
 * DESCRIPTION: Same as Std C Library strcat function.
 *
 * PARAMETERS:	Same as Std C Library strcat function.
 *
 * RETURNED:	Same as Std C Library strcat function.
 *
 ***********************************************************************/

template <class T1, class T2>
T1 EmMem_strcat(T1 dst, T2 src) {
    T1 q = dst;
    T2 p = src;

    while (_get_byte(q)) _increment(q);

    while (_get_byte(p)) {
        _put_byte(q, _get_byte(p));
        _increment(q);
        _increment(p);
    }

    _put_byte(q, 0);

    return dst;
}

// Instantiate EmMem_strcat's that work with:
//
//			dest		source
//			------		------
//			char*		emuptr
//			emuptr 		char*
//			emuptr 		const char*
//			emuptr 		emuptr

template char* EmMem_strcat<char*, emuptr>(char* dst, emuptr src);
template emuptr EmMem_strcat<emuptr, char*>(emuptr dst, char* src);
template emuptr EmMem_strcat<emuptr, const char*>(emuptr dst, const char* src);
template emuptr EmMem_strcat<emuptr, emuptr>(emuptr dst, emuptr src);

/***********************************************************************
 *
 * FUNCTION:	EmMem_strncat
 *
 * DESCRIPTION: Same as Std C Library strncat function.
 *
 * PARAMETERS:	Same as Std C Library strncat function.
 *
 * RETURNED:	Same as Std C Library strncat function.
 *
 ***********************************************************************/

template <class T1, class T2>
T1 EmMem_strncat(T1 dst, T2 src, size_t len) {
    T1 q = dst;
    T2 p = src;

    while (_get_byte(q)) _increment(q);

    ++len;

    while (--len) {
        char ch = _get_byte(p);
        _increment(p);

        _put_byte(q, ch);
        _increment(q);

        if (!ch) return dst;
    }

    _put_byte(q, 0);

    return dst;
}

// Instantiate EmMem_strncat's that work with:
//
//			dest		source
//			------		------
//			char*		emuptr
//			emuptr 		char*
//			emuptr 		const char*
//			emuptr 		emuptr

template char* EmMem_strncat<char*, emuptr>(char* dst, emuptr src, size_t len);
template emuptr EmMem_strncat<emuptr, char*>(emuptr dst, char* src, size_t len);
template emuptr EmMem_strncat<emuptr, const char*>(emuptr dst, const char* src, size_t len);
template emuptr EmMem_strncat<emuptr, emuptr>(emuptr dst, emuptr src, size_t len);

/***********************************************************************
 *
 * FUNCTION:	EmMem_strcmp
 *
 * DESCRIPTION: Same as Std C Library strcmp function.
 *
 * PARAMETERS:	Same as Std C Library strcmp function.
 *
 * RETURNED:	Same as Std C Library strcmp function.
 *
 ***********************************************************************/

template <class T1, class T2>
int EmMem_strcmp(T1 dst, T2 src) {
    T1 p1 = dst;
    T2 p2 = src;

    while (1) {
        unsigned char c1 = _get_byte(p1);
        unsigned char c2 = _get_byte(p2);

        if (c1 != c2)
            return (c1 - c2);
        else if (!c1)
            break;

        _increment(p1);
        _increment(p2);
    }

    return 0;
}

// Instantiate EmMem_strcmp's that work with:
//
//			dest		source
//			------		------
//			char*		emuptr
//			const char* emuptr
//			emuptr 		char*
//			emuptr 		const char*
//			emuptr 		emuptr

template int EmMem_strcmp<char*, emuptr>(char* dst, emuptr src);
template int EmMem_strcmp<const char*, emuptr>(const char* dst, emuptr src);
template int EmMem_strcmp<emuptr, char*>(emuptr dst, char* src);
template int EmMem_strcmp<emuptr, const char*>(emuptr dst, const char* src);
template int EmMem_strcmp<emuptr, emuptr>(emuptr dst, emuptr src);

/***********************************************************************
 *
 * FUNCTION:	EmMem_strncmp
 *
 * DESCRIPTION: Same as Std C Library strncmp function.
 *
 * PARAMETERS:	Same as Std C Library strncmp function.
 *
 * RETURNED:	Same as Std C Library strncmp function.
 *
 ***********************************************************************/

template <class T1, class T2>
int EmMem_strncmp(T1 dst, T2 src, size_t len) {
    T1 p1 = dst;
    T2 p2 = src;

    ++len;

    while (--len) {
        unsigned char c1 = _get_byte(p1);
        unsigned char c2 = _get_byte(p2);

        if (c1 != c2)
            return (c1 - c2);
        else if (!c1)
            break;

        _increment(p1);
        _increment(p2);
    }

    return 0;
}

// Instantiate EmMem_strncmp's that work with:
//
//			dest		source
//			------		------
//			char*		emuptr
//			const char* emuptr
//			emuptr 		char*
//			emuptr 		const char*
//			emuptr 		emuptr

template int EmMem_strncmp<char*, emuptr>(char* dst, emuptr src, size_t len);
template int EmMem_strncmp<const char*, emuptr>(const char* dst, emuptr src, size_t len);
template int EmMem_strncmp<emuptr, char*>(emuptr dst, char* src, size_t len);
template int EmMem_strncmp<emuptr, const char*>(emuptr dst, const char* src, size_t len);
template int EmMem_strncmp<emuptr, emuptr>(emuptr dst, emuptr src, size_t len);
