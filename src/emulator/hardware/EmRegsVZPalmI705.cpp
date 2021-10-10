#include "EmRegsVZPalmI705.h"

#include "EmCommon.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x

constexpr int kNumButtonRows = 3;
constexpr int kNumButtonCols = 4;

constexpr uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
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

EmRegsVZPalmI705::EmRegsVZPalmI705(void) : EmRegsVZ() {
    fSPISlaveADC = new EmSPISlaveADS784x(kChannelSet1);
}

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
