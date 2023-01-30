#include "EmRegsVZAcerS1x.h"

#include "EmRegsVZPrv.h"
#include "EmSPISlaveADS784x.h"
#include "ExternalStorage.h"

namespace {
    constexpr uint8 hwrVZPortDPowerFail = 0x80;
    constexpr int kNumButtonRows = 3;
    constexpr int kNumButtonCols = 4;

    const uint16 kGenericMonoMap[kNumButtonRows][kNumButtonCols] = {
        {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
        {keyBitPageUp, keyBitPageDown, 0, 0},
        {keyBitPower, 0, 0, 0}};

    constexpr uint8 hwrVZPortKKbdRow0 = 0x01;
    constexpr uint8 hwrVZPortKKbdRow1 = 0x02;
    constexpr uint8 hwrVZPortKKbdRow2 = 0x04;

}  // namespace

EmRegsVZAcerS1x::EmRegsVZAcerS1x(EmRegsMB86189& mb86189) : mb86189(mb86189) {
    fSPISlaveADC = new EmSPISlaveADS784x(kChannelSet1);

    mb86189.SetGpioReadHandler(
        []() { return gExternalStorage.IsMounted(EmHAL::Slot::memorystick) ? 0x01 : 0x00; });
    mb86189.irqChange.AddHandler([=](bool newState) { UpdateIRQ3(newState ? 0x40 : 0x00); });
}

EmRegsVZAcerS1x::~EmRegsVZAcerS1x() { delete fSPISlaveADC; }

Bool EmRegsVZAcerS1x::GetLCDScreenOn() { return true; }

Bool EmRegsVZAcerS1x::GetLCDBacklightOn() { return true; }

void EmRegsVZAcerS1x::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kGenericMonoMap, sizeof(kGenericMonoMap));

    // Determine what row is being asked for.

    UInt8 portJDir = READ_REGISTER(portJDir);
    UInt8 portJData = READ_REGISTER(portJData);

    rows[0] = (portJDir & hwrVZPortKKbdRow0) != 0 && (portJData & hwrVZPortKKbdRow0) == 0;
    rows[1] = (portJDir & hwrVZPortKKbdRow1) != 0 && (portJData & hwrVZPortKKbdRow1) == 0;
    rows[2] = (portJDir & hwrVZPortKKbdRow2) != 0 && (portJData & hwrVZPortKKbdRow2) == 0;
}

uint8 EmRegsVZAcerS1x::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZ::GetPortInternalValue(port);

    if (port == 'D') {
        result |= hwrVZPortDPowerFail;

        if (mb86189.GetIrq()) {
            result &= ~0x40;
        } else
            result |= 0x40;
    }

    return result;
}

void EmRegsVZAcerS1x::Mount(EmHAL::Slot slot, CardImage& cardImage) {
    if (this->GetNextHandler()) this->GetNextHandler()->Mount(slot, cardImage);
    if (slot != EmHAL::Slot::memorystick) return;

    mb86189.NotifyGpioChanged();
}

void EmRegsVZAcerS1x::Unmount(EmHAL::Slot slot) {
    if (this->GetNextHandler()) this->GetNextHandler()->Unmount(slot);
    if (slot != EmHAL::Slot::memorystick) return;

    mb86189.NotifyGpioChanged();
}

EmSPISlave* EmRegsVZAcerS1x::GetSPI2Slave() { return fSPISlaveADC; }
