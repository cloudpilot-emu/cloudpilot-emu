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

#include "EmRegsPLDPalmVIIEZ.h"

#include "EmCommon.h"
#include "EmHAL.h"  // EmHAL::GetSerialPortOn

#define NON_PORTABLE
#include "EZSumo/IncsPrv/HardwareEZ.h"  // hwrJerryPldBase, hwrJerryPld232Enable
#undef NON_PORTABLE

// Given a register (specified by its field name), return its address
// in emulated space.

#define db_addressof(reg) (this->GetAddressStart() + fRegs.offsetof_##reg())

// Macro to help the installation of handlers for a register.

#define INSTALL_HANDLER(read, write, reg)                                          \
    this->SetHandler((ReadFunction)&EmRegsPLDPalmVIIEZ::read,                      \
                     (WriteFunction)&EmRegsPLDPalmVIIEZ::write, db_addressof(reg), \
                     fRegs.reg.GetSize())

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::EmRegsPLDPalmVIIEZ
// ---------------------------------------------------------------------------

EmRegsPLDPalmVIIEZ::EmRegsPLDPalmVIIEZ(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::~EmRegsPLDPalmVIIEZ
// ---------------------------------------------------------------------------

EmRegsPLDPalmVIIEZ::~EmRegsPLDPalmVIIEZ(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::Initialize
// ---------------------------------------------------------------------------

void EmRegsPLDPalmVIIEZ::Initialize(void) { EmRegs::Initialize(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::Reset
// ---------------------------------------------------------------------------

void EmRegsPLDPalmVIIEZ::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);

    if (hardwareReset) {
        memset(fRegs.GetPtr(), 0, fRegs.GetSize());
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::Dispose
// ---------------------------------------------------------------------------

void EmRegsPLDPalmVIIEZ::Dispose(void) { EmRegs::Dispose(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::GetSerialPortOn
// ---------------------------------------------------------------------------

Bool EmRegsPLDPalmVIIEZ::GetSerialPortOn(int /*uartNum*/) {
    return (fRegs.rs232Shdn & hwrJerryPld232Enable) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsPLDPalmVIIEZ::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(StdReadBE, StdWriteBE, dspOn);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, chargeOn);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, refOn);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, pipaBiasEnable);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, dspReset);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, ezToDsl);
    INSTALL_HANDLER(StdReadBE, rs232ShdnWrite, rs232Shdn);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, spareOut);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, dTo3Sln);
    INSTALL_HANDLER(iXtrnl2Read, StdWriteBE, iXtrnl2);
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsPLDPalmVIIEZ::GetRealAddress(emuptr address) {
    return (uint8*)fRegs.GetPtr() + address - this->GetAddressStart();
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsPLDPalmVIIEZ::GetAddressStart(void) { return hwrJerryPldBase; }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmVIIEZ::GetAddressRange(void) { return fRegs.GetSize(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::rs232ShdnWrite
// ---------------------------------------------------------------------------

void EmRegsPLDPalmVIIEZ::rs232ShdnWrite(emuptr address, int size, uint32 value) {
    // Take a snapshot of the line driver states.

    Bool driverStates[kUARTEnd];
    EmHAL::GetLineDriverStates(driverStates);

    StdWriteBE(address, size, value);

    // Respond to any changes in the line driver states.

    EmHAL::CompareLineDriverStates(driverStates);
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmVIIEZ::iXtrnl2Read
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmVIIEZ::iXtrnl2Read(emuptr address, int size) {
    uint32 result = StdReadBE(address, size);

    // Ensure that bit 0x0080 is set.  If it's clear, HotSync
    // will sync via the modem instead of the serial port.

    result |= 0x0080;

    return result;
}
