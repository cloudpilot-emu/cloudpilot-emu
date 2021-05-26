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

#include "EmRegsEZPalmM100.h"

#include "EmCommon.h"
#include "EmRegsEZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "EmSession.h"          // GetDevice

// The following are taken from the m100 version of HardwareEZ.h.

/************************************************************************
 * Port B Bit settings
 ************************************************************************/
#define hwrEZPortBKbdRow0 0x01
#define hwrEZPortBLCDVccOff 0x02  // (L) LCD Vcc On
#define hwrEZPortBKbdRow1 0x08
#define hwrEZPortBKbdRow2 0x40
#define hwrEZPortBKbdRows 0x49

#define hwrEZPortBLCDAdjOn 0x40  // (L) LCD Contrast Adjust On (Brad Rev 0)

/************************************************************************
 * Port C Bit settings
 ************************************************************************/
#define hwrEZPortCLCDEnableOn 0x80  // (H) LCD Enable

/************************************************************************
 * Port D Bit settings
 ************************************************************************/
#define hwrEZPortDKbdCol0 0x01  // (H) Keyboard Column 0		(aka INT0)
#define hwrEZPortDKbdCol1 0x02  // (H) Keyboard Column 1		(aka INT1)
#define hwrEZPortDKbdCol2 0x04  // (H) Keyboard Column 2		(aka INT2)
#define hwrEZPortDKbdCol3 0x08  // (H) Keyboard Column 3		(aka INT3)
#define hwrEZPortDKeyBits 0x0F  // (H) All Keyboard Columns
#define hwrEZPortDDockButton \
    0x10                          // (L) Dock Button				(aka IRQ1) (edge, negative)
#define hwrEZPortD232Enable 0x20  // (L) Serial Shutdown on Sumo Rev 1	(aka IRQ2)
#define hwrEZPortDLCDBiasOn 0x40  // (H) LCD Bias on
#define hwrEZPortDPowerFail 0x80  // (L) Power Fail Interrupt	(aka IRQ6) (level, low)

#define hwrEZPortDDataMaskBeforeWrite (~(hwrEZPortDKeyBits))  // Don't write ones to these bits!
#define hwrEZPortDKeyOffset 0x0                               // bits to shift to put Col 0 in msb

/************************************************************************
 * Port E Bit settings
 ************************************************************************/
#define hwrEZPortESpiTxD 0x01   // (L) SPI TXD (Internal)
#define hwrEZPortESpiRxD 0x02   // (L) SPI RXD (Internal)
#define hwrEZPortESpiClk 0x04   // SPI Clock
#define hwrEZPortESpiBits 0x07  // all the SPI bits
#define hwrEZPortERXD 0x10      // external RXD
#define hwrEZPortETXD 0x20      // external TXD
#define hwrEZPortECTS 0x80      // external CTS

/************************************************************************
 * Port F Bit settings
 ************************************************************************/
#define hwrEZPortFLCDBiasData 0x01  // LCD Bias data
#define hwrEZPortFPenIO 0x02        // (L) Pen Interrupt,Pen I/O
#define hwrEZPortFLCDBiasStrb 0x04  // LCD Bias strobe
#define hwrEZPortFBacklightOn 0x20  // (H) Backlight on
#define hwrEZPortFLCDDispOn 0x40    // (H) LCD display on
#define hwrEZPortFLCDBiasClk 0x80   // LCD Bias clock

#define hwrEZPortFLCDAdjOn 0x01     // (L) LCD Contrast Adjust On (Brad Rev 1)
#define hwrEZPortF232Enable 0x04    // (L) Shutdown Serial
#define hwrEZPortFContrastPWM 0x01  // (L) PWM output for LCD contrast

/************************************************************************
 * Port G Bit settings
 ************************************************************************/
#define hwrEZPortGDTACK 0x01             // (L) DTACK
#define hwrEZPortGFree1 0x02             // (-) FREE5
#define hwrEZPortGIDDetect 0x04          // (L) ID select (drives kbd)
#define hwrEZPortGLiIonVoltReadDis 0x08  // (L) Enable A/D Voltage Divider
#define hwrEZPortGIRShutdown 0x10        // (H) Shutdown IRDA
#define hwrEZPortGADCOff 0x20            // (L) A/D Select

const int kNumButtonRows = 3;
const int kNumButtonCols = 4;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {0, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitPageUp, keyBitHard2, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmM100::EmRegsEZPalmM100
// ---------------------------------------------------------------------------

EmRegsEZPalmM100::EmRegsEZPalmM100(void)
    : EmRegsEZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmM100::~EmRegsEZPalmM100
// ---------------------------------------------------------------------------

EmRegsEZPalmM100::~EmRegsEZPalmM100(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmM100::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmM100::GetLCDScreenOn(void) {
    return (READ_REGISTER(portFData) & hwrEZPortFLCDDispOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmM100::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsEZPalmM100::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portFData) & hwrEZPortFBacklightOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmM100::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsEZPalmM100::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portDData) & hwrEZPortD232Enable) != 0;

    if (type == kUARTIR) return (READ_REGISTER(portGData) & hwrEZPortGIRShutdown) == 0;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmM100::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsEZPalmM100::GetPortInputValue(int port) {
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
//		� EmRegsEZPalmM100::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsEZPalmM100::GetPortInternalValue(int port) {
    uint8 result = EmRegsEZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrEZPortDPowerFail;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmM100::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsEZPalmM100::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.

    UInt8 portBDir = READ_REGISTER(portBDir);
    UInt8 portBData = READ_REGISTER(portBData);

    rows[0] = (portBDir & hwrEZPortBKbdRow0) != 0 && (portBData & hwrEZPortBKbdRow0) == 0;
    rows[1] = (portBDir & hwrEZPortBKbdRow1) != 0 && (portBData & hwrEZPortBKbdRow1) == 0;
    rows[2] = (portBDir & hwrEZPortBKbdRow2) != 0 && (portBData & hwrEZPortBKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmM100::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsEZPalmM100::GetSPISlave(void) {
    if ((READ_REGISTER(portGData) & hwrEZPortGADCOff) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}
