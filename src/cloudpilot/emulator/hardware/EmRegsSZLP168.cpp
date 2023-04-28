#include "EmRegsSZLP168.h"

#include "EmRegsSZPrv.h"
#include "EmSPISlaveADS784x.h"

namespace {
    constexpr const int kNumButtonRows = 2;
    constexpr const int kNumButtonCols = 4;

    const uint16 kKeyMap[kNumButtonRows][kNumButtonCols] = {
        {keyBitPageDown, keyBitHard1, keyBitHard2, keyBitPower},
        {keyBitPageUp, keyBitHard3, keyBitHard4, 0}};

    constexpr uint8_t hwrPortCKbdRow0 = 0x01;
    constexpr uint8_t hwrPortCKbdRow1 = 0x02;
}  // namespace

EmRegsSZLP168::EmRegsSZLP168() { revision = Revision::rev12; }

EmRegsSZLP168::~EmRegsSZLP168(void) { delete fSPISlaveADC; }

Bool EmRegsSZLP168::GetLCDScreenOn(void) { return true; }

Bool EmRegsSZLP168::GetLCDBacklightOn(void) { return true; }

void EmRegsSZLP168::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kKeyMap, sizeof(kKeyMap));

    // Determine what row is being asked for.

    UInt8 portCDir = READ_REGISTER(portCDir);
    UInt8 portCData = READ_REGISTER(portCData);

    rows[0] = (portCDir & hwrPortCKbdRow0) != 0 && (portCData & hwrPortCKbdRow0) == 0;
    rows[1] = (portCDir & hwrPortCKbdRow1) != 0 && (portCData & hwrPortCKbdRow1) == 0;
}

uint8 EmRegsSZLP168::GetKeyBits() { return EmRegsSZ::GetKeyBits() << 4; }
