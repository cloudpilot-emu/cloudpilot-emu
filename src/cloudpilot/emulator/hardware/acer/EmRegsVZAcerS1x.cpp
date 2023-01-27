#include "EmRegsVZAcerS1x.h"

#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"

namespace {
    constexpr uint8 hwrVZPortDPowerFail = 0x80;
    constexpr int kNumButtonRows = 3;
    constexpr int kNumButtonCols = 4;

    const uint16 kGenericMonoMap[kNumButtonRows][kNumButtonCols] = {
        {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
        {keyBitPageUp, keyBitPageDown, 0, 0},
        {keyBitPower, keyBitContrast, keyBitHard2, 0}};

    constexpr uint8 hwrVZPortKKbdRow0 = 0x20;
    constexpr uint8 hwrVZPortKKbdRow1 = 0x40;
    constexpr uint8 hwrVZPortKKbdRow2 = 0x80;

}  // namespace

EmRegsVZAcerS1x::EmRegsVZAcerS1x() { fSPISlaveADC = new EmSPISlaveADS784x(kChannelSet1); }

EmRegsVZAcerS1x::~EmRegsVZAcerS1x() { delete fSPISlaveADC; }

Bool EmRegsVZAcerS1x::GetLCDScreenOn() { return true; }

Bool EmRegsVZAcerS1x::GetLCDBacklightOn() { return true; }

void EmRegsVZAcerS1x::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kGenericMonoMap, sizeof(kGenericMonoMap));

    // Determine what row is being asked for.

    UInt8 portKDir = READ_REGISTER(portKDir);
    UInt8 portKData = READ_REGISTER(portKData);

    rows[0] = (portKDir & hwrVZPortKKbdRow0) != 0 && (portKData & hwrVZPortKKbdRow0) == 0;
    rows[1] = (portKDir & hwrVZPortKKbdRow1) != 0 && (portKData & hwrVZPortKKbdRow1) == 0;
    rows[2] = (portKDir & hwrVZPortKKbdRow2) != 0 && (portKData & hwrVZPortKKbdRow2) == 0;
}

uint8 EmRegsVZAcerS1x::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZ::GetPortInternalValue(port);

    if (port == 'D') {
        result |= hwrVZPortDPowerFail;
    }

    return result;
}

EmSPISlave* EmRegsVZAcerS1x::GetSPI2Slave() { return fSPISlaveADC; }
