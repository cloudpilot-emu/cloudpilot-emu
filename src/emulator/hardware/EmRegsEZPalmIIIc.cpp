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

#include "EmRegsEZPalmIIIc.h"

#include "EmCommon.h"
#include "EmRegsEZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "PalmPack.h"
#define NON_PORTABLE
#include "EZAustin/IncsPrv/HardwareAustin.h"  // hwrEZPortCKbdRow0, hwrEZPortBRS232Enable, etc.
#undef NON_PORTABLE
#include "PalmPackPop.h"

const int kNumButtonRows = 3;
const int kNumButtonCols = 4;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {keyBitPageUp, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitContrast, keyBitHard2, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::EmRegsEZPalmIIIc
// ---------------------------------------------------------------------------

EmRegsEZPalmIIIc::EmRegsEZPalmIIIc(void) : EmRegsEZ() {
    fSPISlaveADC = new EmSPISlaveADS784x(kChannelSet2);
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::~EmRegsEZPalmIIIc
// ---------------------------------------------------------------------------

EmRegsEZPalmIIIc::~EmRegsEZPalmIIIc(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmIIIc::GetLCDScreenOn(void) {
    // Override the Dragonball version and let the SED 1375 handle it.

    return EmHALHandler::GetLCDScreenOn();
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmIIIc::GetLCDBacklightOn(void) {
    // Override the Dragonball version and let the SED 1375 handle it.

    return EmHALHandler::GetLCDBacklightOn();
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmIIIc::GetLCDHasFrame(void) {
    // Override the Dragonball version and let the SED 1375 handle it.

    return EmHALHandler::GetLCDHasFrame();
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetLCDBeginEnd
// ---------------------------------------------------------------------------

void EmRegsEZPalmIIIc::GetLCDBeginEnd(emuptr& begin, emuptr& end) {
    // Override the Dragonball version and let the SED 1375 handle it.

    EmHALHandler::GetLCDBeginEnd(begin, end);
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetLCDScanlines
// ---------------------------------------------------------------------------

bool EmRegsEZPalmIIIc::CopyLCDFrame(Frame& frame) { return EmHALHandler::CopyLCDFrame(frame); }

uint16 EmRegsEZPalmIIIc::GetLCD2bitMapping() { return EmHALHandler::GetLCD2bitMapping(); }

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsEZPalmIIIc::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portBData) & hwrEZPortBRS232Enable) != 0;

    if (type == kUARTIR) return (READ_REGISTER(portGData) & hwrEZPortGIRShutdown) == 0;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsEZPalmIIIc::GetPortInputValue(int port) {
    uint8 result = EmRegsEZ::GetPortInputValue(port);

    if (port == 'F') {
        // Ensure that bit hwrEZPortFIXTRNL2 is set.  If it's clear, HotSync
        // will sync via the modem instead of the serial port.

        // Make sure hwrEZPortFLCDPowered is always set, or HwrDisplayWake will hang

        result |= hwrEZPortFIXTRNL2 | hwrEZPortFLCDPowered;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsEZPalmIIIc::GetPortInternalValue(int port) {
    uint8 result = EmRegsEZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrEZPortDPowerFail;

        // Make sure hwrEZPortDAdapterInstalled is set, or the dock-status routines
        // will report that we're powered (even if we aren't docked!).

        result |= hwrEZPortDAdapterInstalled;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsEZPalmIIIc::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.

    UInt8 portCDir = READ_REGISTER(portCDir);
    UInt8 portCData = READ_REGISTER(portCData);

    rows[0] = (portCDir & hwrEZPortCKbdRow0) != 0 && (portCData & hwrEZPortCKbdRow0) == 0;
    rows[1] = (portCDir & hwrEZPortCKbdRow1) != 0 && (portCData & hwrEZPortCKbdRow1) == 0;
    rows[2] = (portCDir & hwrEZPortCKbdRow2) != 0 && (portCData & hwrEZPortCKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsEZPalmIIIc::GetSPISlave(void) {
    if ((READ_REGISTER(portBData) & hwrEZPortBCS_ADC) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

void EmRegsEZPalmIIIc::MarkScreen() {}

void EmRegsEZPalmIIIc::UnmarkScreen() {}
