#include "EmRegsSonyDSP.h"

#include <iomanip>

#include "EmMemory.h"
#include "UAE.h"

#define LOGGING

#ifdef LOGGING
    #define LOG_WRITE_ACCESS logWriteAccess
    #define LOG_READ_ACCESS logReadAccess
#else
    #define LOG_WRITE_ACCESS(address, size, value)
    #define LOG_READ_ACCESS(address, size, value)
#endif

#define READ_REGISTER(r) DoStdRead(baseAddress + r, 2)
#define WRITE_REGISTER(r, v) DoStdWrite(baseAddress + r, 2, v)

#define INSTALL_HANDLER(read, write, offset, size)                                       \
    SetHandler((ReadFunction)&EmRegsSonyDSP::read, (WriteFunction)&EmRegsSonyDSP::write, \
               baseAddress + offset, size)

namespace {
    constexpr emuptr REG_IPC_COMMAND = 0x0c04;
    constexpr emuptr REG_IPC_STATUS = 0x0c06;

    constexpr emuptr REG_IPC_RESULT_1 = 0x0c14;
    constexpr emuptr REG_IPC_RESULT_2 = 0x0c16;
    constexpr emuptr REG_IPC_RESULT_3 = 0x0c18;
    constexpr emuptr REG_IPC_RESULT_4 = 0x0c1a;
    constexpr emuptr REG_IPC_RESULT_5 = 0x0c1c;
    constexpr emuptr REG_IPC_RESULT_6 = 0x0c1e;

    constexpr emuptr REG_INT_STATUS = 0x0220;
    constexpr emuptr REG_INT_MASK = 0x0222;

    constexpr uint16 INT_IPC_DONE = 0x0001;
    constexpr uint16 INT_MS_INSERT = 0x0004;
    constexpr uint16 INT_MS_EJECT = 0x0008;

#ifdef LOGGING
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

    void logWriteAccess(emuptr address, int size, uint32 value) {
        cerr << "DSP register write 0x" << hex << right << setfill('0') << setw(8) << address
             << " <- 0x" << setw(4) << value << dec << " (" << size << " bytes) from "
             << trace(::regs.pc) << " : 0x" << hex << ::regs.pc << dec << endl
             << flush;
    }

    void logReadAccess(emuptr address, int size, uint32 value) {
        cerr << "DSP register read 0x" << hex << right << setfill('0') << setw(8) << address
             << " -> 0x" << setw(4) << value << dec << " (" << size << " bytes) from "
             << trace(::regs.pc) << " : 0x" << hex << ::regs.pc << dec << endl
             << flush;
    }
#endif
}  // namespace

EmRegsSonyDSP::EmRegsSonyDSP(emuptr baseAddress) : baseAddress(baseAddress) {}

void EmRegsSonyDSP::Reset(Bool hardwareReset) {
    if (!hardwareReset) return;

    memset(regs, 0, sizeof(regs));

    WRITE_REGISTER(REG_IPC_STATUS, 0xfc00);
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

    INSTALL_HANDLER(StdRead, IpcCmdWrite, REG_IPC_COMMAND, 2);
    INSTALL_HANDLER(StdRead, IpcStatusWrite, REG_IPC_STATUS, 2);
    INSTALL_HANDLER(StdRead, IrqMaskWrite, REG_INT_MASK, 2);
    INSTALL_HANDLER(StdRead, IrqStatusWrite, REG_INT_STATUS, 2);
}

bool EmRegsSonyDSP::SupportsImageInSlot(EmHAL::Slot slot, const CardImage& cardImage) {
    return slot == EmHAL::Slot::memorystick &&
           MemoryStick::IsSizeRepresentable(cardImage.BlocksTotal());
}

void EmRegsSonyDSP::Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) {
    if (slot != EmHAL::Slot::memorystick) return;

    cout << hex << "0x" << READ_REGISTER(REG_INT_STATUS) << " 0x" << READ_REGISTER(REG_INT_MASK)
         << dec << endl
         << flush;

    RaiseInt(INT_MS_INSERT);

    cout << hex << "0x" << READ_REGISTER(REG_INT_STATUS) << " 0x" << READ_REGISTER(REG_INT_MASK)
         << dec << endl
         << flush;
}

void EmRegsSonyDSP::Unmount(EmHAL::Slot slot) { RaiseInt(INT_MS_EJECT); }

bool EmRegsSonyDSP::GetIrqLine() {
    return (READ_REGISTER(REG_INT_STATUS) & READ_REGISTER(REG_INT_MASK)) != 0;
}

uint32 EmRegsSonyDSP::StdRead(emuptr address, int size) {
    if (address - baseAddress < 0x00008000 && size == 2)
        LOG_READ_ACCESS(address, size, DoStdRead(address, size));

    return DoStdRead(address, size);
}

void EmRegsSonyDSP::StdWrite(emuptr address, int size, uint32 value) {
    if (address - baseAddress < 0x00008000) LOG_WRITE_ACCESS(address, size, value);

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

void EmRegsSonyDSP::IrqMaskWrite(emuptr address, int size, uint32 value) {
    StdWrite(address, size, value);

    const uint16 clearIntFlags = value & (INT_MS_EJECT | INT_MS_INSERT);
    if (clearIntFlags) ClearInt(clearIntFlags);
}

void EmRegsSonyDSP::IrqStatusWrite(emuptr address, int size, uint32 value) {
    LOG_WRITE_ACCESS(address, size, value);
    cerr << "unsupported write to int status register" << endl << flush;
}

void EmRegsSonyDSP::RaiseInt(uint16 flags) {
    const bool irqLineOld = GetIrqLine();

    WRITE_REGISTER(REG_INT_STATUS, READ_REGISTER(REG_INT_STATUS) | flags);

    const bool irqLine = GetIrqLine();
    if (irqLine != irqLineOld) irqChange.Dispatch(irqLine);
}

void EmRegsSonyDSP::ClearInt(uint16 flags) {
    const bool irqLineOld = GetIrqLine();

    WRITE_REGISTER(REG_INT_STATUS, READ_REGISTER(REG_INT_STATUS) & ~flags);

    const bool irqLine = GetIrqLine();
    if (irqLine != irqLineOld) irqChange.Dispatch(irqLine);
}

void EmRegsSonyDSP::IpcStatusWrite(emuptr address, int size, uint32 value) {
    StdWrite(address, size, value);

    ClearInt(INT_IPC_DONE);
}

void EmRegsSonyDSP::IpcCmdWrite(emuptr address, int size, uint32 value) {
    LOG_WRITE_ACCESS(address, size, value);

    IpcDispatch(value);
}

void EmRegsSonyDSP::IpcDispatch(uint16 cmd) {
    cerr << "DSP dispatch command 0x" << hex << cmd << dec << endl << flush;

    WRITE_REGISTER(REG_IPC_RESULT_1, 0);
    WRITE_REGISTER(REG_IPC_RESULT_2, 0);
    WRITE_REGISTER(REG_IPC_RESULT_3, 0);
    WRITE_REGISTER(REG_IPC_RESULT_4, 0);
    WRITE_REGISTER(REG_IPC_RESULT_5, 0);
    WRITE_REGISTER(REG_IPC_RESULT_6, 0);

    switch (cmd) {
        case 0x0036:
            cerr << "DSP upload, type 1" << endl << flush;
            WRITE_REGISTER(REG_IPC_STATUS, 0x36 << 10);
            break;

        case 0x0c85:
            cerr << "DSP upload, type 2" << endl << flush;
            WRITE_REGISTER(REG_IPC_STATUS, 0xfc << 10);
            break;

        case 0x0037:
            cerr << "DSP init" << endl << flush;
            WRITE_REGISTER(REG_IPC_STATUS, 0x37 << 10);
            break;

        default:
            cerr << "unknown DSP command" << endl << flush;
            WRITE_REGISTER(REG_IPC_STATUS, 0x00f);
            break;
    }

    RaiseInt(INT_IPC_DONE);
}
