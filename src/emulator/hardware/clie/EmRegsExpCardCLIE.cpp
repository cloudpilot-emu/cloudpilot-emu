/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2000 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmRegsExpCardCLIE.h"

#include "Byteswapping.h"  // ByteswapWords
#include "DebugMgr.h"      // Debug::CheckStepSpy
#include "EmBankROM.h"     // ROMBank::IsPCInRAM
#include "EmBankRegs.h"    // RegsBank::GetROMSize
#include "EmCommon.h"
#include "EmHAL.h"
#include "EmMemory.h"  // gMemoryAccess

#define WAITSTATES_USB 0

typedef uint32 (*ReadFunction)(emuptr address, int size);
typedef void (*WriteFunction)(emuptr address, int size, uint32 value);

//*******************************************************************
// EmRegsExpCardCLIE Class
//*******************************************************************

// Macro to return the Dragonball address of the specified register

#undef addressof
#define addressof(x) (this->GetAddressStart() + offsetof(HwrExpCardCLIEType, x))

#define INSTALL_HANDLER(read, write, reg)                    \
    this->SetHandler((ReadFunction)&EmRegsExpCardCLIE::read, \
                     (WriteFunction)&EmRegsExpCardCLIE::write, addressof(reg), sizeof(fRegs.reg))
#pragma mark -

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::EmRegsExpCardCLIE
// ---------------------------------------------------------------------------

EmRegsExpCardCLIE::EmRegsExpCardCLIE() {
    fAddressOffset = 0;
    fBaseAddr = 0;
}

EmRegsExpCardCLIE::EmRegsExpCardCLIE(emuptr baseAddr) {
    fAddressOffset = 0;
    fBaseAddr = baseAddr;
}

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::~EmRegsExpCardCLIE
// ---------------------------------------------------------------------------

EmRegsExpCardCLIE::~EmRegsExpCardCLIE(void) {}

/***********************************************************************
 *
 * FUNCTION:	EmRegsExpCardCLIE::Initialize
 *
 * DESCRIPTION: Standard initialization function.  Responsible for
 *				initializing this sub-system when a new session is
 *				created.  May also be called from the Load function
 *				to share common functionality.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmRegsExpCardCLIE::Initialize(void) { EmRegs::Initialize(); }

/***********************************************************************
 *
 * FUNCTION:	EmRegsExpCardCLIE::Reset
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

void EmRegsExpCardCLIE::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);
    if (hardwareReset) {
        memset(&fRegs, 0, sizeof(fRegs));
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmRegsExpCardCLIE::Dispose
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

void EmRegsExpCardCLIE::Dispose(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::SetSubBankHandlers
// ---------------------------------------------------------------------------
void EmRegsExpCardCLIE::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(Read, Write, dataWrite);
    INSTALL_HANDLER(Read, Write, dataRead);
    INSTALL_HANDLER(Read, Write, cmdWrite);
    INSTALL_HANDLER(Read, Write, cmdRead);

    INSTALL_HANDLER(Read, Write, ExpCard0008);
    INSTALL_HANDLER(Read, Write, ExpCard0009);

    INSTALL_HANDLER(ReadFromDummy, WrireToDummy, _filler01);
    INSTALL_HANDLER(ReadFromDummy, WrireToDummy, _filler02);
}

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsExpCardCLIE::GetRealAddress(emuptr address) {
    uint8* loc = ((uint8*)&fRegs) + (address - this->GetAddressStart());
    return loc;
}

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsExpCardCLIE::GetAddressStart(void) {
    if (fBaseAddr) return fBaseAddr + fAddressOffset;

    return EmBankROM::GetMemoryStart() + EmHAL::GetROMSize() + fAddressOffset;
}

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsExpCardCLIE::GetAddressRange(void) { return sizeof(fRegs); }

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::Write
// ---------------------------------------------------------------------------
void EmRegsExpCardCLIE::Write(emuptr address, int size, uint32 value) {
    if ((value & 0x000000FF) == 0xFB && address == (this->GetAddressStart() + 2)) {
        extern uint8* gRAM_Memory;
        fRegs.dataRead = 0xC0;
    }

    this->StdWriteBE(address, size, value);
}

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::Read
// ---------------------------------------------------------------------------
uint32 EmRegsExpCardCLIE::Read(emuptr address, int size) {
    uint32 rstValue = this->StdReadBE(address, size);
    return rstValue;
}

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::WrireToDummy
// ---------------------------------------------------------------------------
void EmRegsExpCardCLIE::WrireToDummy(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    this->StdWriteBE(address, size, value);

    value &= 0x0FF;
}

// ---------------------------------------------------------------------------
//		� EmRegsExpCardCLIE::ReadFromDummy
// ---------------------------------------------------------------------------
uint32 EmRegsExpCardCLIE::ReadFromDummy(emuptr address, int size) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    return this->StdReadBE(address, size);
}

// ---------------------------------------------------------------------------
//		� USBSonyReg::UnsupportedWrite
// ---------------------------------------------------------------------------

void EmRegsExpCardCLIE::UnsupportedWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(value)

    return;
}

#pragma mark -
