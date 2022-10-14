#include "EmRegsSonyDSP.h"

#include <iomanip>

#include "EmMemory.h"
#include "UAE.h"

#define READ_REGISTER(r) DoStdRead(baseAddress + r, 2)
#define WRITE_REGISTER(r, v) DoStdWrite(baseAddress + r, 2, v)

#define INSTALL_HANDLER(read, write, offset, size)                                       \
    SetHandler((ReadFunction)&EmRegsSonyDSP::read, (WriteFunction)&EmRegsSonyDSP::write, \
               baseAddress + offset, size)

namespace {
    constexpr emuptr REG_IPC_RESULT = 0x0c06;

    string trace(emuptr pc) {
        emuptr rtsAddr;

        for (rtsAddr = pc; rtsAddr < pc + 0x0400; rtsAddr += 2) {
            if (EmMemGet16(rtsAddr) == 0x4e75) break;
        }

        if (rtsAddr >= pc + 0x0400) return "[unknown 1]";

        char name[64];
        memset(name, 0, sizeof(name));

        size_t i;
        for (i = 0; i < sizeof(name) - 1; i++) {
            unsigned char c = EmMemGet8(rtsAddr + 3 + i);

            if (c >= 128) return "[unknown 2]";
            if (c == 0) break;

            name[i] = c;
        }

        return i == sizeof(name) ? "[unknown 3]" : name;
    }
}  // namespace

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
#if 0
    if (address - baseAddress < 0x00008000 && size == 2)
        cerr << "DSP register read 0x" << hex << right << setfill('0') << setw(8) << address
             << " -> 0x" << setw(4) << DoStdRead(address, size) << dec << " (" << size
             << " bytes) from " << trace(::regs.pc) << " : 0x" << hex << ::regs.pc << dec << endl
             << flush;
#endif

    return DoStdRead(address, size);
}

void EmRegsSonyDSP::StdWrite(emuptr address, int size, uint32 value) {
    if (address - baseAddress < 0x00008000)
        cerr << "DSP register write 0x" << hex << right << setfill('0') << setw(8) << address
             << " <- 0x" << setw(4) << value << dec << " (" << size << " bytes) from "
             << trace(::regs.pc) << " : 0x" << hex << ::regs.pc << dec << endl
             << flush;

    DoStdWrite(address, size, value);
}

uint32 EmRegsSonyDSP::DoStdRead(emuptr address, int size) {
    uint8* realAddr = this->regs + (address - baseAddress);

    if (size == 1) return EmMemDoGet8(realAddr);

    if (size == 2) return EmMemDoGet16(realAddr);

    return EmMemDoGet32(realAddr);
}

void EmRegsSonyDSP::DoStdWrite(emuptr address, int size, uint32 value) {
    uint8* realAddr = this->regs + (address - baseAddress);

    if (size == 1)
        EmMemDoPut8(realAddr, value);

    else if (size == 2)
        EmMemDoPut16(realAddr, value);

    else
        EmMemDoPut32(realAddr, value);
}
