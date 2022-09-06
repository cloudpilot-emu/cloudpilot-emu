#include "EmRegsVZPegYellowStone.h"

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

// Port B Bit settings for YellowStone
#define hwrVZYellowPortBDC_IN 0x40  // DC_IN

// Port C Bit settings for YellowStone
#define hwrVZYellowPortCBacklightOn 0x10  // (H) Backlight ON Enable
#define hwrVZYellowPortCKbdRow0 0x01
#define hwrVZYellowPortCKbdRow1 0x02
#define hwrVZYellowPortCKbdRow2 0x04

// Port D Bit settings for YellowStone
#define hwrVZYellowPortDKbdCol0 0x01
#define hwrVZYellowPortDKbdCol1 0x02
#define hwrVZYellowPortDKbdCol2 0x04
#define hwrVZYellowPortDMS_INS 0x40

#define hwrVZYellowPortDDockButton 0x10
#define hwrVZYellowPortDPowerFail 0x80  // (L) Power Fail Interrupt	(aka IRQ6) (level, low)

// Port F Bit settings for YellowStone
#define hwrVZYellowPortFLCDPowered 0x01

// Port G Bit settings for YellowStone
#define hwrVZYellowPortGADC_CS 0x08       // ADC chip select
#define hwrVZYellowPortGRS232Enable 0x10  // Enable the RS232 Transceiver

#define keyBitJogBack (0x0800)  // JogDial: Back key

// �ȉ�����
const int kNumButtonRows = 3;
const int kNumButtonCols = 3;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3},
    {keyBitPageUp, keyBitPageDown, 0},
    {keyBitPower, keyBitHard4, keyBitJogBack}};

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::EmRegsVzPegYellowStone
// ---------------------------------------------------------------------------

EmRegsVzPegYellowStone::EmRegsVzPegYellowStone(void)
    : EmRegsVZNoScreen(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::~EmRegsEZPalmIIIc
// ---------------------------------------------------------------------------

EmRegsVzPegYellowStone::~EmRegsVzPegYellowStone(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetLCDHasFrame
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegYellowStone::GetLCDScreenOn(void) {
    UInt8 portFData = READ_REGISTER(portFData);
    return (portFData & hwrVZYellowPortFLCDPowered) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegYellowStone::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portCData) & hwrVZYellowPortCBacklightOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetSerialPortOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegYellowStone::GetSerialPortOn(int /*uartNum*/) {
    return (READ_REGISTER(portGData) & hwrVZYellowPortGRS232Enable) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetPortInputValue
// ---------------------------------------------------------------------------

uint8 EmRegsVzPegYellowStone::GetPortInputValue(int port) {
    uint8 result = EmRegsVZNoScreen::GetPortInputValue(port);

    if (port == 'B') {
        result |= hwrVZYellowPortBDC_IN;
    }

    if (port == 'F') {
        // Make sure this is always set, or HwrDisplayWake will hang
        //		result |= hwrVZNascaPortFLCDPowered;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetPortInternalValue
// ---------------------------------------------------------------------------

uint8 EmRegsVzPegYellowStone::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZNoScreen::GetPortInternalValue(port);

    if (port == 'D') {
        result = GetKeyBits();

        // Ensure that bit hwrEZPortDDockButton is set.  If it's clear, HotSync
        // will sync via the modem instead of the serial port.
        //
        // Also make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrVZYellowPortDDockButton | hwrVZYellowPortDPowerFail;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsVzPegYellowStone::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.
    UInt8 portCDir = READ_REGISTER(portCDir);
    UInt8 portCData = READ_REGISTER(portCData);

    rows[0] =
        (portCDir & hwrVZYellowPortCKbdRow0) != 0 && (portCData & hwrVZYellowPortCKbdRow0) == 0;
    rows[1] =
        (portCDir & hwrVZYellowPortCKbdRow1) != 0 && (portCData & hwrVZYellowPortCKbdRow1) == 0;
    rows[2] =
        (portCDir & hwrVZYellowPortCKbdRow2) != 0 && (portCData & hwrVZYellowPortCKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetSPI2Slave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVzPegYellowStone::GetSPI2Slave(void) {
    if ((READ_REGISTER(portGData) & hwrVZYellowPortGADC_CS) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmRegsVzPegYellowStone::GetLEDState(void) {
    uint16 result = kLEDOff;
    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVzPegYellowStone::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmRegsVzPegYellowStone::GetVibrateOn(void) { return false; }
