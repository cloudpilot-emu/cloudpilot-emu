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

#include "EmBankRegs.h"

#include "EmCPU.h"     // GetPC
#include "EmCPU68K.h"  // gCPU68K
#include "EmCommon.h"
#include "EmMemory.h"   // gMemAccessFlags, EmMemory::IsPCInRAM
#include "EmSession.h"  // GetDevice, ScheduleDeferredError
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

/*
        When emulating memory, UAE divides up the 4GB address space into
        65,536 banks, each of which is 64K long.  For more details, see the
        comments in EmMemory.cpp.

        For our purposes, we sometimes need to divide up each 64K bank in a
        similar fashion.  For instance, in order to support accesses to each
        of the Dragonball registers, we'd like to assign a handler to each
        byte, word, or long of memory in the bank that contains the
        Dragonball registers.

        This module supports that further subdivision.  EmBankRegs is a bank
        handler -- just like the RAM and ROM bank handlers -- in that it is
        responsible for memory accesses to one or more 64K banks of memory.
        It divides up each 64K bank by making use of instances of EmRegs
        subclasses.

        EmRegs subclasses are the moral equivalent of EmBanks, except on a
        smaller scale.  They are responsible for byte, word, and long memory
        accesses to a small range of memory completely contained within a
        64K bank.

        Many such EmRegs instances can be created and installed into the
        EmBankRegs object.  EmBankRegs stores these EmRegs in a list. When
        in comes time for the EmBanks to register which 64K banks they are
        responsible for, EmBankRegs iterates over all the EmRegs objects
        registered with it, and takes responsibility for each 64K bank that
        contains the EmRegs object memory range (an EmRegs object can be
        queried as to the memory start and length it is responsible for).

        Later, when a memory access is made to any of the 64K banks
        EmBankRegs registered for, it again iterates over the EmRegs objects
        installed into it, looking for one that takes responsibility for the
        memory address being accessed.  When it finds one, it passes off the
        request to that object.  Otherwise, it signals a bus error.
*/

static EmAddressBank gAddressBank = {EmBankRegs::GetLong,
                                     EmBankRegs::GetWord,
                                     EmBankRegs::GetByte,
                                     EmBankRegs::SetLong,
                                     EmBankRegs::SetWord,
                                     EmBankRegs::SetByte,
                                     EmBankRegs::GetRealAddress,
                                     EmBankRegs::ValidAddress,
                                     NULL,
                                     NULL};

EmRegsList EmBankRegs::fgSubBanks;
EmRegsList EmBankRegs::fgDisabledSubBanks;

static EmRegs* gLastSubBank;
static uint64 gLastStart;
static uint32 gLastRange;

static void PrvSwitchBanks(EmRegsList& fromList, EmRegsList& toList, emuptr address);

#pragma mark -

/***********************************************************************
 *
 * FUNCTION:	EmBankRegs::Initialize
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

void EmBankRegs::Initialize(void) {
    EmAssert(gSession);
    gSession->GetDevice().CreateRegs();

    EmRegsList::iterator iter = fgSubBanks.begin();
    while (iter != fgSubBanks.end()) {
        (*iter)->Initialize();
        ++iter;
    }

    // Install the handlers for each affected bank.

    EmBankRegs::SetBankHandlers();
}

/***********************************************************************
 *
 * FUNCTION:	EmBankRegs::Reset
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

void EmBankRegs::Reset(Bool hardwareReset) {
    EmRegsList::iterator iter = fgSubBanks.begin();
    while (iter != fgSubBanks.end()) {
        (*iter)->Reset(hardwareReset);
        ++iter;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmBankRegs::Save
 *
 * DESCRIPTION: Standard save function.  Saves any sub-system state to
 *				the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

template <typename T>
void EmBankRegs::Save(T& savestate) {
    EmRegsList::iterator iter = fgSubBanks.begin();
    while (iter != fgSubBanks.end()) {
        (*iter)->Save(savestate);
        ++iter;
    }
}

template void EmBankRegs::Save(Savestate& savestate);
template void EmBankRegs::Save(SavestateProbe& savestate);

void EmBankRegs::Load(SavestateLoader& loader) {
    EmRegsList::iterator iter = fgSubBanks.begin();
    while (iter != fgSubBanks.end()) {
        (*iter)->Load(loader);
        ++iter;
    }

    gLastSubBank = NULL;
}

/***********************************************************************
 *
 * FUNCTION:	EmBankRegs::Dispose
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

void EmBankRegs::Dispose(void) {
    EmRegsList::iterator iter = fgSubBanks.begin();
    while (iter != fgSubBanks.end()) {
        (*iter)->Dispose();
        ++iter;
    }

    while (fgSubBanks.size() > 0) {
        EmRegs* bank = fgSubBanks[0];
        fgSubBanks.erase(fgSubBanks.begin());
        delete bank;
    }

    gLastSubBank = NULL;
}

/***********************************************************************
 *
 * FUNCTION:    EmBankRegs::SetBankHandlers
 *
 * DESCRIPTION: Set the bank handlers UAE uses to dispatch memory
 *				access operations.
 *
 * PARAMETERS:  None
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void EmBankRegs::SetBankHandlers(void) {
    EmRegsList::iterator iter = fgSubBanks.begin();
    while (iter != fgSubBanks.end()) {
        (*iter)->SetBankHandlers(gAddressBank);
        ++iter;
    }
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::GetLong
// ---------------------------------------------------------------------------

uint32 EmBankRegs::GetLong(emuptr address) {
#if (VALIDATE_REGISTER_GET)
    if (gMemAccessFlags.fValidate_RegisterGet && !ValidAddress(address, sizeof(uint32))) {
        EmBankRegs::InvalidAccess(address, sizeof(uint32), true);
    }
#endif

    EmRegs* bank = EmBankRegs::GetSubBank(address, sizeof(uint32));

    if (bank) {
#if (CHECK_FOR_ADDRESS_ERROR)
        if ((address & 1 && !bank->AllowUnalignedAccess(address, 4)) != 0) {
            AddressError(address, sizeof(uint32), true);
            address &= ~1;
        }
#endif

        return bank->GetLong(address);
    }

    EmBankRegs::InvalidAccess(address, sizeof(uint32), true);
    return ~0;
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::GetWord
// ---------------------------------------------------------------------------

uint32 EmBankRegs::GetWord(emuptr address) {
#if (VALIDATE_REGISTER_GET)
    if (gMemAccessFlags.fValidate_RegisterGet && !ValidAddress(address, sizeof(uint16))) {
        EmBankRegs::InvalidAccess(address, sizeof(uint16), true);
    }
#endif

    EmRegs* bank = EmBankRegs::GetSubBank(address, sizeof(uint16));

    if (bank) {
#if (CHECK_FOR_ADDRESS_ERROR)
        if ((address & 1 && !bank->AllowUnalignedAccess(address, 2)) != 0) {
            AddressError(address, sizeof(uint16), true);
            address &= ~1;
        }
#endif

        return bank->GetWord(address);
    }

    EmBankRegs::InvalidAccess(address, sizeof(uint16), true);
    return ~0;
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::GetByte
// ---------------------------------------------------------------------------

uint32 EmBankRegs::GetByte(emuptr address) {
#if (VALIDATE_REGISTER_GET)
    if (gMemAccessFlags.fValidate_RegisterGet && !ValidAddress(address, sizeof(uint8))) {
        EmBankRegs::InvalidAccess(address, sizeof(uint8), true);
    }
#endif

    EmRegs* bank = EmBankRegs::GetSubBank(address, sizeof(uint8));

    if (bank) {
        return bank->GetByte(address);
    }

    EmBankRegs::InvalidAccess(address, sizeof(uint8), true);
    return ~0;
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::SetLong
// ---------------------------------------------------------------------------

void EmBankRegs::SetLong(emuptr address, uint32 value) {
#if (VALIDATE_REGISTER_SET)
    if (gMemAccessFlags.fValidate_RegisterSet && !ValidAddress(address, sizeof(uint32))) {
        EmBankRegs::InvalidAccess(address, sizeof(uint32), false);
    }
#endif

    EmRegs* bank = EmBankRegs::GetSubBank(address, sizeof(uint32));

    if (bank) {
#if (CHECK_FOR_ADDRESS_ERROR)
        if ((address & 1 && !bank->AllowUnalignedAccess(address, 4)) != 0) {
            AddressError(address, sizeof(uint32), false);
            address &= ~1;
        }
#endif

        bank->SetLong(address, value);

        return;
    }

    EmBankRegs::InvalidAccess(address, sizeof(uint32), false);
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::SetWord
// ---------------------------------------------------------------------------

void EmBankRegs::SetWord(emuptr address, uint32 value) {
#if (VALIDATE_REGISTER_SET)
    if (gMemAccessFlags.fValidate_RegisterSet && !ValidAddress(address, sizeof(uint16))) {
        EmBankRegs::InvalidAccess(address, sizeof(uint16), false);
    }
#endif

    EmRegs* bank = EmBankRegs::GetSubBank(address, sizeof(uint16));

    if (bank) {
#if (CHECK_FOR_ADDRESS_ERROR)
        if ((address & 1 && !bank->AllowUnalignedAccess(address, 2)) != 0) {
            AddressError(address, sizeof(uint16), false);
            address &= ~1;
        }
#endif

        bank->SetWord(address, value);

        return;
    }

    EmBankRegs::InvalidAccess(address, sizeof(uint16), false);
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::SetByte
// ---------------------------------------------------------------------------

void EmBankRegs::SetByte(emuptr address, uint32 value) {
#if (VALIDATE_REGISTER_SET)
    if (gMemAccessFlags.fValidate_RegisterSet && !ValidAddress(address, sizeof(uint8))) {
        EmBankRegs::InvalidAccess(address, sizeof(uint8), false);
    }
#endif

    EmRegs* bank = EmBankRegs::GetSubBank(address, sizeof(uint8));

    if (bank) {
        bank->SetByte(address, value);

        return;
    }

    EmBankRegs::InvalidAccess(address, sizeof(uint8), false);
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::ValidAddress
// ---------------------------------------------------------------------------

int EmBankRegs::ValidAddress(emuptr address, uint32 size) {
    EmRegs* bank = EmBankRegs::GetSubBank(address, size);

    if (bank) {
        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmBankRegs::GetRealAddress(emuptr address) {
    EmRegs* bank = EmBankRegs::GetSubBank(address, 0);

    if (bank) {
        return bank->GetRealAddress(address);
    }

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::AddSubBank
// ---------------------------------------------------------------------------

void EmBankRegs::AddSubBank(EmRegs* bank) {
    fgSubBanks.push_back(bank);
    gLastSubBank = NULL;
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::EnableSubBank
// ---------------------------------------------------------------------------

void EmBankRegs::EnableSubBank(emuptr address) {
    PrvSwitchBanks(fgDisabledSubBanks, fgSubBanks, address);
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::DisableSubBank
// ---------------------------------------------------------------------------

void EmBankRegs::DisableSubBank(emuptr address) {
    PrvSwitchBanks(fgSubBanks, fgDisabledSubBanks, address);
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::GetSubBank
// ---------------------------------------------------------------------------

EmRegs* EmBankRegs::GetSubBank(emuptr address, long size) {
    // Cast address to a 64-bit value in case address + size == 0x1 0000 0000.

    uint64 addrStart64 = address;
    uint64 addrEnd64 = addrStart64 + size;

    // This cache is a big win.  When emulating a Palm IIIc (which uses
    // three EmRegs-based objects: EmRegsEZPalmIIIc, EmRegsSED1375, and
    // EmRegsFrameBuffer) and doing a Gremlins run, the cache was hit
    // 2,356,670 times and missed 19456 times.  When doing the same run
    // on a Palm V, the cache was missed only the first time.

    if (gLastSubBank && (addrStart64 >= gLastStart) && (addrEnd64 <= gLastStart + gLastRange)) {
        return gLastSubBank;
    }

    EmRegsList::iterator iter = fgSubBanks.begin();
    while (iter != fgSubBanks.end()) {
        uint64 start = (*iter)->GetAddressStart();
        uint32 range = (*iter)->GetAddressRange();

        if ((addrStart64 >= start) && (addrEnd64 <= start + range)) {
            gLastSubBank = *iter;
            gLastStart = start;
            gLastRange = range;

            return *iter;
        }

        ++iter;
    }

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::AddressError
// ---------------------------------------------------------------------------

void EmBankRegs::AddressError(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->AddressError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� EmBankRegs::InvalidAccess
// ---------------------------------------------------------------------------

void EmBankRegs::InvalidAccess(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->BusError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� PrvSwitchBanks
// ---------------------------------------------------------------------------

void PrvSwitchBanks(EmRegsList& fromList, EmRegsList& toList, emuptr address) {
    EmRegsList::iterator iter = fromList.begin();
    while (iter != fromList.end()) {
        uint64 start = (*iter)->GetAddressStart();
        uint32 range = (*iter)->GetAddressRange();

        if ((address >= start) && (address <= start + range)) {
            toList.push_back(*iter);
            fromList.erase(iter);
            return;
        }

        ++iter;
    }
}
