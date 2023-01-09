#include "EmRegs330cStubs.h"

namespace {
    constexpr emuptr MEMORY_START = 0x10a00000;
    constexpr uint32 MEMORY_SIZE = 16;
}  // namespace

uint8* EmRegs330cStubs::GetRealAddress(emuptr address) {
    EmAssert(false);

    return nullptr;
}

emuptr EmRegs330cStubs::GetAddressStart(void) { return MEMORY_START; }

uint32 EmRegs330cStubs::GetAddressRange(void) { return MEMORY_SIZE; }

void EmRegs330cStubs::SetSubBankHandlers(void) {
    SetHandler((ReadFunction)&EmRegs330cStubs::stubRead, (WriteFunction)&EmRegs330cStubs::stubWrite,
               MEMORY_START, MEMORY_SIZE);
}

uint32 EmRegs330cStubs::stubRead(emuptr address, int size) { return 0; }

void EmRegs330cStubs::stubWrite(emuptr address, int size, uint32 value) {}
