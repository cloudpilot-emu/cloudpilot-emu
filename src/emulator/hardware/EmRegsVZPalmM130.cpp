#include "EmRegsVZPalmM130.h"

#include "EmCommon.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x

constexpr int kNumButtonRows = 3;
constexpr int kNumButtonCols = 4;

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

constexpr uint16 kButtonMapPalmM130[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {0, keyBitPageDown, 0, 0},
    {keyBitPower, keyBitPageUp, 0, 0}};

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM130::EmRegsVZPalmM130
// ---------------------------------------------------------------------------

EmRegsVZPalmM130::EmRegsVZPalmM130(void)
    : EmRegsVZNoScreen(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet1)) {}

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
