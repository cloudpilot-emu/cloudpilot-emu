#include "EmRegsVZAtlantiC.h"

#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"
#include "ExternalStorage.h"

using VZ = EMPalmVZWithSD<EmRegsVZNoScreen>;

namespace {
    constexpr int kNumButtonRows = 3;
    constexpr int kNumButtonCols = 4;
    constexpr int hwrVZPalmI710PortKKbdRow0 = 0x01;
    constexpr int hwrVZPalmI710PortKKbdRow1 = 0x02;
    constexpr int hwrVZPalmI710PortKKbdRow2 = 0x04;
    constexpr int hwrVZPalmI710PortG232_SHDN_N = 0x08;
    constexpr int hwrVZPalmI710PortMIR_SD = 0x20;
    constexpr int hwrVZPalmM130PortGADC_CS_N = 0x20;

    constexpr uint16 kButtonMapPalmI710[kNumButtonRows][kNumButtonCols] = {
        {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
        {keyBitPageUp, keyBitPageDown, 0, 0},
        {keyBitPower, keyBitContrast, 0, 0}};
}  // namespace

EmRegsVZAtlantiC::EmRegsVZAtlantiC() { fSPISlaveADC = new EmSPISlaveADS784x(kChannelSet1); }

EmRegsVZAtlantiC::~EmRegsVZAtlantiC() { delete fSPISlaveADC; }

Bool EmRegsVZAtlantiC::GetLCDScreenOn() { return (READ_REGISTER(portGData) & 0x04) != 0; }

Bool EmRegsVZAtlantiC::GetLCDBacklightOn() { return true; }

uint8 EmRegsVZAtlantiC::GetPortInputValue(int port) {
    uint32 result = VZ::GetPortInputValue(port);

    if (port == 'B') return (result & 0xff) | 0x40;

    return result;
}

uint8 EmRegsVZAtlantiC::GetPortInternalValue(int port) {
    uint32 result = VZ::GetPortInternalValue(port);

    if (port == 'D') result |= 0x80;

    return result;
}

void EmRegsVZAtlantiC::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMapPalmI710, sizeof(kButtonMapPalmI710));

    // Determine what row is being asked for.

    UInt8 portCDir = READ_REGISTER(portCDir);
    UInt8 portCData = READ_REGISTER(portCData);

    rows[0] =
        (portCDir & hwrVZPalmI710PortKKbdRow0) != 0 && (portCData & hwrVZPalmI710PortKKbdRow0) == 0;
    rows[1] =
        (portCDir & hwrVZPalmI710PortKKbdRow1) != 0 && (portCData & hwrVZPalmI710PortKKbdRow1) == 0;
    rows[2] =
        (portCDir & hwrVZPalmI710PortKKbdRow2) != 0 && (portCData & hwrVZPalmI710PortKKbdRow2) == 0;
}

Bool EmRegsVZAtlantiC::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portGData) & hwrVZPalmI710PortG232_SHDN_N) != 0;

    if (type == kUARTIR) return (READ_REGISTER(portMData) & hwrVZPalmI710PortMIR_SD) == 0;

    return false;
}

EmUARTDeviceType EmRegsVZAtlantiC::GetUARTDevice(int uartNum) {
    Bool serEnabled = this->GetLineDriverState(kUARTSerial);
    Bool irEnabled = this->GetLineDriverState(kUARTIR);

    if (uartNum == 0) {
        if (serEnabled) return kUARTSerial;

        if (irEnabled) return kUARTIR;
    }

    return kUARTNone;
}

Bool EmRegsVZAtlantiC::GetDTR(int uartNum) {
    if (uartNum == kUARTIR) {
        return (READ_REGISTER(portGData) & 0x10) != 0;
    }

    return false;
}

Bool EmRegsVZAtlantiC::GetVibrateOn(void) { return (READ_REGISTER(portFData) & 0x01) != 0; }

EmSPISlave* EmRegsVZAtlantiC::GetSPI2Slave(void) {
    if ((READ_REGISTER(portGData) & hwrVZPalmM130PortGADC_CS_N) == 0) {
        return fSPISlaveADC;
    }

    return NULL;
}
