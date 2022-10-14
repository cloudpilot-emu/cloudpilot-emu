#include "EmRegsSonyDSP.h"

#include "EmMemory.h"

#define READ_REGISTER(r) StdRead(baseAddress + r, 2)
#define WRITE_REGISTER(r, v) StdWrite(baseAddress + r, 2, v)

#define INSTALL_HANDLER(read, write, offset, size)                                       \
    SetHandler((ReadFunction)&EmRegsSonyDSP::read, (WriteFunction)&EmRegsSonyDSP::write, \
               baseAddress + offset, size)

namespace {
    constexpr emuptr REG_IPC_RESULT = 0x0c06;
}

EmRegsSonyDSP::EmRegsSonyDSP(emuptr baseAddress) : baseAddress(baseAddress) {}

void EmRegsSonyDSP::Reset(Bool hardwareReset) {
    if (!hardwareReset) return;

    memset(regs, 0, sizeof(regs));

    WRITE_REGISTER(REG_IPC_RESULT, 0xfc00);
}

uint8* EmRegsSonyDSP::GetRealAddress(emuptr address) {
    // We don't support direct access from outside.
    EmAssert(false);

    return 0;
}

emuptr EmRegsSonyDSP::GetAddressStart(void) { return baseAddress; }

uint32 EmRegsSonyDSP::GetAddressRange(void) { return sizeof(regs); }

void EmRegsSonyDSP::SetSubBankHandlers(void) {
    INSTALL_HANDLER(StdRead, StdWrite, 0, sizeof(regs));
}

uint32 EmRegsSonyDSP::StdRead(emuptr address, int size) {
    uint8* realAddr = this->regs + (address - baseAddress);

    if (size == 1) return EmMemDoGet8(realAddr);

    if (size == 2) return EmMemDoGet16(realAddr);

    return EmMemDoGet32(realAddr);
}

void EmRegsSonyDSP::StdWrite(emuptr address, int size, uint32 value) {
    uint8* realAddr = this->regs + (address - baseAddress);

    if (size == 1)
        EmMemDoPut8(realAddr, value);

    else if (size == 2)
        EmMemDoPut16(realAddr, value);

    else
        EmMemDoPut32(realAddr, value);
}
