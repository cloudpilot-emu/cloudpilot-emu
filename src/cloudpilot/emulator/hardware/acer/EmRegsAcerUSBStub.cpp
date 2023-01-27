#include "EmRegsAcerUSBStub.h"

namespace {
    constexpr emuptr BASE_ADDRESS = 0x1f000000;
    constexpr uint32 SIZE = 16;
}  // namespace

uint8* EmRegsAcerUSBStub::GetRealAddress(emuptr address) {
    EmAssert(false);

    return nullptr;
}

emuptr EmRegsAcerUSBStub::GetAddressStart(void) { return BASE_ADDRESS; }

uint32 EmRegsAcerUSBStub::GetAddressRange(void) { return SIZE; }

void EmRegsAcerUSBStub::SetSubBankHandlers(void) {
    SetHandler((ReadFunction)&EmRegsAcerUSBStub::stubRead,
               (WriteFunction)&EmRegsAcerUSBStub::stubWrite, BASE_ADDRESS, SIZE);
}

uint32 EmRegsAcerUSBStub::stubRead(emuptr address, int size) { return 0; }

void EmRegsAcerUSBStub::stubWrite(emuptr address, int size, uint32 value) {}
