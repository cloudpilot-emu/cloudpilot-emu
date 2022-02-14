#include "EmRegsSZRedwood.h"

#include "EmBankRegs.h"  // EmBankRegs::InvalidAccess
#include "EmCommon.h"
#include "EmMemory.h"
#include "EmRegsSZPrv.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

// Port C Bit settings for Redwood
#define hwrSZRedwoodPortCKbdRow0 0x01  // PC0: KB_ROW0
#define hwrSZRedwoodPortCKbdRow1 0x02  // PC1: KB_ROW1
#define hwrSZRedwoodPortCKbdRow2 0x04  // PC2: KB_ROW2

// Port D Bit settings for Redwood
#define hwrSZRedwoodPortDKbdCol0 0x01  // PD0: KB_COL0
#define hwrSZRedwoodPortDKbdCol1 0x02  // PD1: KB_COL1
#define hwrSZRedwoodPortDKbdCol2 0x04  // PD2: KB_COL2

#define hwrSZRedwoodPortDDockButton 0x10  // PD4: HSYN_IRQ
#define hwrSZRedwoodPortDPowerFail \
    0x80  // PD7: LOWB_IRQ (L) Power Fail Interrupt	(aka IRQ6) (level, low)

// Port E Bit settings for Redwood
//#define	hwrSZRedwoodPortEDC_IN			0x08		// PE3 : CHARGING

// Port F Bit settings for Redwood
#define hwrSZRedwoodPortFRS232Enable 0x01  // PF0: RS232C_SD Enable the RS232 Transceiver

// Port K Bit settings for Redwood
#define hwrSZRedwoodPortKBacklightOn 0x40  // PK6: LIGHT_ON Backlight ON Enable

// Port J Bit settings for Redwood

// Port N Bit settings for Redwood
#define hwrSZRedwoodPortNMS_INS 0x10    // PN4: MS_INS (IRQ3)
#define hwrSZRedwoodPortNAlermLED 0x20  // PN5: ATNLED_ON AlermLED
#define hwrSZRedwoodPortNVibrate 0x80   // PN7: VIBRATE

// Port P Bit settings for Redwood
#define hwrSZRedwoodPortPADC_CNVST 0x40  // PP6: ADC Conversion Start
#define hwrSZRedwoodPortPDC_IN 0x80      // PP7 : CHARGING

const int kNumButtonRows = 3;
const int kNumButtonCols = 3;

#define keyBitJogBack (0x0800)  // JogDial: Back key

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3},
    {keyBitPageUp, keyBitPageDown, 0},
    {keyBitPower, keyBitHard4, keyBitJogBack}};

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::EmRegsSzRedwood
// ---------------------------------------------------------------------------

EmRegsSzRedwood::EmRegsSzRedwood(void)
    : EmRegsSZ(), fSPISlaveADC(new EmSPISlaveADS784x(kChannelSet2)) {}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::~EmRegsSzRedwood
// ---------------------------------------------------------------------------

EmRegsSzRedwood::~EmRegsSzRedwood(void) { delete fSPISlaveADC; }

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsSzRedwood::GetLCDScreenOn(void) {
    // UInt8 portJData = READ_REGISTER(portJData);
    //	return (portCData & 0x01) != 0;
    //	return (portJData & 0x80) != 0;
    return true;
}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsSzRedwood::GetLCDBacklightOn(void) {
    return (READ_REGISTER(portKData) & hwrSZRedwoodPortKBacklightOn) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetSerialPortOn
// ---------------------------------------------------------------------------

Bool EmRegsSzRedwood::GetSerialPortOn(int /*uartNum*/) {
    return (READ_REGISTER(portFData) & hwrSZRedwoodPortFRS232Enable) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetPortInputValue
// ---------------------------------------------------------------------------

uint8 EmRegsSzRedwood::GetPortInputValue(int port) {
    uint8 result = EmRegsSZ::GetPortInputValue(port);

    if (port == 'P') {
        result |= hwrSZRedwoodPortPDC_IN;
    }

    if (port == 'F') {
        // Make sure this is always set, or HwrDisplayWake will hang
        //		result |= hwrVZNascaPortFLCDPowered;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetPortInternalValue
// ---------------------------------------------------------------------------

uint8 EmRegsSzRedwood::GetPortInternalValue(int port) {
    uint8 result = EmRegsSZ::GetPortInternalValue(port);

    if (port == 'D') {
        result = GetKeyBits();

        // Ensure that bit hwrEZPortDDockButton is set.  If it's clear, HotSync
        // will sync via the modem instead of the serial port.
        //
        // Also make sure that hwrEZPortDPowerFail is set.  If it's clear,
        // the battery code will make the device go to sleep immediately.

        result |= hwrSZRedwoodPortDDockButton | hwrSZRedwoodPortDPowerFail;
    }

    if (port == 'C') {
        result = ~result & 0x07;
    }

    if (port == 'G') {
        result |= 0x02;  // LCD_FLIP
    }

    if (port == 'J') {
        result |= 0xFF;  // KBD_COL0-6,LCD_ROTATE
    }
    if (port == 'P') {
        result |= 0x08;  // AREM_PUSH
        result |= 0x02;  // HOLD
    }

    /*	if (port == 'R')
            {
                    result |= 0x3F;		// KBD_ROW0-5
            }*/
    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsSzRedwood::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.
    UInt8 portCDir = READ_REGISTER(portCDir);
    UInt8 portCData = READ_REGISTER(portCData);

    rows[0] =
        (portCDir & hwrSZRedwoodPortCKbdRow0) != 0 && (portCData & hwrSZRedwoodPortCKbdRow0) == 0;
    rows[1] =
        (portCDir & hwrSZRedwoodPortCKbdRow1) != 0 && (portCData & hwrSZRedwoodPortCKbdRow1) == 0;
    rows[2] =
        (portCDir & hwrSZRedwoodPortCKbdRow2) != 0 && (portCData & hwrSZRedwoodPortCKbdRow2) == 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsSzRedwood::GetSPISlave(void) {
    if ((READ_REGISTER(portPData) & hwrSZRedwoodPortPADC_CNVST) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetLEDState
// ---------------------------------------------------------------------------
/*
uint16 EmRegsSzRedwood::GetLEDState (void)
{
        uint16	result		= kLEDOff;
        UInt8	portNData	= READ_REGISTER (portNData);

        if (portNData & hwrSZRedwoodPortNAlermLED)
                result |= (kLEDRed | kLEDGreen);
        return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSzRedwood::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmRegsSzRedwood::GetVibrateOn (void)
{
        UInt8	portNData	= READ_REGISTER (portNData);
        return (portNData & hwrSZRedwoodPortNVibrate);
}
*/
