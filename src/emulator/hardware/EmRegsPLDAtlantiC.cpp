#include "EmRegsPLDAtlantiC.h"

EmRegsPLDAtlantiC::EmRegsPLDAtlantiC(emuptr baseAddress) : baseAddress(baseAddress) {}

void EmRegsPLDAtlantiC::Reset(Bool hardwareReset) {
    if (hardwareReset) memset(regs, 0, sizeof(regs));
}

void EmRegsPLDAtlantiC::SetSubBankHandlers() {
    EmRegs::SetSubBankHandlers();

    for (int i = 0; i < 0x15 * 2; i++) {
        switch (i * 2) {
            case 0x06:
                SetHandler((ReadFunction)&EmRegsPLDAtlantiC::ReadReg_0x06,
                           (WriteFunction)&EmRegsPLDAtlantiC::StdWriteBE, baseAddress + 2 * i, 2);
                break;

            case 0x2e:
                SetHandler((ReadFunction)&EmRegsPLDAtlantiC::ReadReg_0x2e,
                           (WriteFunction)&EmRegsPLDAtlantiC::StdWriteBE, baseAddress + 2 * i, 2);

                break;

            case 0x50:
                SetHandler((ReadFunction)&EmRegsPLDAtlantiC::ReadReg_0x50,
                           (WriteFunction)&EmRegsPLDAtlantiC::StdWriteBE, baseAddress + 2 * i, 2);

                break;

            default:
                SetHandler((ReadFunction)&EmRegsPLDAtlantiC::StdReadBE,
                           (WriteFunction)&EmRegsPLDAtlantiC::StdWriteBE, baseAddress + 2 * i, 2);
        }
    }
}

uint8* EmRegsPLDAtlantiC::GetRealAddress(emuptr address) {
    return reinterpret_cast<uint8*>(regs) + (address - baseAddress);
}

emuptr EmRegsPLDAtlantiC::GetAddressStart() { return baseAddress; }

uint32 EmRegsPLDAtlantiC::GetAddressRange() { return sizeof(regs); }

uint32 EmRegsPLDAtlantiC::ReadReg_0x06(emuptr address, int size) { return 0xffffffff; }

uint32 EmRegsPLDAtlantiC::ReadReg_0x2e(emuptr address, int size) {
    uint8 result = EmRegs::StdReadBE(address, size);

    if ((size == 2) || (size == 1 && (address & 0x01))) result |= 0x80;

    return result;
}

uint32 EmRegsPLDAtlantiC::ReadReg_0x50(emuptr address, int size) { return 0xffffffff; }
