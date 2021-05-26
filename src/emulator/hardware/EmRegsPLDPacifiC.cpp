#include "EmRegsPLDPacifiC.h"

EmRegsPLDPacifiC::EmRegsPLDPacifiC(emuptr baseAddress) : EmRegsPLDAtlantiC(baseAddress) {}

void EmRegsPLDPacifiC::SetSubBankHandlers() {
    EmRegsPLDAtlantiC::SetSubBankHandlers();

    SetHandler((ReadFunction)&EmRegsPLDPacifiC::ReadReg_0x34,
               (WriteFunction)&EmRegsPLDPacifiC::StdWriteBE, baseAddress + 0x34, 2);
}

uint32 EmRegsPLDPacifiC::ReadReg_0x34(emuptr address, int size) {
    return EmRegs::StdReadBE(address, size) | 0x80;
}
