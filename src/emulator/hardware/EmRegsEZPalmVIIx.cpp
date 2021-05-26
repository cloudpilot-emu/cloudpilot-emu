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

#include "EmRegsEZPalmVIIx.h"

#include "EmCommon.h"
#include "EmDevice.h"  // HardwareSubID
#include "EmRegsEZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "EmSession.h"          // GetDevice

// clang-format off
#include "PalmPack.h"
// clang-format on

#define NON_PORTABLE
#include "EZSumo/IncsPrv/HardwareEZ.h"  // hwrEZPortCLCDEnableOn, etc.
#undef NON_PORTABLE
#include "PalmPackPop.h"

// As near as I can tell, PalmVIIx uses the same hardware defs as
// a Sumo, but with this one bit difference.  Actually, there are
// other Port F and G differences, but they aren't (currently)
// relevent to Poser.

#undef hwrEZPortFKbdRow0
#define hwrEZPortFKbdRow0 0x01  // (L) Keyboard Row 0

const int kNumButtonRows = 3;
const int kNumButtonCols = 4;

// ============================== PalmVIIx ==============================

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {keyBitPageUp, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitContrast, keyBitHard2, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::EmRegsEZPalmVIIx
// ---------------------------------------------------------------------------

EmRegsEZPalmVIIx::EmRegsEZPalmVIIx(void)
    : EmRegsEZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::~EmRegsEZPalmVIIx
// ---------------------------------------------------------------------------

EmRegsEZPalmVIIx::~EmRegsEZPalmVIIx(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmVIIx::GetLCDScreenOn(void) {
    return (READ_REGISTER(portCData) & hwrEZPortCLCDEnableOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmVIIx::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portFData) & hwrEZPortFBacklightOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsEZPalmVIIx::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial)
        // Pass this on to the PLD handler.
        return EmHALHandler::GetLineDriverState(type);

    if (type == kUARTIR) return (READ_REGISTER(portGData) & hwrEZPortGIRShutdown) == 0;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsEZPalmVIIx::GetPortInputValue(int port) {
    uint8 result = EmRegsEZ::GetPortInputValue(port);

    if (port == 'E') {
        // Return the hardware sub-ID.

        EmAssert(gSession);

        EmDevice device = gSession->GetDevice();
        result |= ~device.HardwareSubID();
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsEZPalmVIIx::GetPortInternalValue(int port) {
    uint8 result = EmRegsEZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrEZPortDPowerFail;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsEZPalmVIIx::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.

    UInt8 portFDir = READ_REGISTER(portFDir);
    UInt8 portFData = READ_REGISTER(portFData);

    rows[0] = (portFDir & hwrEZPortFKbdRow0) != 0 && (portFData & hwrEZPortFKbdRow0) == 0;
    rows[1] = (portFDir & hwrEZPortFKbdRow1) != 0 && (portFData & hwrEZPortFKbdRow1) == 0;
    rows[2] = (portFDir & hwrEZPortFKbdRow2) != 0 && (portFData & hwrEZPortFKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVIIx::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsEZPalmVIIx::GetSPISlave(void) {
    if ((READ_REGISTER(portGData) & hwrEZPortGADCOff) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}
