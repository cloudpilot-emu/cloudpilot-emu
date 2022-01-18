#include "EmRegsEzPegS300.h"

#include "EmCommon.h"
#include "EmDevice.h"  // HardwareSubID
#include "EmRegsEZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "EmSession.h"          // GetDevice
#include "PalmPack.h"

#define NON_PORTABLE
#include "EZSumo/IncsPrv/HardwareEZ.h"  // hwrEZPortCLCDEnableOn, etc.
#undef NON_PORTABLE
#include "PalmPackPop.h"

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

// Port B Bit settings for PEG_S300
#define hwrEZPegPortBLCDPowered 0x02
#define hwrEZPegPortBBacklightOn 0x08  // Backlight

// Port C Bit settings for PEG_S300
#define hwrEZPegPortCLCDEnableOn 0x80  // (H) LCD Enable

// Port D Bit settings for PEG_S300
#define hwrEZPegPortDMsIns 0x08       // MemoryStick Inserted
#define hwrEZPegPortDMS_IF_Intl 0x40  //
#define hwrEZPegPortDPowerFail 0x80   // (L) Power Fail Interrupt	(aka IRQ6) (level, low)

// Port F Bit settings for PEG_S300
#define hwrEZPegPortFKbdRow0 0x01
#define hwrEZPegPortFKbdRow1 0x20
#define hwrEZPegPortFKbdRow2 0x40

// Port G Bit settings for PEG_S300
#define hwrEZPegPortGRS232Enable 0x10  // Enable the RS232 Transceiver
#define hwrEZPegPortGADC_CS 0x08       //

const int kNumButtonRows = 3;
const int kNumButtonCols = 3;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {{keyBitHard1, keyBitHard2, keyBitHard3},
                                                           {keyBitPageUp, keyBitPageDown, 0},
                                                           {keyBitPower, keyBitHard4, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::EmRegsEZPalmV
// ---------------------------------------------------------------------------

EmRegsEzPegS300::EmRegsEzPegS300(void)
    : EmRegsEZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmV::~EmRegsEZPalmV
// ---------------------------------------------------------------------------

EmRegsEzPegS300::~EmRegsEzPegS300(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS300::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsEzPegS300::GetLCDScreenOn(void) {
    return (READ_REGISTER(portCData) & hwrEZPortCLCDEnableOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS300::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsEzPegS300::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portFData) & hwrEZPegPortBBacklightOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS300::GetSerialPortOn
// ---------------------------------------------------------------------------

Bool EmRegsEzPegS300::GetSerialPortOn(int uartNum) {
    return (READ_REGISTER(portGData) & hwrEZPegPortGRS232Enable) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS300::GetPortInputValue
// ---------------------------------------------------------------------------

uint8 EmRegsEzPegS300::GetPortInputValue(int port) {
    uint8 result = EmRegsEZ::GetPortInputValue(port);

    if (port == 'B') {
        // Make sure this is always set, or HwrDisplayWake will hang
        result |= hwrEZPegPortBLCDPowered;
    }

    if (port == 'D') {
        result |= hwrEZPegPortDMsIns;
    }

#if 0
	if (port == 'E')
	{
		// Return the hardware sub-ID.

		EmDevice	device	= Emulator::GetDevice ();
		result |= ~device.HardwareSubID ();
	}
#endif

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS300::GetPortInternalValue
// ---------------------------------------------------------------------------

uint8 EmRegsEzPegS300::GetPortInternalValue(int port) {
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

        //		if (g_nCardInserted)
        //			result &= ~hwrEZPegPortDMsIns;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsEzPegS300::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsEzPegS300::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
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
//		� EmRegsEzPegS300::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsEzPegS300::GetSPISlave(void) {
    if ((READ_REGISTER(portGData) & hwrEZPegPortGADC_CS) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}
