#include "EmRegs330CPLDMirror.h"

#include "EmMemory.h"
#include "EmRegs330CPLD.h"

namespace {
    constexpr emuptr MEMORY_START = 0x10a00000;
}  // namespace

uint8* EmRegs330CPLDMirror::GetRealAddress(emuptr address) {
    EmAssert(false);

    return nullptr;
}

emuptr EmRegs330CPLDMirror::GetAddressStart(void) { return MEMORY_START; }

uint32 EmRegs330CPLDMirror::GetAddressRange(void) { return kMemorySizeCPLD; }

void EmRegs330CPLDMirror::SetSubBankHandlers(void) {
    SetHandler((ReadFunction)&EmRegs330CPLDMirror::stubRead,
               (WriteFunction)&EmRegs330CPLDMirror::stubWrite, MEMORY_START, kMemorySizeCPLD);
}

uint32 EmRegs330CPLDMirror::stubRead(emuptr address, int size) {
    emuptr mirrorAddress = address - MEMORY_START + kMemoryStartCPLD330c;

    switch (size) {
        case 1:
            return EmMemGet8(mirrorAddress);

        case 2:
            return EmMemGet16(mirrorAddress);

        case 4:
            return EmMemGet32(mirrorAddress);

        default:
            EmAssert(false);
            return 0;
    }
}

void EmRegs330CPLDMirror::stubWrite(emuptr address, int size, uint32 value) {
    emuptr mirrorAddress = address - MEMORY_START + kMemoryStartCPLD330c;

    switch (size) {
        case 1:
            return EmMemPut8(mirrorAddress, value);

        case 2:
            return EmMemPut16(mirrorAddress, value);

        case 4:
            return EmMemPut32(mirrorAddress, value);

        default:
            EmAssert(false);
    }
}
