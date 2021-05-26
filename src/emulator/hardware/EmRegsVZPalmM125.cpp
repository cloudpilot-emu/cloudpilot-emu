#include "EmRegsVZPalmM125.h"

#include "EmCommon.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x

constexpr int kNumButtonRows = 3;
constexpr int kNumButtonCols = 4;

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

constexpr uint16 kButtonMapPalmM125[kNumButtonRows][kNumButtonCols] = {
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
