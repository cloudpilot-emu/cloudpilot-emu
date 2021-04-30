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

#include "EmRegsVZM1xx.h"

#include "EmCommon.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x

const int kNumButtonRows = 3;
const int kNumButtonCols = 4;

#pragma mark -

#define hwrVZPalmM125PortDTWISTIRQ 0x20   // Twister IRQ (IRQ2)
#define hwrVZPalmM125PortDPowerFail 0x80  // (L) Power Fail interrupt (aka IRQ6)

#define hwrVZPalmM125PortGEL_ON 0x02        // (L) EL_ON
#define hwrVZPalmM125PortGLCD_DISP_ON 0x04  // (H) LCD_DISP_ON
#define hwrVZPalmM125PortG232_SHDN_N 0x08   // (L) 232_SHDN#
#define hwrVZPalmM125PortGADC_CS_N 0x20     // (H) ADC_CS#

#define hwrVZPalmM125PortKIRShutdown 0x04  // (-) IR_SHDN
#define hwrVZPalmM125PortKVIB_EN 0x10      // vibarator enable/disable
#define hwrVZPalmM125PortKKbdRow0 0x20     // (H) Keyboard Row 0
#define hwrVZPalmM125PortKKbdRow1 0x40     // (H) Keyboard Row 1
#define hwrVZPalmM125PortKKbdRow2 0x80     // (H) Keyboard Row 2

const uint16 kButtonMapPalmM125[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {0, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitPageUp, 0, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::EmRegsVZPalmM125
// ---------------------------------------------------------------------------

EmRegsVZPalmM125::EmRegsVZPalmM125(void)
    : EmRegsVZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet3)) {}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::~EmRegsVZPalmM125
// ---------------------------------------------------------------------------

EmRegsVZPalmM125::~EmRegsVZPalmM125(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM125::GetLCDScreenOn(void) { return (READ_REGISTER(portKData) & 0x02) != 0; }

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM125::GetLCDBacklightOn(void) { return (READ_REGISTER(portFData) & 0x10) != 0; }

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsVZPalmM125::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portEData) & 0x40) != 0;

    if (type == kUARTIR) return (READ_REGISTER(portKData) & hwrVZPalmM125PortKIRShutdown) == 0;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::GetUARTDevice
// ---------------------------------------------------------------------------
// Return what sort of device is hooked up to the given UART.

EmUARTDeviceType EmRegsVZPalmM125::GetUARTDevice(int uartNum) {
    Bool serEnabled = this->GetLineDriverState(kUARTSerial);
    Bool irEnabled = this->GetLineDriverState(kUARTIR);

    // It's probably an error to have them both enabled at the same
    // time.  !!! TBD: make this an error message.

    EmAssert(!(serEnabled && irEnabled));

    if (uartNum == 0) {
        if (irEnabled) return kUARTIR;
    } else if (uartNum == 1) {
        if (serEnabled) return kUARTSerial;
    }

    return kUARTNone;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsVZPalmM125::GetPortInputValue(int port) {
    uint8 result = EmRegsVZ::GetPortInputValue(port);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsVZPalmM125::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Make sure that hwrVZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.
        //
        // Also make sure that hwrVZPalmM125PortDTWISTIRQ is set.  If it's clear,
        // the slot driver will think there's a card installed and will try querying it.

        result |= hwrVZPalmM125PortDPowerFail | hwrVZPalmM125PortDTWISTIRQ;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsVZPalmM125::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMapPalmM125, sizeof(kButtonMapPalmM125));

    // Determine what row is being asked for.

    UInt8 portKDir = READ_REGISTER(portKDir);
    UInt8 portKData = READ_REGISTER(portKData);

    rows[0] =
        (portKDir & hwrVZPalmM125PortKKbdRow0) != 0 && (portKData & hwrVZPalmM125PortKKbdRow0) == 0;
    rows[1] =
        (portKDir & hwrVZPalmM125PortKKbdRow1) != 0 && (portKData & hwrVZPalmM125PortKKbdRow1) == 0;
    rows[2] =
        (portKDir & hwrVZPalmM125PortKKbdRow2) != 0 && (portKData & hwrVZPalmM125PortKKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM125::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVZPalmM125::GetSPISlave(void) {
    if ((READ_REGISTER(portGData) & hwrVZPalmM125PortGADC_CS_N) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

#pragma mark -

#define hwrVZPalmM130PortDTWISTIRQ 0x20   // Twister IRQ (IRQ2)
#define hwrVZPalmM130PortDPowerFail 0x80  // (L) Power Fail interrupt (aka IRQ6)

#define hwrVZPalmM130PortGEL_ON 0x02        // (L) EL_ON
#define hwrVZPalmM130PortGLCD_DISP_ON 0x04  // (H) LCD_DISP_ON
#define hwrVZPalmM130PortG232_SHDN_N 0x08   // (L) 232_SHDN#
#define hwrVZPalmM130PortGADC_CS_N 0x20     // (H) ADC_CS#

#define hwrVZPalmM130PortKIRShutdown 0x04  // (-) IR_SHDN
#define hwrVZPalmM130PortKVIB_EN 0x10      // vibarator enable/disable
#define hwrVZPalmM130PortKKbdRow0 0x20     // (H) Keyboard Row 0
#define hwrVZPalmM130PortKKbdRow1 0x40     // (H) Keyboard Row 1
#define hwrVZPalmM130PortKKbdRow2 0x80     // (H) Keyboard Row 2

const uint16 kButtonMapPalmM130[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {0, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitPageUp, 0, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::EmRegsVZPalmM130
// ---------------------------------------------------------------------------

EmRegsVZPalmM130::EmRegsVZPalmM130(void)
    : EmRegsVZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet1)) {}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::~EmRegsVZPalmM130
// ---------------------------------------------------------------------------

EmRegsVZPalmM130::~EmRegsVZPalmM130(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM130::GetLCDScreenOn(void) { return (READ_REGISTER(portKData) & 0x02) != 0; }

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM130::GetLCDBacklightOn(void) { return (READ_REGISTER(portFData) & 0x10) != 0; }

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsVZPalmM130::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portEData) & 0x40) != 0;

    if (type == kUARTIR) return (READ_REGISTER(portKData) & hwrVZPalmM130PortKIRShutdown) == 0;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::GetUARTDevice
// ---------------------------------------------------------------------------
// Return what sort of device is hooked up to the given UART.

EmUARTDeviceType EmRegsVZPalmM130::GetUARTDevice(int uartNum) {
    Bool serEnabled = this->GetLineDriverState(kUARTSerial);
    Bool irEnabled = this->GetLineDriverState(kUARTIR);

    // It's probably an error to have them both enabled at the same
    // time.  !!! TBD: make this an error message.

    EmAssert(!(serEnabled && irEnabled));

    if (uartNum == 0) {
        if (irEnabled) return kUARTIR;
    } else if (uartNum == 1) {
        if (serEnabled) return kUARTSerial;
    }

    return kUARTNone;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsVZPalmM130::GetPortInputValue(int port) {
    uint8 result = EmRegsVZ::GetPortInputValue(port);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsVZPalmM130::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Make sure that hwrVZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.
        //
        // Also make sure that hwrVZPalmM130PortDTWISTIRQ is set.  If it's clear,
        // the slot driver will think there's a card installed and will try querying it.

        result |= hwrVZPalmM130PortDPowerFail | hwrVZPalmM130PortDTWISTIRQ;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsVZPalmM130::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMapPalmM130, sizeof(kButtonMapPalmM130));

    // Determine what row is being asked for.

    UInt8 portKDir = READ_REGISTER(portKDir);
    UInt8 portKData = READ_REGISTER(portKData);

    rows[0] =
        (portKDir & hwrVZPalmM130PortKKbdRow0) != 0 && (portKData & hwrVZPalmM130PortKKbdRow0) == 0;
    rows[1] =
        (portKDir & hwrVZPalmM130PortKKbdRow1) != 0 && (portKData & hwrVZPalmM130PortKKbdRow1) == 0;
    rows[2] =
        (portKDir & hwrVZPalmM130PortKKbdRow2) != 0 && (portKData & hwrVZPalmM130PortKKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVZPalmM130::GetSPISlave(void) {
    if ((READ_REGISTER(portGData) & hwrVZPalmM130PortGADC_CS_N) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

#pragma mark -

// Given a register (specified by its field name), return its address
// in emulated space.

#undef db_addressoff
#define db_addressoff(reg) (this->GetAddressStart() + fRegs.offsetof_##reg())

// Macro to help the installation of handlers for a register.

#undef INSTALL_HANDLER
#define INSTALL_HANDLER(read, write, reg)                                          \
    this->SetHandler((ReadFunction)&EmRegsPLDPalmI705::read,                       \
                     (WriteFunction)&EmRegsPLDPalmI705::write, db_addressoff(reg), \
                     fRegs.reg.GetSize())

#if 0

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {keyBitPageUp, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitContrast, keyBitHard2, 0}};

    #pragma mark -
    #define hwrVZPortBRADIO_DTR 0x40  // (L) Radio DTR
    #define hwrVZPortKRADIO_DSR 0x02  // Radio DTR

    #define hwrVZPalmI705PortDTWISTIRQ 0x20   // Twister IRQ (IRQ2)
    #define hwrVZPalmI705PortDPowerFail 0x80  // (L) Power Fail interrupt (aka IRQ6)

    #define hwrVZPalmI705PortGEL_ON 0x02        // (L) EL_ON
    #define hwrVZPalmI705PortGLCD_DISP_ON 0x04  // (H) LCD_DISP_ON
    #define hwrVZPalmI705PortG232_SHDN_N 0x08   // (L) 232_SHDN#
    #define hwrVZPalmI705PortGADC_CS_N 0x20     // (H) ADC_CS#

    #define hwrVZPalmI705PortKVIB_EN 0x10   // vibarator enable/disable
    #define hwrVZPalmI705PortKKbdRow0 0x20  // (H) Keyboard Row 0
    #define hwrVZPalmI705PortKKbdRow1 0x40  // (H) Keyboard Row 1
    #define hwrVZPalmI705PortKKbdRow2 0x80  // (H) Keyboard Row 2

    #define hwrVZPalmI705PortMIR_SD 0x20  // (L) Infrared Shut-down (UART 2)


// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::EmRegsVZPalmI705
// ---------------------------------------------------------------------------

EmRegsVZPalmI705::EmRegsVZPalmI705(void)
    : EmRegsVZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet1)) {}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::~EmRegsVZPalmI705
// ---------------------------------------------------------------------------

EmRegsVZPalmI705::~EmRegsVZPalmI705(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmI705::GetLCDScreenOn(void) {
    return (READ_REGISTER(portGData) & hwrVZPalmI705PortGLCD_DISP_ON) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmI705::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portGData) & hwrVZPalmI705PortGEL_ON) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsVZPalmI705::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portGData) & hwrVZPalmI705PortG232_SHDN_N) != 0;

    if (type == kUARTIR) return (READ_REGISTER(portMData) & hwrVZPalmI705PortMIR_SD) == 0;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetUARTDevice
// ---------------------------------------------------------------------------
// Return what sort of device is hooked up to the given UART.

EmUARTDeviceType EmRegsVZPalmI705::GetUARTDevice(int uartNum) {
    Bool serEnabled = this->GetLineDriverState(kUARTSerial);
    Bool irEnabled = this->GetLineDriverState(kUARTIR);

    // It's probably an error to have them both enabled at the same
    // time.  !!! TBD: make this an error message.

    EmAssert(!(serEnabled && irEnabled));

    /*
            From Van Oler:

                    UART #2 is used for serial (rs-232) and IR.  The rs-232 receiver
                    and the IR receiver are tristate outouts with individual enables
                    to control which one drives the RXD2 input.

                    UART1 is not used; only the pins are used for GPIO.

                    The Master SPI (on PE0,1,2) is used for data to and from the
                    touchscreen A/D and the radio DSP.  Each has individual selects.

                    The Master/Slave SPI (on PJ0,1,2) is used for the MMC/SD card slot.

                    There is a separate USB chip, the same as tornado.
    */

    if (uartNum == 1) {
        if (serEnabled) return kUARTSerial;

        if (irEnabled) return kUARTIR;
    }

    return kUARTNone;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmI705::GetVibrateOn(void) {
    return (READ_REGISTER(portKData) & hwrVZPalmI705PortKVIB_EN) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsVZPalmI705::GetPortInputValue(int port) {
    uint8 result = EmRegsVZ::GetPortInputValue(port);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsVZPalmI705::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Make sure that hwrVZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.
        //
        // Also make sure that hwrVZPalmI705PortDTWISTIRQ is set.  If it's clear,
        // the slot driver will think there's a card installed and will try querying it.

        result |= hwrVZPalmI705PortDPowerFail | hwrVZPalmI705PortDTWISTIRQ;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsVZPalmI705::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.

    UInt8 portKDir = READ_REGISTER(portKDir);
    UInt8 portKData = READ_REGISTER(portKData);

    rows[0] =
        (portKDir & hwrVZPalmI705PortKKbdRow0) != 0 && (portKData & hwrVZPalmI705PortKKbdRow0) == 0;
    rows[1] =
        (portKDir & hwrVZPalmI705PortKKbdRow1) != 0 && (portKData & hwrVZPalmI705PortKKbdRow1) == 0;
    rows[2] =
        (portKDir & hwrVZPalmI705PortKKbdRow2) != 0 && (portKData & hwrVZPalmI705PortKKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmI705::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVZPalmI705::GetSPISlave(void) {
    if ((READ_REGISTER(portGData) & hwrVZPalmI705PortGADC_CS_N) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::EmRegsPLDPalmI705
// ---------------------------------------------------------------------------

EmRegsPLDPalmI705::EmRegsPLDPalmI705(emuptr baseAddr) : fBaseAddr(baseAddr) {}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::~EmRegsPLDPalmI705
// ---------------------------------------------------------------------------

EmRegsPLDPalmI705::~EmRegsPLDPalmI705(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::Initialize
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::Initialize(void) { EmRegs::Initialize(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::Reset
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);

    if (hardwareReset) {
        memset(fRegs.GetPtr(), 0, fRegs.GetSize());
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::Save
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::Save(SessionFile& f) { EmRegs::Save(f); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::Load
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::Load(SessionFile& f) { EmRegs::Load(f); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::Dispose
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::Dispose(void) { EmRegs::Dispose(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmRegsPLDPalmI705::GetLEDState(void) {
    uint16 result = kLEDOff;

    if (fRegs.green_led) result |= kLEDGreen;

    if (fRegs.red_led) result |= kLEDRed;

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsPLDPalmI705::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(StdReadBE, StdWriteBE, todsln3);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, green_led);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, pin_notify_enable);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, red_led);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, acpr);
    INSTALL_HANDLER(low_battRead, StdWriteBE, low_batt);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, dto3svc_irq);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, dsp_irq_enable);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, dsp_irq_polarity);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, charging_in_progress);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, charging_in_prog2);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, charging_in_prog_OE);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, twister_irq_wake);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, low_batt_mask);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, acpr_mask);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, acpr_polarity);
    INSTALL_HANDLER(usb_intRead, StdWriteBE, usb_int);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, usb_int_mask);
    INSTALL_HANDLER(bullet_detRead, StdWriteBE, bullet_det);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, bullet_det_mask);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, bullet_det_pol);
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsPLDPalmI705::GetRealAddress(emuptr address) {
    return (uint8*)fRegs.GetPtr() + address - this->GetAddressStart();
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsPLDPalmI705::GetAddressStart(void) { return fBaseAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmI705::GetAddressRange(void) { return fRegs.GetSize(); }

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::low_battRead
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmI705::low_battRead(emuptr address, int size) {
    uint32 reg = StdReadBE(address, size);

    #define hwrPLDlow_battOn 0  // this is inverted!
    #define hwrPLDlow_battOff 0x80

    // If we're reading the whole register, or if we're reading the second
    // byte, then set the bit that says that the low-battery condition is off.

    if ((size == 2) || ((size == 1) && ((address & 1) == 1))) {
        reg |= hwrPLDlow_battOff;
    }

    return reg;
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::usb_intRead
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmI705::usb_intRead(emuptr /*address*/, int /*size*/) {
    // this bit is low-asserted

    return ~0;
}

// ---------------------------------------------------------------------------
//		� EmRegsPLDPalmI705::bullet_detRead
// ---------------------------------------------------------------------------

uint32 EmRegsPLDPalmI705::bullet_detRead(emuptr /*address*/, int /*size*/) {
    // 1 = undocked, 0 = docked

    return ~0;
}

#endif
