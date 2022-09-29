#include "EmRegsVZPegModena.h"

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

using VZ = EmSonyVzWithSlot<EmRegsVZNoScreen>;

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

// Port B Bit settings for Modena
#define hwrVZModenaPortBDC_IN 0x40  // PB6:DC_IN

// Port C Bit settings for Modena
#define hwrVZModenaPortCBacklightOn 0x10  // (H) Backlight ON Enable

// Port D Bit settings for Modena
#define hwrVZModenaPortDKbdCol0 0x01
#define hwrVZModenaPortDKbdCol1 0x02
#define hwrVZModenaPortDKbdCol2 0x04
#define hwrVZModenaPortDMS_INS 0x40

#define hwrVZModenaPortDDockButton 0x10
#define hwrVZModenaPortDPowerFail 0x80  // (L) Power Fail Interrupt	(aka IRQ6) (level, low)

// Port E Bit settings for Modena
#define hwrVZModenaPortEAlermLED 0x08  // PE3: AlermLED

// Port G Bit settings for Modena
#define hwrVZModenaPortGADC_CS 0x08       // ADC chip select
#define hwrVZModenaPortGRS232Enable 0x10  // Enable the RS232 Transceiver

// Port K Bit settings for Modena
#define hwrVZModenaPortKVibrate 0x04  // PK2: �U�����[�^�[
#define hwrVZModenaPortKKbdRow0 0x20  // PK5:
#define hwrVZModenaPortKKbdRow1 0x40  // PK6:
#define hwrVZModenaPortKKbdRow2 0x80  // PK7:

// Port J Bit settings for Modena
#define hwrVZModenaPortJDC_IN 0x04        // PJ2 : DC_IN
#define hwrVZModenaPortJLCDEnableOn 0x08  // (H) LCD Enable

#define keyBitJogBack (0x0800)  // JogDial: Back key

const int kNumButtonRows = 3;
const int kNumButtonCols = 3;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {{keyBitHard1, keyBitHard2, keyBitHard3},
                                                           {keyBitPageUp, keyBitPageDown, 0},
                                                           {keyBitPower, keyBitHard4, (0x0800)}};

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::EmRegsVzPegModena
// ---------------------------------------------------------------------------

EmRegsVzPegModena::EmRegsVzPegModena(EmRegsMB86189& mb86189)
    : VZ(mb86189), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::~EmRegsEZPalmIIIc
// ---------------------------------------------------------------------------

EmRegsVzPegModena::~EmRegsVzPegModena(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegModena::GetLCDScreenOn(void) {
    UInt8 portCData = READ_REGISTER(portCData);
    return (portCData & 0x01) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegModena::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portCData) & hwrVZModenaPortCBacklightOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetSerialPortOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegModena::GetSerialPortOn(int /*uartNum*/) {
    return (READ_REGISTER(portGData) & hwrVZModenaPortGRS232Enable) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetPortInputValue
// ---------------------------------------------------------------------------

uint8 EmRegsVzPegModena::GetPortInputValue(int port) {
    uint8 result = VZ::GetPortInputValue(port);

    if (port == 'J') {
        result |= hwrVZModenaPortJDC_IN;
    }

    if (port == 'F') {
        // Make sure this is always set, or HwrDisplayWake will hang
        //		result |= hwrVZNascaPortFLCDPowered;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetPortInternalValue
// ---------------------------------------------------------------------------

uint8 EmRegsVzPegModena::GetPortInternalValue(int port) {
    uint8 result = VZ::GetPortInternalValue(port);

    if (port == 'D') {
        // Ensure that bit hwrEZPortDDockButton is set.  If it's clear, HotSync
        // will sync via the modem instead of the serial port.
        //
        // Also make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrVZModenaPortDDockButton | hwrVZModenaPortDPowerFail;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsVzPegModena::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.
    UInt8 portKDir = READ_REGISTER(portKDir);
    UInt8 portKData = READ_REGISTER(portKData);

    rows[0] =
        (portKDir & hwrVZModenaPortKKbdRow0) != 0 && (portKData & hwrVZModenaPortKKbdRow0) == 0;
    rows[1] =
        (portKDir & hwrVZModenaPortKKbdRow1) != 0 && (portKData & hwrVZModenaPortKKbdRow1) == 0;
    rows[2] =
        (portKDir & hwrVZModenaPortKKbdRow2) != 0 && (portKData & hwrVZModenaPortKKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetSPI2Slave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVzPegModena::GetSPI2Slave(void) {
    if ((READ_REGISTER(portGData) & hwrVZModenaPortGADC_CS) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmRegsVzPegModena::GetLEDState(void) {
    uint16 result = kLEDOff;
    UInt8 portEData = READ_REGISTER(portEData);

    if (portEData & hwrVZModenaPortEAlermLED) result |= (kLEDRed | kLEDGreen);
    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegModena::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegModena::GetVibrateOn(void) {
    UInt8 portKData = READ_REGISTER(portKData);
    return (portKData & hwrVZModenaPortKVibrate);
}
