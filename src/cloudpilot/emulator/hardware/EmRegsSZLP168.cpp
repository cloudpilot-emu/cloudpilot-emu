#include "EmRegsSZLP168.h"

#include "EmRegsSZPrv.h"
#include "EmSPISlaveADS784x.h"

namespace {
    constexpr const int kNumButtonRows = 2;
    constexpr const int kNumButtonCols = 4;

    const uint16 kKeyMap[kNumButtonRows][kNumButtonCols] = {
        {keyBitPageUp, keyBitHard1, keyBitHard2, keyBitPower},
        {keyBitPageDown, keyBitHard3, keyBitHard4, 0}};

    constexpr uint8_t hwrPortCKbdRow0 = 0x01;
    constexpr uint8_t hwrPortCKbdRow1 = 0x02;
}  // namespace

EmRegsSZLP168::EmRegsSZLP168() { revision = Revision::rev12; }

EmRegsSZLP168::~EmRegsSZLP168(void) { delete fSPISlaveADC; }

Bool EmRegsSZLP168::GetLCDScreenOn(void) { return true; }

Bool EmRegsSZLP168::GetLCDBacklightOn(void) { return true; }

uint8 EmRegsSZLP168::GetPortInputValue(int port) {
    switch (port) {
        case 'G':
            return 0x3C;  // 0x34: jog wheel , 0x08: card sense

        default:
            return EmRegsSZ::GetPortInputValue(port);
    }
}

void EmRegsSZLP168::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kKeyMap, sizeof(kKeyMap));

    // Determine what row is being asked for.

    UInt8 portJDir = READ_REGISTER(portJDir);
    UInt8 portJData = READ_REGISTER(portJData);

    rows[0] = (portJDir & hwrPortCKbdRow0) != 0 && (portJData & hwrPortCKbdRow0) == 0;
    rows[1] = (portJDir & hwrPortCKbdRow1) != 0 && (portJData & hwrPortCKbdRow1) == 0;
}

uint8 EmRegsSZLP168::GetKeyBits() { return EmRegsSZ::GetKeyBits() << 4; }

uint16 EmRegsSZLP168::GetADCValueU() {
    if (READ_REGISTER(portGData) & 0x02) {
        return 1024 << 3;
    }

    if (READ_REGISTER(portBData) & 0x20) {
        return 0;
    }

    if (READ_REGISTER(portKData) & 0x02) {
        return 1024 << 3;
    }

    return 0;
}
