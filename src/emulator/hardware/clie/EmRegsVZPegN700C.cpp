#include "EmRegsVZPegN700C.h"

#include "EmBankRegs.h"  // EmBankRegs::InvalidAccess
#include "EmCommon.h"
#include "EmMemory.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "PalmPack.h"

#define NON_PORTABLE
#include "EZAustin/IncsPrv/HardwareAustin.h"  // hwrEZPortCKbdRow0, hwrEZPortBRS232Enable, etc.
#undef NON_PORTABLE
#include "PalmPackPop.h"

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

// Port B Bit settings for HiRez_CLIE

// Port C Bit settings for HiRez_CLIE
#define hwrVZPEG700PortCBacklightOn 0x10  // (H) Backlight ON Enable
#define hwrVZPEG700PortCKbdRow0 0x01
#define hwrVZPEG700PortCKbdRow1 0x02
#define hwrVZPEG700PortCKbdRow2 0x04

// Port D Bit settings for HiRez_CLIE
#define hwrVZPEG700PortDKbdCol0 0x01
#define hwrVZPEG700PortDKbdCol1 0x02
#define hwrVZPEG700PortDKbdCol2 0x04
#define hwrVZPEG700PortDMS_IF_Intl 0x08  // MemoryStick Interface IC
#define hwrVZPEG700PortDDockButton 0x10
#define hwrVZPEG700PortDPowerFail 0x80  // (L) Power Fail Interrupt	(aka IRQ6) (level, low)

// Port F Bit settings for HiRez_CLIE

// Port E Bit settings for HiRez_CLIE
#define hwrVZPEG700PortEHold 0x08  // Hold select

// Port G Bit settings for HiRez_CLIE
#define hwrVZPEG700PortMADC_CS 0x20  // ADC chip select

// Port J Bit settings for HiRez_CLIE
#define hwrVZPEG700PortJHSYN_IRQ 0x01
#define hwrVZPEG700PortJRS232Enable 0x10  // Enable the RS232 Transceiver
#define hwrVZPEG700PortJDC_IN 0x20        // DC_IN

#define hwrVZPEG700PortKLCDPowered 0x10

#define keyBitJogBack (0x0800)  // JogDial: Back key

const int kNumButtonRows = 3;
const int kNumButtonCols = 3;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3},
    {keyBitPageUp, keyBitPageDown, 0},
    {keyBitPower, keyBitHard4, keyBitJogBack}};

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::EmRegsEZPalmIIIc
// ---------------------------------------------------------------------------

EmRegsVzPegN700C::EmRegsVzPegN700C(void)
    : EmRegsVZNoScreen(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::~EmRegsEZPalmIIIc
// ---------------------------------------------------------------------------

EmRegsVzPegN700C::~EmRegsVzPegN700C(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegN700C::GetLCDScreenOn(void) {
    UInt8 portKData = READ_REGISTER(portKData);
    return (portKData & hwrVZPEG700PortKLCDPowered) != 0;
    //	return true;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegN700C::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portCData) & hwrVZPEG700PortCBacklightOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetSerialPortOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegN700C::GetSerialPortOn(int /*uartNum*/) {
    return (READ_REGISTER(portJData) & hwrVZPEG700PortJRS232Enable) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetPortInputValue
// ---------------------------------------------------------------------------

uint8 EmRegsVzPegN700C::GetPortInputValue(int port) {
    uint8 result = EmRegsVZ::GetPortInputValue(port);

    if (port == 'K') {
        // Make sure this is always set, or HwrDisplayWake will hang
        result |= 0x02;
    }

    if (port == 'E') {
        result |= hwrVZPEG700PortEHold;
    }

    if (port == 'J') {
        // Make sure this is always set, or HwrDisplayWake will hang
        result |= hwrVZPEG700PortJDC_IN | hwrVZPEG700PortJHSYN_IRQ;
    }
    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetPortInternalValue
// ---------------------------------------------------------------------------

uint8 EmRegsVzPegN700C::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZ::GetPortInternalValue(port);

    if (port == 'D') {
        result = GetKeyBits();

        // Ensure that bit hwrEZPortDDockButton is set.  If it's clear, HotSync
        // will sync via the modem instead of the serial port.
        //
        // Also make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrVZPEG700PortDDockButton | hwrVZPEG700PortDPowerFail;

        result |= hwrVZPEG700PortDMS_IF_Intl;
    }

    if (port == 'K') {
        // Make sure this is always set, or HwrDisplayWake will hang
        result |= 0x02;
    }

    if (port == 'E') {
        result |= hwrVZPEG700PortEHold;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsVzPegN700C::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.
    UInt8 portCDir = READ_REGISTER(portCDir);
    UInt8 portCData = READ_REGISTER(portCData);

    rows[0] =
        (portCDir & hwrVZPEG700PortCKbdRow0) != 0 && (portCData & hwrVZPEG700PortCKbdRow0) == 0;
    rows[1] =
        (portCDir & hwrVZPEG700PortCKbdRow1) != 0 && (portCData & hwrVZPEG700PortCKbdRow1) == 0;
    rows[2] =
        (portCDir & hwrVZPEG700PortCKbdRow2) != 0 && (portCData & hwrVZPEG700PortCKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetSPI2Slave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVzPegN700C::GetSPI2Slave(void) {
    if ((READ_REGISTER(portMData) & hwrVZPEG700PortMADC_CS) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmRegsVzPegN700C::GetLEDState(void) {
    uint16 result = kLEDOff;
    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegN700C::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegN700C::GetVibrateOn(void) { return false; }

void EmRegsVzPegN700C::portDIntReqEnWrite(emuptr address, int size, uint32 value) {
    EmRegsVZ::portDIntReqEnWrite(address, size, value & ~0x08);
}
