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

#include "EmBankDummy.h"

#include "EmBankDRAM.h"  // EmBankDRAM::ValidAddress
#include "EmBankSRAM.h"  // EmBankSRAM::GetMemoryStart
#include "EmCPU68K.h"    // gCPU68K
#include "EmCommon.h"
#include "EmMemory.h"  // Memory::InitializeBanks

// ===========================================================================
//		� Dummy Bank Accessors
// ===========================================================================
// Dummy banks are non-existent blocks of memory.  Dummy bank accessors do
// not do anything.

static EmAddressBank gAddressBank = {EmBankDummy::GetLong,        EmBankDummy::GetWord,
                                     EmBankDummy::GetByte,        EmBankDummy::SetLong,
                                     EmBankDummy::SetWord,        EmBankDummy::SetByte,
                                     EmBankDummy::GetRealAddress, EmBankDummy::ValidAddress,
                                     EmBankDummy::GetMetaAddress, EmBankDummy::AddOpcodeCycles};

inline Bool HackForHwrGetRAMSize(emuptr address) {
    //	if ((address & 0xFF000000) == EmBankSRAM::GetMemoryStart ())
    if (address == EmBankSRAM::GetMemoryStart() + gRAMBank_Size) return true;

    return false;
}

/***********************************************************************
 *
 * FUNCTION:	EmBankDummy::Initialize
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

void EmBankDummy::Initialize(void) {}

/***********************************************************************
 *
 * FUNCTION:	EmBankDummy::Reset
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

void EmBankDummy::Reset(Bool /*hardwareReset*/) {}

/***********************************************************************
 *
 * FUNCTION:	EmBankDummy::Save
 *
 * DESCRIPTION:	Standard save function.  Saves any sub-system state to
 *				the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankDummy::Save(SessionFile&) {}

/***********************************************************************
 *
 * FUNCTION:	EmBankDummy::Load
 *
 * DESCRIPTION:	Standard load function.  Loads any sub-system state
 *				from the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankDummy::Load(SessionFile&) {}

/***********************************************************************
 *
 * FUNCTION:	EmBankDummy::Dispose
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

void EmBankDummy::Dispose(void) {}

/***********************************************************************
 *
 * FUNCTION:    EmBankDummy::SetBankHandlers
 *
 * DESCRIPTION: Set the bank handlers UAE uses to dispatch memory
 *				access operations.
 *
 * PARAMETERS:  None
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void EmBankDummy::SetBankHandlers(void) { Memory::InitializeBanks(gAddressBank, 0, 0xFFFF); }

// ---------------------------------------------------------------------------
//		� EmBankDummy::GetLong
// ---------------------------------------------------------------------------

uint32 EmBankDummy::GetLong(emuptr address) {
    // Hack to keep HwrGetRAMSize working: it runs off
    // the end of RAM while testing it.

    if (HackForHwrGetRAMSize(address)) return 0;

    InvalidAccess(address, sizeof(uint32), true);
    return ~0;
}

// ---------------------------------------------------------------------------
//		� EmBankDummy::GetWord
// ---------------------------------------------------------------------------

uint32 EmBankDummy::GetWord(emuptr address) {
    // Hack to keep HwrGetRAMSize working: it runs off
    // the end of RAM while testing it.

    if (HackForHwrGetRAMSize(address)) return 0;

    InvalidAccess(address, sizeof(uint16), true);
    return ~0;
}

// ---------------------------------------------------------------------------
//		� EmBankDummy::GetByte
// ---------------------------------------------------------------------------

uint32 EmBankDummy::GetByte(emuptr address) {
    // Hack to keep HwrGetRAMSize working: it runs off
    // the end of RAM while testing it.

    if (HackForHwrGetRAMSize(address)) return 0;

    InvalidAccess(address, sizeof(uint8), true);
    return ~0;
}

// ---------------------------------------------------------------------------
//		� EmBankDummy::SetLong
// ---------------------------------------------------------------------------

void EmBankDummy::SetLong(emuptr address, uint32) {
    // Hack to keep HwrGetRAMSize working: it runs off
    // the end of RAM while testing it.

    if (HackForHwrGetRAMSize(address)) return;

    InvalidAccess(address, sizeof(uint32), true);
}

// ---------------------------------------------------------------------------
//		� EmBankDummy::SetWord
// ---------------------------------------------------------------------------

void EmBankDummy::SetWord(emuptr address, uint32) {
    // Hack to keep HwrGetRAMSize working: it runs off
    // the end of RAM while testing it.

    if (HackForHwrGetRAMSize(address)) return;

    InvalidAccess(address, sizeof(uint16), true);
}

// ---------------------------------------------------------------------------
//		� EmBankDummy::SetByte
// ---------------------------------------------------------------------------

void EmBankDummy::SetByte(emuptr address, uint32) {
    // Hack to keep HwrGetRAMSize working: it runs off
    // the end of RAM while testing it.

    if (HackForHwrGetRAMSize(address)) return;

    InvalidAccess(address, sizeof(uint8), true);
}

// ---------------------------------------------------------------------------
//		� EmBankDummy::ValidAddress
// ---------------------------------------------------------------------------

int EmBankDummy::ValidAddress(emuptr, uint32) {
    int result = false;

    return result;
}

// ---------------------------------------------------------------------------
//		� EmBankDummy::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmBankDummy::GetRealAddress(emuptr address) { return nullptr; }

// ---------------------------------------------------------------------------
//		� EmBankDummy::GetMetaAddress
// ---------------------------------------------------------------------------

uint8* EmBankDummy::GetMetaAddress(emuptr address) {
    UNUSED_PARAM(address)

    static uint8 dummyBits[4] = {0, 0, 0, 0};

    return dummyBits;
}

// ---------------------------------------------------------------------------
//		� EmBankDummy::AddOpcodeCycles
// ---------------------------------------------------------------------------

void EmBankDummy::AddOpcodeCycles(void) {}

// ---------------------------------------------------------------------------
//		� EmBankDummy::InvalidAccess
// ---------------------------------------------------------------------------

void EmBankDummy::InvalidAccess(emuptr address, long size, Bool forRead) {
    if (CEnableFullAccess::AccessOK()) return;

    EmAssert(gCPU68K);
    gCPU68K->BusError(address, size, forRead);
}
