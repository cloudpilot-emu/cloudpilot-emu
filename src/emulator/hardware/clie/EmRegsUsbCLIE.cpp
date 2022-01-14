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

#include "EmRegsUsbCLIE.h"

#include "Byteswapping.h"  // ByteswapWords
#include "DebugMgr.h"      // Debug::CheckStepSpy
#include "EmBankROM.h"     // ROMBank::IsPCInRAM
#include "EmBankRegs.h"    // RegsBank::GetROMSize
#include "EmCommon.h"
#include "EmHAL.h"
#include "EmMemory.h"  // gMemoryAccess

typedef uint32 (*ReadFunction)(emuptr address, int size);
typedef void (*WriteFunction)(emuptr address, int size, uint32 value);

//*******************************************************************
// EmRegsUsbCLIE Class
//*******************************************************************

// Macro to return the Dragonball address of the specified register

#define addressof(x) (this->GetAddressStart() + offsetof(HwrUsbCLIEType, x))

#define INSTALL_HANDLER(read, write, reg)                                                      \
    this->SetHandler((ReadFunction)&EmRegsUsbCLIE::read, (WriteFunction)&EmRegsUsbCLIE::write, \
                     addressof(reg), sizeof(fRegs.reg))
#pragma mark -

// ---------------------------------------------------------------------------
//		� EmRegsUsbCLIE::EmRegsUsbCLIE
// ---------------------------------------------------------------------------

EmRegsUsbCLIE::EmRegsUsbCLIE() {
    fOffsetAddr = 0;
    fBaseAddr = 0;
}

EmRegsUsbCLIE::EmRegsUsbCLIE(uint32 offset) {
    fOffsetAddr = offset;
    fBaseAddr = 0;
}

EmRegsUsbCLIE::EmRegsUsbCLIE(emuptr baseAddr, uint32 offset) {
    fOffsetAddr = offset;
    fBaseAddr = baseAddr;
}

// ---------------------------------------------------------------------------
//		� EmRegsUsbCLIE::~EmRegsUsbCLIE
// ---------------------------------------------------------------------------

EmRegsUsbCLIE::~EmRegsUsbCLIE(void) {}

/***********************************************************************
 *
 * FUNCTION:	EmRegsUsbCLIE::Initialize
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

void EmRegsUsbCLIE::Initialize(void) { EmRegs::Initialize(); }

/***********************************************************************
 *
 * FUNCTION:	EmRegsUsbCLIE::Reset
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

void EmRegsUsbCLIE::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);
    if (hardwareReset) {
        memset(&fRegs, 0, sizeof(fRegs));
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmRegsUsbCLIE::Dispose
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

void EmRegsUsbCLIE::Dispose(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsUsbCLIE::SetSubBankHandlers
// ---------------------------------------------------------------------------
void EmRegsUsbCLIE::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(Read, Write, dataWrite);
    INSTALL_HANDLER(Read, Write, dataRead);
    INSTALL_HANDLER(Read, Write, cmdWrite);
    INSTALL_HANDLER(Read, Write, cmdRead);
}

// ---------------------------------------------------------------------------
//		� EmRegsUsbCLIE::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsUsbCLIE::GetRealAddress(emuptr address) {
    uint8* loc = ((uint8*)&fRegs) + (address - this->GetAddressStart());
    return loc;
}

// ---------------------------------------------------------------------------
//		� EmRegsUsbCLIE::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsUsbCLIE::GetAddressStart(void) {
    emuptr startAddr = fBaseAddr;
    if (!startAddr) startAddr = EmBankROM::GetMemoryStart() + EmHAL::GetROMSize();
    return startAddr + fOffsetAddr;
}

// ---------------------------------------------------------------------------
//		� EmRegsUsbCLIE::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsUsbCLIE::GetAddressRange(void) { return sizeof(fRegs); }

// ---------------------------------------------------------------------------
//		� EmRegsUsbCLIE::Write
// ---------------------------------------------------------------------------
void EmRegsUsbCLIE::Write(emuptr address, int size, uint32 value) {
    if ((value & 0x000000FF) == 0xFB && address == (this->GetAddressStart() + 2)) {
        fRegs.dataRead = 0xC0;
    }

    this->StdWriteBE(address, size, value);
}

// ---------------------------------------------------------------------------
//		� EmRegsUsbCLIE::Read
// ---------------------------------------------------------------------------
uint32 EmRegsUsbCLIE::Read(emuptr address, int size) {
    uint32 rstValue = this->StdReadBE(address, size);
    return rstValue;
}

// ---------------------------------------------------------------------------
//		� USBSonyReg::UnsupportedWrite
// ---------------------------------------------------------------------------

void EmRegsUsbCLIE::UnsupportedWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(value)

    return;
}
