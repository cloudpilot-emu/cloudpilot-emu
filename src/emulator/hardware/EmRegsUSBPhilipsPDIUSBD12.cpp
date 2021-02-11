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

#include "EmRegsUSBPhilipsPDIUSBD12.h"

#include "EmCommon.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

// Given a register (specified by its field name), return its address
// in emulated space.

#undef addressof
#define addressof(reg) (this->GetAddressStart() + fRegs.offsetof_##reg())

// Macro to help the installation of handlers for a register.

#undef INSTALL_HANDLER
#define INSTALL_HANDLER(read, write, reg)                                              \
    this->SetHandler((ReadFunction)&EmRegsUSBPhilipsPDIUSBD12::read,                   \
                     (WriteFunction)&EmRegsUSBPhilipsPDIUSBD12::write, addressof(reg), \
                     fRegs.reg.GetSize())

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::EmRegsUSBPhilipsPDIUSBD12
// ---------------------------------------------------------------------------

EmRegsUSBPhilipsPDIUSBD12::EmRegsUSBPhilipsPDIUSBD12(emuptr baseAddr) : fBaseAddr(baseAddr) {}

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::~EmRegsUSBPhilipsPDIUSBD12
// ---------------------------------------------------------------------------

EmRegsUSBPhilipsPDIUSBD12::~EmRegsUSBPhilipsPDIUSBD12(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::Initialize
// ---------------------------------------------------------------------------

void EmRegsUSBPhilipsPDIUSBD12::Initialize(void) { EmRegs::Initialize(); }

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::Reset
// ---------------------------------------------------------------------------

void EmRegsUSBPhilipsPDIUSBD12::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);

    if (hardwareReset) {
        memset(fRegs.GetPtr(), 0, fRegs.GetSize());
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::Dispose
// ---------------------------------------------------------------------------

void EmRegsUSBPhilipsPDIUSBD12::Dispose(void) { EmRegs::Dispose(); }

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsUSBPhilipsPDIUSBD12::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(StdReadBE, StdWriteBE, data);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, command);
}

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsUSBPhilipsPDIUSBD12::GetRealAddress(emuptr address) {
    return (uint8*)fRegs.GetPtr() + address - this->GetAddressStart();
}

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsUSBPhilipsPDIUSBD12::GetAddressStart(void) { return fBaseAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsUSBPhilipsPDIUSBD12::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsUSBPhilipsPDIUSBD12::GetAddressRange(void) { return fRegs.GetSize(); }
