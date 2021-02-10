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

#include "EmBankSRAM.h"

#include "Byteswapping.h"  // ByteswapWords
#include "DebugMgr.h"      // Debug::CheckStepSpy
#include "EmCPU68K.h"      // gCPU68K
#include "EmCommon.h"
#include "EmMemory.h"   // gRAMBank_Size, gRAM_Memory, gMemoryAccess
#include "EmSession.h"  // GetDevice
#include "EmSystemState.h"
#include "MetaMemory.h"  // MetaMemory::
#include "Platform.h"

#if 0                           //  CSTODO
    #include "Miscellaneous.h"  // StWordSwapper
#endif

// ===========================================================================
//		� SRAM Bank Accessors
// ===========================================================================
// These functions provide fetch and store access to the emulator's random
// access memory.

static EmAddressBank gAddressBank = {EmBankSRAM::GetLong,        EmBankSRAM::GetWord,
                                     EmBankSRAM::GetByte,        EmBankSRAM::SetLong,
                                     EmBankSRAM::SetWord,        EmBankSRAM::SetByte,
                                     EmBankSRAM::GetRealAddress, EmBankSRAM::ValidAddress,
                                     EmBankSRAM::GetMetaAddress, EmBankSRAM::AddOpcodeCycles};

// Note: I'd've used hwrCardBase0 here, except that that
// changes on different hardware. It's 0x10000000 in some
// cases, 0x00000000 in others.

static const emuptr kMemoryStart = 0x10000000;
static const emuptr kMemoryStartEZ = 0x00000000;
static const emuptr kMemoryStartVZ = 0x00000000;

emuptr gMemoryStart;

uint32 gRAMBank_Size;
uint32 gRAMBank_Mask;
uint8* gRAM_Memory;
uint8* gRAM_MetaMemory;

uint8* gRAM_DirtyPages;

#if defined(_DEBUG)

// In debug mode, define a global variable that points to the
// Palm ROM's low-memory globals.  That makes it easier to find
// out what's going wrong when something goes wrong.
//
// Keep in mind that on Windows, all values are "wordswapped"
// (each pair of bytes is byteswapped).

LowMemHdrType* gLowMemory;

#endif

static inline uint8* InlineGetMetaAddress(emuptr address) {
    return (uint8*)&(gRAM_MetaMemory[address]);
}

static inline void markDirty(emuptr address) {
    gRAM_DirtyPages[address >> 13] |= (1 << ((address >> 10) & 0x07));
}

/***********************************************************************
 *
 * FUNCTION:	EmBankSRAM::Initialize
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

void EmBankSRAM::Initialize(RAMSizeType ramSize) {
    EmAssert(gRAM_Memory == NULL);
    EmAssert(gRAM_MetaMemory == NULL);

    if (ramSize > 0) {
        gRAMBank_Size = ramSize * 1024;
        gRAMBank_Mask = gRAMBank_Size - 1;
        gRAM_Memory = (uint8*)Platform::AllocateMemoryClear(gRAMBank_Size);
        gRAM_MetaMemory = (uint8*)Platform::AllocateMemoryClear(gRAMBank_Size);
        gRAM_DirtyPages = (uint8*)Platform::AllocateMemoryClear(gRAMBank_Size / 8192 +
                                                                (gRAMBank_Size % 8192 ? 1 : 0));

#if defined(_DEBUG)
        // In debug mode, define a global variable that points to the
        // Palm ROM's low-memory globals.  That makes it easier to find
        // out what's going wrong when something goes wrong.

        gLowMemory = (LowMemHdrType*)gRAM_Memory;
#endif
    }

    EmAssert(gSession);

    if (gSession->GetDevice().Supports68EZ328()) {
        gMemoryStart = kMemoryStartEZ;
    } else if (gSession->GetDevice().Supports68VZ328()) {
        gMemoryStart = kMemoryStartVZ;
    } else {
        gMemoryStart = kMemoryStart;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmBankSRAM::Reset
 *
 * DESCRIPTION: Standard reset function.  Sets the sub-system to a
 *				default state.	This occurs not only on a Reset (as
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

void EmBankSRAM::Reset(Bool /*hardwareReset*/) { memset(gRAM_MetaMemory, 0, gRAMBank_Size); }

/***********************************************************************
 *
 * FUNCTION:	EmBankSRAM::Dispose
 *
 * DESCRIPTION: Standard dispose function.	Completely release any
 *				resources acquired or allocated in Initialize and/or
 *				Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankSRAM::Dispose(void) {
    Platform::DisposeMemory(gRAM_Memory);
    Platform::DisposeMemory(gRAM_MetaMemory);
    Platform::DisposeMemory(gRAM_DirtyPages);
}

/***********************************************************************
 *
 * FUNCTION:    EmBankSRAM::SetBankHandlers
 *
 * DESCRIPTION: Set the bank handlers UAE uses to dispatch memory
 *				access operations.
 *
 * PARAMETERS:  None
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void EmBankSRAM::SetBankHandlers(void) {
    // Memory banks 0x1000 to <mumble> are managed by the functions
    // in EmBankSRAM.  <mumble> is based on the amount of RAM being emulated.
    long numBanks =
        EmMemBankIndex(gMemoryStart + gRAMBank_Size - 1) - EmMemBankIndex(gMemoryStart) + 1;

    // On the m515 PalmOS plays with the memory layout while it is detecting
    // memory size. We need to claim 32MB of the address space for this to work,
    // and the layout set up by writing to SDCTL will take care that only the
    // physical memory is actually accessed.
    if (gRAMBank_Size == 16 * 1024 * 1024) numBanks *= 2;

    Memory::InitializeBanks(gAddressBank, EmMemBankIndex(gMemoryStart), numBanks);
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::GetLong
// ---------------------------------------------------------------------------

uint32 EmBankSRAM::GetLong(emuptr address) {
#if (CHECK_FOR_ADDRESS_ERROR)
    if ((address & 1) != 0) {
        AddressError(address, sizeof(uint32), true);
    }
#endif

#if (PREVENT_USER_SRAM_GET)
    if (gMemAccessFlags.fProtect_SRAMGet) {
        ProtectedAccess(address, sizeof(uint32), true);
    }
#endif

#if (VALIDATE_SRAM_GET)
    if (gMemAccessFlags.fValidate_SRAMGet && !ValidAddress(address, sizeof(uint32))) {
        InvalidAccess(address, sizeof(uint32), true);
    }
#endif

    address &= gRAMBank_Mask;

    return EmMemDoGet32(gRAM_Memory + address);
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::GetWord
// ---------------------------------------------------------------------------

uint32 EmBankSRAM::GetWord(emuptr address) {
#if PROFILE_MEMORY
    gMemoryAccess[kSRAMWordRead]++;
#endif

#if (CHECK_FOR_ADDRESS_ERROR)
    if ((address & 1) != 0) {
        AddressError(address, sizeof(uint16), true);
    }
#endif

#if (PREVENT_USER_SRAM_GET)
    if (gMemAccessFlags.fProtect_SRAMGet) {
        ProtectedAccess(address, sizeof(uint16), true);
    }
#endif

#if (VALIDATE_SRAM_GET)
    if (gMemAccessFlags.fValidate_SRAMGet && !ValidAddress(address, sizeof(uint16))) {
        InvalidAccess(address, sizeof(uint16), true);
    }
#endif

    address &= gRAMBank_Mask;

    return EmMemDoGet16(gRAM_Memory + address);
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::GetByte
// ---------------------------------------------------------------------------

uint32 EmBankSRAM::GetByte(emuptr address) {
#if PROFILE_MEMORY
    gMemoryAccess[kSRAMByteRead]++;
#endif

#if (PREVENT_USER_SRAM_GET)
    if (gMemAccessFlags.fProtect_SRAMGet) {
        ProtectedAccess(address, sizeof(uint8), true);
    }
#endif

#if (VALIDATE_SRAM_GET)
    if (gMemAccessFlags.fValidate_SRAMGet && !ValidAddress(address, sizeof(uint8))) {
        InvalidAccess(address, sizeof(uint8), true);
    }
#endif

#if HAS_PROFILING
    CYCLE_GETBYTE(WAITSTATES_SRAM);
#endif

    address &= gRAMBank_Mask;

    return EmMemDoGet8(gRAM_Memory + address);
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::SetLong
// ---------------------------------------------------------------------------

void EmBankSRAM::SetLong(emuptr address, uint32 value) {
#if PROFILE_MEMORY
    gMemoryAccess[kSRAMLongWrite]++;
    if (address & 2) gMemoryAccess[kSRAMLongWrite2]++;
#endif

#if (CHECK_FOR_ADDRESS_ERROR)
    if ((address & 1) != 0) {
        AddressError(address, sizeof(uint32), false);
    }
#endif

#if (PREVENT_USER_SRAM_SET)
    if (gMemAccessFlags.fProtect_SRAMSet) {
        ProtectedAccess(address, sizeof(uint32), false);
    }
#endif

#if (VALIDATE_SRAM_SET)
    if (gMemAccessFlags.fValidate_SRAMSet && !ValidAddress(address, sizeof(uint32))) {
        InvalidAccess(address, sizeof(uint32), false);
    }
#endif

    emuptr phyAddress = address;
    phyAddress &= gRAMBank_Mask;

    // uint8* metaAddress = InlineGetMetaAddress(phyAddress);
    //	META_CHECK (metaAddress, address, SetLong, uint32, false);

    EmMemDoPut32(gRAM_Memory + phyAddress, value);

    markDirty(phyAddress);
    markDirty(phyAddress + 2);

    if (MetaMemory::IsScreenBuffer32(InlineGetMetaAddress(phyAddress)))
        gSystemState.MarkScreenDirty();

    // Debug::CheckStepSpy(address, sizeof(uint16));
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::SetWord
// ---------------------------------------------------------------------------

void EmBankSRAM::SetWord(emuptr address, uint32 value) {
#if (CHECK_FOR_ADDRESS_ERROR)
    if ((address & 1) != 0) {
        AddressError(address, sizeof(uint16), false);
    }
#endif

#if (PREVENT_USER_SRAM_SET)
    if (gMemAccessFlags.fProtect_SRAMSet) {
        ProtectedAccess(address, sizeof(uint16), false);
    }
#endif

#if (VALIDATE_SRAM_SET)
    if (gMemAccessFlags.fValidate_SRAMSet && !ValidAddress(address, sizeof(uint16))) {
        InvalidAccess(address, sizeof(uint16), false);
    }
#endif

    emuptr phyAddress = address;
    phyAddress &= gRAMBank_Mask;

    // uint8* metaAddress = InlineGetMetaAddress(phyAddress);
    //	META_CHECK (metaAddress, address, SetLong, uint16, false);

    EmMemDoPut16(gRAM_Memory + phyAddress, value);

    markDirty(phyAddress);

    if (MetaMemory::IsScreenBuffer16(InlineGetMetaAddress(phyAddress)))
        gSystemState.MarkScreenDirty();

    // Debug::CheckStepSpy(address, sizeof(uint16));
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::SetByte
// ---------------------------------------------------------------------------

void EmBankSRAM::SetByte(emuptr address, uint32 value) {
#if (PREVENT_USER_SRAM_SET)
    if (gMemAccessFlags.fProtect_SRAMSet) {
        ProtectedAccess(address, sizeof(uint8), false);
    }
#endif

#if (VALIDATE_SRAM_SET)
    if (gMemAccessFlags.fValidate_SRAMSet && !ValidAddress(address, sizeof(uint8))) {
        InvalidAccess(address, sizeof(uint8), false);
    }
#endif

    emuptr phyAddress = address;
    phyAddress &= gRAMBank_Mask;

    // uint8* metaAddress = InlineGetMetaAddress(phyAddress);
    //	META_CHECK (metaAddress, address, SetLong, uint8, false);

    EmMemDoPut8(gRAM_Memory + phyAddress, value);

    markDirty(phyAddress);

    if (MetaMemory::IsScreenBuffer8(InlineGetMetaAddress(phyAddress)))
        gSystemState.MarkScreenDirty();

    // Debug::CheckStepSpy(address, sizeof(uint8));
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::ValidAddress
// ---------------------------------------------------------------------------

int EmBankSRAM::ValidAddress(emuptr address, uint32 size) {
    address -= gMemoryStart;
    int result = (address + size) <= (gMemoryStart + gRAMBank_Size);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmBankSRAM::GetRealAddress(emuptr address) {
    // Strip the uppermost bit of the address.

    address &= gRAMBank_Mask;

    return (uint8*)&(gRAM_Memory[address]);
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::GetMetaAddress
// ---------------------------------------------------------------------------

uint8* EmBankSRAM::GetMetaAddress(emuptr address) {
    // Strip the uppermost bit of the address.

    address &= gRAMBank_Mask;

    return (uint8*)&(gRAM_MetaMemory[address]);
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::AddOpcodeCycles
// ---------------------------------------------------------------------------

void EmBankSRAM::AddOpcodeCycles(void) {
#if (HAS_PROFILING)
    CYCLE_GETWORD(WAITSTATES_SRAM);
#endif
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::AddressError
// ---------------------------------------------------------------------------

void EmBankSRAM::AddressError(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->AddressError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::InvalidAccess
// ---------------------------------------------------------------------------

void EmBankSRAM::InvalidAccess(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->BusError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� EmBankSRAM::ProtectedAccess
// ---------------------------------------------------------------------------

void EmBankSRAM::ProtectedAccess(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->BusError(address, size, forRead);
}
