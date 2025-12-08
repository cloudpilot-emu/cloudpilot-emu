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

// ---------------------------------------------------------------------------
#pragma mark ===== Includes
// ---------------------------------------------------------------------------

#include "EmBankDRAM.h"

#include "EmBankSRAM.h"  // gRAMBank_Size, gRAM_Memory, gMemoryAccess
#include "EmCPU.h"       // GetSP
#include "EmCPU68K.h"    // gCPU68K
#include "EmCommon.h"
#include "EmHAL.h"           // EmHAL
#include "EmMemory.h"        // Memory::InitializeBanks, IsPCInRAM (implicitly, through META_CHECK)
#include "EmPalmFunction.h"  // InSysLaunch
#include "EmSession.h"       // gSession
#include "EmSystemState.h"
#include "MemoryRegion.h"
#include "MetaMemory.h"  // MetaMemory

namespace {
    constexpr uint32 kMemoryStart = 0x00000000;

    uint32 dynamicHeapSize;

    EmAddressBank addressBank = {EmBankDRAM::GetLong,        EmBankDRAM::GetWord,
                                 EmBankDRAM::GetByte,        EmBankDRAM::SetLong,
                                 EmBankDRAM::SetWord,        EmBankDRAM::SetByte,
                                 EmBankDRAM::GetRealAddress, EmBankDRAM::ValidAddress,
                                 EmBankDRAM::GetMetaAddress, EmBankDRAM::AddOpcodeCycles};

    EmAddressBank addressBankDisabled = {EmBankDRAM::GetDummy,       EmBankDRAM::GetDummy,
                                         EmBankDRAM::GetDummy,       EmBankDRAM::SetDummy,
                                         EmBankDRAM::SetDummy,       EmBankDRAM::SetDummy,
                                         EmBankDRAM::GetRealAddress, EmBankDRAM::ValidAddress,
                                         EmBankDRAM::GetMetaAddress, EmBankDRAM::AddOpcodeCycles};

    uint32 ramSize;
    uint8* ram;
    uint8* dirtyPages;

    inline int InlineValidAddress(emuptr address, size_t size) {
        int result = (address + size) <= ramSize;

        return result;
    }

    inline uint8* InlineGetRealAddress(emuptr address) { return (uint8*)&(ram[address]); }

    inline uint8* InlineGetMetaAddress(emuptr address) {
        return (uint8*)&(gRAM_MetaMemory[address]);
    }

    inline void markDirty(emuptr address) {
        dirtyPages[address >> 13] |= (1 << ((address >> 10) & 0x07));
    }

}  // namespace

// ===========================================================================
//		� DRAM Bank Accessors
// ===========================================================================
// These functions provide fetch and store access to the emulator's random
// access memory.

/***********************************************************************
 *
 * FUNCTION:	EmBankDRAM::Initialize
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

void EmBankDRAM::Initialize(void) {
    ramSize = EmMemory::GetRegionSize(MemoryRegion::ram);
    ram = EmMemory::GetForRegion(MemoryRegion::ram);
    dirtyPages = EmMemory::GetDirtyPagesForRegion(MemoryRegion::ram);

    EmAssert(ramSize > 0);
    EmAssert(ram);
    EmAssert(dirtyPages);
}

/***********************************************************************
 *
 * FUNCTION:	EmBankDRAM::Reset
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

void EmBankDRAM::Reset(Bool /*hardwareReset*/) {}

/***********************************************************************
 *
 * FUNCTION:	EmBankDRAM::Dispose
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

void EmBankDRAM::Dispose(void) {}

/***********************************************************************
 *
 * FUNCTION:    EmBankDRAM::SetBankHandlers
 *
 * DESCRIPTION: Set the bank handlers UAE uses to dispatch memory
 *				access operations.
 *
 * PARAMETERS:  None
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void EmBankDRAM::SetBankHandlers(void) {
    // First few memory banks are managed by the functions in EmBankDRAM.

    dynamicHeapSize = EmHAL::GetDynamicHeapSize();

    if (dynamicHeapSize > ramSize) dynamicHeapSize = ramSize;

    uint32 sixtyFourK = 64 * 1024L;
    uint32 numBanks = (dynamicHeapSize + sixtyFourK - 1) / sixtyFourK;

    Memory::InitializeBanks(EmHAL::EnableRAM() ? addressBank : addressBankDisabled,
                            EmMemBankIndex(kMemoryStart), numBanks);
}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::GetLong
// ---------------------------------------------------------------------------

uint32 EmBankDRAM::GetLong(emuptr address) {
    if (address > dynamicHeapSize) return EmBankSRAM::GetLong(address);

    if (CHECK_FOR_ADDRESS_ERROR && (address & 1) != 0) {
        AddressError(address, sizeof(uint32), true);
        address &= ~1;
    }

    return EmMemDoGet32(ram + address);
}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::GetWord
// ---------------------------------------------------------------------------

uint32 EmBankDRAM::GetWord(emuptr address) {
    if (address > dynamicHeapSize) return EmBankSRAM::GetWord(address);

    if (CHECK_FOR_ADDRESS_ERROR && (address & 1) != 0) {
        AddressError(address, sizeof(uint16), true);
        address &= ~1;
    }

    return EmMemDoGet16(ram + address);
}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::GetByte
// ---------------------------------------------------------------------------

uint32 EmBankDRAM::GetByte(emuptr address) {
    if (address > dynamicHeapSize) return EmBankSRAM::GetByte(address);

    return EmMemDoGet8(ram + address);
}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::SetLong
// ---------------------------------------------------------------------------

void EmBankDRAM::SetLong(emuptr address, uint32 value) {
    if (address > dynamicHeapSize) {
        EmBankSRAM::SetLong(address, value);
        return;
    }

    if (CHECK_FOR_ADDRESS_ERROR && (address & 1) != 0) {
        AddressError(address, sizeof(uint32), false);
        address &= ~1;
    }

    EmMemDoPut32(ram + address, value);

    markDirty(address);
    markDirty(address + 2);

    if (MetaMemory::IsScreenBuffer32(InlineGetMetaAddress(address)))
        gSystemState.MarkScreenDirty(address, address + 4);
}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::SetWord
// ---------------------------------------------------------------------------

void EmBankDRAM::SetWord(emuptr address, uint32 value) {
    if (address > dynamicHeapSize) {
        EmBankSRAM::SetWord(address, value);
        address &= ~1;
        return;
    }

    if (CHECK_FOR_ADDRESS_ERROR && (address & 1) != 0) {
        AddressError(address, sizeof(uint16), false);
    }

    EmMemDoPut16(ram + address, value);

    markDirty(address);

    if (MetaMemory::IsScreenBuffer16(InlineGetMetaAddress(address)))
        gSystemState.MarkScreenDirty(address, address + 2);
}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::SetByte
// ---------------------------------------------------------------------------

void EmBankDRAM::SetByte(emuptr address, uint32 value) {
    if (address > dynamicHeapSize) {
        EmBankSRAM::SetByte(address, value);
        return;
    }

    EmMemDoPut8(ram + address, value);

    markDirty(address);

    if (MetaMemory::IsScreenBuffer8(InlineGetMetaAddress(address)))
        gSystemState.MarkScreenDirty(address, address);
}

uint32 EmBankDRAM::GetDummy(emuptr address) { return 0; }

void EmBankDRAM::SetDummy(emuptr address, uint32 value) {}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::ValidAddress
// ---------------------------------------------------------------------------

int EmBankDRAM::ValidAddress(emuptr address, uint32 size) {
    int result = InlineValidAddress(address, size);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmBankDRAM::GetRealAddress(emuptr address) { return InlineGetRealAddress(address); }

// ---------------------------------------------------------------------------
//		� EmBankDRAM::GetMetaAddress
// ---------------------------------------------------------------------------

uint8* EmBankDRAM::GetMetaAddress(emuptr address) { return InlineGetMetaAddress(address); }

// ---------------------------------------------------------------------------
//		� EmBankDRAM::AddOpcodeCycles
// ---------------------------------------------------------------------------

void EmBankDRAM::AddOpcodeCycles(void) {}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::AddressError
// ---------------------------------------------------------------------------

void EmBankDRAM::AddressError(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->AddressError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� EmBankDRAM::InvalidAccess
// ---------------------------------------------------------------------------

void EmBankDRAM::InvalidAccess(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->BusError(address, size, forRead);
}
