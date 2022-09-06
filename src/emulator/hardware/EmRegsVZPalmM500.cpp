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

#include "EmRegsVZPalmM500.h"

#include "EmCommon.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "ExternalStorage.h"

#define hwrVZPortBALARM_LED 0x40  // (L) Alarm LED

#define hwrVZPortDSpi1Card \
    0x20  // (-) SPI #1, slot 0, SD/MMC Card Detect EOR Wake EOR Ready (aka IRQ2)
#define hwrVZPortDPowerFail 0x80  // (L) Power Fail interrupt	(aka IRQ6)

#define hwrVZPortGEL_ON 0x02       // (L) EL_ON
#define hwrVZPortGADC_CS_N 0x04    // (H) ADC_CS#
#define hwrVZPortG232_SHDN_N 0x08  // (L) 232_SHDN#

#define hwrVZPortKLCDOn 0x02     // (-) LCD_DISP_ON (RW#)
#define hwrVZPortKAC_PWR_N 0x04  // (-) AC_PWR# (LDS#)
#define hwrVZPortKVibeEn 0x10    // (-) VIBE_EN
#define hwrVZPortKKbdRow0 0x20   // (H) Keyboard Row 0
#define hwrVZPortKKbdRow1 0x40   // (H) Keyboard Row 1
#define hwrVZPortKKbdRow2 0x80   // (H) Keyboard Row 2

#define hwrVZPortMIR_SD 0x20  // (L) Infrared Shut-down

const int kNumButtonRows = 3;
const int kNumButtonCols = 4;

const uint16 kGenericMonoMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {keyBitPageUp, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitContrast, keyBitHard2, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::EmRegsVZPalmM500
// ---------------------------------------------------------------------------

EmRegsVZPalmM500::EmRegsVZPalmM500(void) : EmRegsVZ() {
    fSPISlaveADC = new EmSPISlaveADS784x(kChannelSet1);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::~EmRegsVZPalmM500
// ---------------------------------------------------------------------------

EmRegsVZPalmM500::~EmRegsVZPalmM500(void) { delete fSPISlaveADC; }

void EmRegsVZPalmM500::Reset(Bool hardwareReset) {
    EmRegsVZ::Reset(hardwareReset);
    spiSlaveSD->Reset();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM500::GetLCDScreenOn(void) {
    return (READ_REGISTER(portKData) & hwrVZPortKLCDOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM500::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portGData) & hwrVZPortGEL_ON) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsVZPalmM500::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portGData) & hwrVZPortG232_SHDN_N) != 0;

    if (type == kUARTIR) return (READ_REGISTER(portMData) & hwrVZPortMIR_SD) == 0;

    if (type == kUARTMystery) return true;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetUARTDevice
// ---------------------------------------------------------------------------
// Return what sort of device is hooked up to the given UART.

EmUARTDeviceType EmRegsVZPalmM500::GetUARTDevice(int uartNum) {
    Bool serEnabled = this->GetLineDriverState(kUARTSerial);
    Bool irEnabled = this->GetLineDriverState(kUARTIR);

    // It's probably an error to have them both enabled at the same
    // time.  !!! TBD: make this an error message.

    EmAssert(!(serEnabled && irEnabled));

    /*
            From Michael Cortopassi:

                    UART 1 for IRda
                    UART 2 for hotsync
    */

    if (uartNum == 0) {
        if (irEnabled) return kUARTIR;
    } else if (uartNum == 1) {
        if (serEnabled) return kUARTSerial;
    }

    return kUARTNone;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM500::GetVibrateOn(void) {
    return (READ_REGISTER(portKData) & hwrVZPortKVibeEn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmRegsVZPalmM500::GetLEDState(void) {
    uint16 result = kLEDOff;
    UInt8 portBData = READ_REGISTER(portBData);

    if (portBData & hwrVZPortBALARM_LED) result |= kLEDGreen;

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsVZPalmM500::GetPortInputValue(int port) {
    uint8 result = EmRegsVZ::GetPortInputValue(port);

    if (port == 'K') {
        // Make sure hwrVZPortKAC_PWR_N is set, or the dock-status routines
        // will report that we're powered (even if we aren't docked!).

        result |= hwrVZPortKAC_PWR_N;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsVZPalmM500::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Make sure that hwrVZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.
        //
        // Also make sure that hwrVZPortDSpi1Card is set.  If it's clear,
        // the slot driver will think there's a card installed and will try querying it.

        result |= hwrVZPortDPowerFail;

        if (!gExternalStorage.IsMounted(EmHAL::Slot::sdcard))
            result |= hwrVZPortDSpi1Card;
        else
            result &= ~hwrVZPortDSpi1Card;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsVZPalmM500::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kGenericMonoMap, sizeof(kGenericMonoMap));

    // Determine what row is being asked for.

    UInt8 portKDir = READ_REGISTER(portKDir);
    UInt8 portKData = READ_REGISTER(portKData);

    rows[0] = (portKDir & hwrVZPortKKbdRow0) != 0 && (portKData & hwrVZPortKKbdRow0) == 0;
    rows[1] = (portKDir & hwrVZPortKKbdRow1) != 0 && (portKData & hwrVZPortKKbdRow1) == 0;
    rows[2] = (portKDir & hwrVZPortKKbdRow2) != 0 && (portKData & hwrVZPortKKbdRow2) == 0;
}

bool EmRegsVZPalmM500::SupportsSlot(EmHAL::Slot slot) { return slot == EmHAL::Slot::sdcard; }

void EmRegsVZPalmM500::Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) {
    UpdateIRQ2(hwrVZPortDSpi1Card);
}

void EmRegsVZPalmM500::Unmount(EmHAL::Slot slot) { UpdateIRQ2(0x00); }

void EmRegsVZPalmM500::PortDataChanged(int port, uint8 oldValue, uint8 newValue) {
    EmRegsVZ::PortDataChanged(port, oldValue, newValue);

    if (port != 'J') return;

    uint8 portJSelect = READ_REGISTER(portJSelect);
    if ((portJSelect & 0x08) == 0) return;
    if (((oldValue ^ newValue) & 0x08) == 0) return;

    if (newValue & 0x08)
        spiSlaveSD->Disable();
    else
        spiSlaveSD->Enable();
}

EmSPISlave* EmRegsVZPalmM500::GetSPI1Slave() { return spiSlaveSD.get(); }

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM500::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVZPalmM500::GetSPI2Slave(void) {
    if ((READ_REGISTER(portGData) & hwrVZPortGADC_CS_N) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

void EmRegsVZPalmM500::Spi1AssertSlaveSelect() {
    uint8 portJSelect = READ_REGISTER(portJSelect);
    if (portJSelect & 0x08) return;

    spiSlaveSD->Enable();
}

void EmRegsVZPalmM500::Spi1DeassertSlaveSelect() {
    uint8 portJSelect = READ_REGISTER(portJSelect);
    if (portJSelect & 0x08) return;

    spiSlaveSD->Disable();
}
