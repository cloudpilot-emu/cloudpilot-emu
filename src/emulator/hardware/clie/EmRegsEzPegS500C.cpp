#include "EmRegsEzPegS500C.h"

#include "EmCommon.h"
#include "EmRegsEZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "PalmPack.h"

#define NON_PORTABLE
#include "EZAustin/IncsPrv/HardwareAustin.h"  // hwrEZPortCKbdRow0, hwrEZPortBRS232Enable, etc.
#undef NON_PORTABLE
#include "PalmPackPop.h"

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

// Port B Bit settings for PEG_S500C
#define hwrEZPegPortBLCDPowered 0x02
#define hwrEZPegPortBBacklightOn 0x08  // Backlight

// Port C Bit settings for PEG_S500C
#define hwrEZPegPortCLCDEnableOn 0x80  // (H) LCD Enable

// Port D Bit settings for PEG_S500C
#define hwrEZPegPortDMsIns 0x08       // MemoryStick Inserted
#define hwrEZPegPortDMS_IF_Intl 0x40  //
#define hwrEZPegPortDPowerFail 0x80   // (L) Power Fail Interrupt	(aka IRQ6) (level, low)

// Port F Bit settings for PEG_S500C
#define hwrEZPegPortFKbdRow0 0x01
#define hwrEZPegPortFKbdRow1 0x20
#define hwrEZPegPortFKbdRow2 0x40

// Port G Bit settings for PEG_S500C
#define hwrEZPegPortGRS232Enable 0x10  // Enable the RS232 Transceiver
#define hwrEZPegPortGADC_CS 0x08       // ADC chip select

const int kNumButtonRows = 3;
const int kNumButtonCols = 3;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {{keyBitHard1, keyBitHard2, keyBitHard3},
                                                           {keyBitPageUp, keyBitPageDown, 0},
                                                           {keyBitPower, keyBitHard4, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::EmRegsEZPalmIIIc
// ---------------------------------------------------------------------------

EmRegsEzPegS500C::EmRegsEzPegS500C(void)
    : EmRegsEZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmIIIc::~EmRegsEZPalmIIIc
// ---------------------------------------------------------------------------

EmRegsEzPegS500C::~EmRegsEzPegS500C(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS500C::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsEzPegS500C::GetLCDScreenOn(void) {
    return EmHALHandler::GetLCDScreenOn();
    //	return (READ_REGISTER (portCData) & hwrEZPegPortCLCDEnableOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS500C::GetBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsEzPegS500C::GetLCDBacklightOn(void) { return EmHALHandler::GetLCDBacklightOn(); }

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS500C::GetSerialPortOn
// ---------------------------------------------------------------------------

Bool EmRegsEzPegS500C::GetSerialPortOn(int /*uartNum*/) {
    return (READ_REGISTER(portGData) & hwrEZPegPortGRS232Enable) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS500C::GetPortInputValue
// ---------------------------------------------------------------------------

uint8 EmRegsEzPegS500C::GetPortInputValue(int port) {
    uint8 result = EmRegsEZ::GetPortInputValue(port);

    if (port == 'B') {
        // Make sure this is always set, or HwrDisplayWake will hang
        result |= hwrEZPegPortBLCDPowered;
    }

    if (port == 'D') {
        result |= hwrEZPegPortDMsIns;
    }
    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS500C::GetPortInternalValue
// ---------------------------------------------------------------------------

uint8 EmRegsEzPegS500C::GetPortInternalValue(int port) {
    uint8 result = EmRegsEZ::GetPortInternalValue(port);

    if (port == 'D') {
        result = GetKeyBits();

        // Ensure that bit hwrEZPortDDockButton is set.  If it's clear, HotSync
        // will sync via the modem instead of the serial port.
        //
        // Also make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrEZPortDDockButton | hwrEZPegPortDPowerFail;

        result |= hwrEZPegPortDMS_IF_Intl;
        result |= hwrEZPegPortDMsIns;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS500C::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsEzPegS500C::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.
    UInt8 portFDir = READ_REGISTER(portFDir);
    UInt8 portFData = READ_REGISTER(portFData);

    rows[0] = (portFDir & hwrEZPegPortFKbdRow0) != 0 && (portFData & hwrEZPegPortFKbdRow0) == 0;
    rows[1] = (portFDir & hwrEZPegPortFKbdRow1) != 0 && (portFData & hwrEZPegPortFKbdRow1) == 0;
    rows[2] = (portFDir & hwrEZPegPortFKbdRow2) != 0 && (portFData & hwrEZPegPortFKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS500C::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsEzPegS500C::GetSPISlave(void) {
    if ((READ_REGISTER(portGData) & hwrEZPegPortGADC_CS) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

void EmRegsEzPegS500C::MarkScreen() {}

void EmRegsEzPegS500C::UnmarkScreen() {}
