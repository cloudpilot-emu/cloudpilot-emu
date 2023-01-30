#include "EmRegsAcerDSPStub.h"

namespace {
    constexpr emuptr BASE_ADDRESS = 0x10800000;
    constexpr uint32 SIZE = 16;
}  // namespace

uint8* EmRegsAcerDSPStub::GetRealAddress(emuptr address) {
    EmAssert(false);

    return nullptr;
}

emuptr EmRegsAcerDSPStub::GetAddressStart(void) { return BASE_ADDRESS; }

uint32 EmRegsAcerDSPStub::GetAddressRange(void) { return SIZE; }

void EmRegsAcerDSPStub::SetSubBankHandlers(void) {
    SetHandler((ReadFunction)&EmRegsAcerDSPStub::stubRead,
               (WriteFunction)&EmRegsAcerDSPStub::stubWrite, BASE_ADDRESS, SIZE);
}

uint32 EmRegsAcerDSPStub::stubRead(emuptr address, int size) { return 0; }

void EmRegsAcerDSPStub::stubWrite(emuptr address, int size, uint32 value) {}
