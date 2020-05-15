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

#include "EmRegsEZPalmV.h"

#include "EmCommon.h"
#include "EmRegsEZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "EmSession.h"          // GetDevice

#if 0                      // CSTODO
    #include "EmDevice.h"  // HardwareSubID
#endif

// clang-format false
#include "PalmPack.h"
#define NON_PORTABLE
#include "EZSumo/IncsPrv/HardwareEZ.h"  // hwrEZPortCLCDEnableOn, etc.
#undef NON_PORTABLE
#include "PalmPackPop.h"
// clang-format true

const int kNumButtonRows = 3;
const int kNumButtonCols = 4;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {keyBitPageUp, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitContrast, keyBitHard2, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::EmRegsEZPalmV
// ---------------------------------------------------------------------------

EmRegsEZPalmV::EmRegsEZPalmV(void)
    : EmRegsEZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::~EmRegsEZPalmV
// ---------------------------------------------------------------------------

EmRegsEZPalmV::~EmRegsEZPalmV(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmV::GetLCDScreenOn(void) {
    return (READ_REGISTER(portCData) & hwrEZPortCLCDEnableOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmV::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portFData) & hwrEZPortFBacklightOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsEZPalmV::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portDData) & hwrEZPortD232Enable) != 0;

    if (type == kUARTIR) return (READ_REGISTER(portGData) & hwrEZPortGIRShutdown) == 0;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsEZPalmV::GetPortInputValue(int port) {
    uint8 result = EmRegsEZ::GetPortInputValue(port);

    if (port == 'E') {
        // Return the hardware sub-ID.

        EmAssert(gSession);

#if 0  // CSTODO
        EmDevice device = gSession->GetDevice();
        result |= ~device.HardwareSubID();
#endif
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsEZPalmV::GetPortInternalValue(int port) {
    uint8 result = EmRegsEZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrEZPortDPowerFail;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsEZPalmV::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
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
//		� EmRegsEZPalmV::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsEZPalmV::GetSPISlave(void) {
    if ((READ_REGISTER(portGData) & hwrEZPortGADCOff) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}
