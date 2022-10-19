#include "EmRegsSonyDSP.h"

#include <iomanip>
#include <sstream>

#include "EmCPU68K.h"
#include "EmMemory.h"
#include "UAE.h"

// #define LOG_ACCESS
#define LOGGING

#ifdef LOG_ACCESS
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

    constexpr emuptr REG_IPC_ARG_1 = 0x0c08;
    constexpr emuptr REG_IPC_ARG_2 = 0x0c0a;
    constexpr emuptr REG_IPC_ARG_3 = 0x0c0c;
    constexpr emuptr REG_IPC_ARG_4 = 0x0c0e;
    constexpr emuptr REG_IPC_ARG_5 = 0x0c10;
    constexpr emuptr REG_IPC_ARG_6 = 0x0c12;

    constexpr emuptr REG_IPC_RESULT_1 = 0x0c14;
    constexpr emuptr REG_IPC_RESULT_2 = 0x0c16;
    constexpr emuptr REG_IPC_RESULT_3 = 0x0c18;
    constexpr emuptr REG_IPC_RESULT_4 = 0x0c1a;
    constexpr emuptr REG_IPC_RESULT_5 = 0x0c1c;
    constexpr emuptr REG_IPC_RESULT_6 = 0x0c1e;

    constexpr emuptr REG_INT_STATUS = 0x0220;
    constexpr emuptr REG_INT_MASK = 0x0222;

    constexpr emuptr REG_RESET = 0x0204;

    constexpr uint16 INT_IPC_DONE = 0x0001;
    constexpr uint16 INT_MS_INSERT = 0x0004;
    constexpr uint16 INT_MS_EJECT = 0x0008;

    constexpr uint16 IPC_COMMAND_UPLOAD_TYPE_1 = 0x0036;
    constexpr uint16 IPC_COMMAND_UPLOAD_TYPE_2 = 0x0c85;
    constexpr uint16 IPC_COMMAND_DSP_INIT = 0x0037;
    constexpr uint16 IPC_COMMAND_ESCAPE = 0x0800;
    constexpr uint16 IPC_COMMAND_ESCAPE_ALT = 0xc800;

    constexpr uint16 IPC_COMMAND_MS_SENSE = 0x1e01;

    constexpr uint16 IPC_COMMAND_MS_READ_BOOT_BLOCK = 0x2201;
    constexpr uint16 IPC_COMMAND_MS_READ_BOOT_BLOCK_ALT = 0x6a01;
    constexpr uint16 IPC_COMMAND_MS_READ_OOB = 0x3a01;
    constexpr uint16 IPC_COMMAND_MS_READ_OOB_ALT = 0x1a01;
    constexpr uint16 IPC_COMMAND_MS_ERASE_BLOCK = 0x3201;
    constexpr uint16 IPC_COMMAND_MS_ERASE_BLOCK_ALT = 0x1201;
    constexpr uint16 IPC_COMMAND_MS_READ_BLOCK_OOB = 0x3601;
    constexpr uint16 IPC_COMMAND_MS_READ_BLOCK_OOB_ALT = 0x1601;
    constexpr uint16 IPC_COMMAND_MS_READ_BLOCK = 0x2601;
    constexpr uint16 IPC_COMMAND_MS_READ_BLOCK_ALT = 0x0601;
    constexpr uint16 IPC_COMMAND_MS_WRITE_BLOCK = 0x2a01;
    constexpr uint16 IPC_COMMAND_MS_WRITE_BLOCK_ALT = 0x0a01;

    constexpr uint16 IPC_STATUS_MASK = 0xfc00;

    constexpr uint16 SHM_SPACE_START = 0x8000;
    constexpr uint16 SHM_SPACE_SIZE = 0x8000;

    constexpr uint16 SHM_BOOT_BLOCK_BASE = 18 * 512;
    constexpr uint16 SHM_GET_BOOT_BLOCK_BLOB_SIZE = 0x2a00;

#ifdef LOG_ACCESS
    string identifyFrame(emuptr pc) {
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

    string trace(emuptr pc) {
        emuptr pcFrame0 = pc;
        emuptr pcFrame1 = EmMemGet32(m68k_areg(regs, 6) + 4);

        ostringstream ss;
        ss << identifyFrame(pcFrame1) << ":0x" << hex << pcFrame1 << " -> "
           << identifyFrame(pcFrame0) << ":0x" << pcFrame0 << dec;

        return ss.str();
    }

    void logWriteAccess(emuptr address, int size, uint32 value) {
        cerr << "DSP register write 0x" << hex << right << setfill('0') << setw(8) << address
             << " <- 0x" << setw(4) << value << dec << " (" << size << " bytes) from "
             << trace(regs.pc) << dec << endl
             << flush;
    }

    void logReadAccess(emuptr address, int size, uint32 value) {
        cerr << "DSP register read  0x" << hex << right << setfill('0') << setw(8) << address
             << " -> 0x" << setw(4) << value << dec << " (" << size << " bytes) from "
             << trace(regs.pc) << dec << endl
             << flush;
    }
#endif
}  // namespace

EmRegsSonyDSP::EmRegsSonyDSP(emuptr baseAddress) : baseAddress(baseAddress) {}

void EmRegsSonyDSP::Reset(Bool hardwareReset) {
    if (!hardwareReset) return;

    memset(regs, 0, sizeof(regs));
    memset(savedArguments, 0, sizeof(savedArguments));

    WRITE_REGISTER(REG_INT_STATUS, isMounted ? INT_MS_INSERT : INT_MS_EJECT);
    WRITE_REGISTER(REG_IPC_STATUS, IPC_STATUS_MASK);

    memoryStick.Reset();
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
    INSTALL_HANDLER(StdRead, ResetWrite, REG_RESET, 2);
}

bool EmRegsSonyDSP::SupportsImageInSlot(EmHAL::Slot slot, const CardImage& cardImage) {
    return slot == EmHAL::Slot::memorystick &&
           MemoryStick::IsSizeRepresentable(cardImage.BlocksTotal());
}

void EmRegsSonyDSP::Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) {
    if (slot != EmHAL::Slot::memorystick) return;
    memoryStick.Mount(&cardImage);

    isMounted = true;

    WRITE_REGISTER(REG_INT_STATUS, READ_REGISTER(REG_INT_STATUS) & ~INT_MS_EJECT);
    RaiseInt(INT_MS_INSERT);
}

void EmRegsSonyDSP::Unmount(EmHAL::Slot slot) {
    memoryStick.Unmount();

    isMounted = false;

    WRITE_REGISTER(REG_INT_STATUS, READ_REGISTER(REG_INT_STATUS) & ~INT_MS_INSERT);
    RaiseInt(INT_MS_EJECT);
}

bool EmRegsSonyDSP::GetIrqLine() {
    return (READ_REGISTER(REG_INT_STATUS) & READ_REGISTER(REG_INT_MASK)) != 0;
}

uint32 EmRegsSonyDSP::StdRead(emuptr address, int size) {
    if (address - baseAddress < SHM_SPACE_START && size == 2)
        LOG_READ_ACCESS(address, size, DoStdRead(address, size));

    return DoStdRead(address, size);
}

void EmRegsSonyDSP::StdWrite(emuptr address, int size, uint32 value) {
    if (address - baseAddress < SHM_SPACE_START) LOG_WRITE_ACCESS(address, size, value);

    DoStdWrite(address, size, value);
}

uint32 EmRegsSonyDSP::DoStdRead(emuptr address, int size) {
    if ((address + size - baseAddress) > sizeof(regs)) {
        gCPU68K->BusError(address, size, false);

        return 0;
    }

    uint8* realAddr = this->regs + (address - baseAddress);

    if (size == 1) return *realAddr;

    if (size == 2) return (*realAddr << 8) | *(realAddr + 1);

    return (*realAddr << 24) | (*(realAddr + 1) << 16) | (*(realAddr + 2) << 8) | *(realAddr + 3);
}

void EmRegsSonyDSP::DoStdWrite(emuptr address, int size, uint32 value) {
    if ((address + size - baseAddress) > sizeof(regs)) {
        gCPU68K->BusError(address, size, false);

        return;
    }

    uint8* realAddr = this->regs + (address - baseAddress);

    if (size == 1) {
        *realAddr = value;
    } else if (size == 2) {
        *(realAddr++) = value >> 8;
        *(realAddr++) = value;
    }

    else {
        *(realAddr++) = value >> 24;
        *(realAddr++) = value >> 16;
        *(realAddr++) = value >> 8;
        *(realAddr++) = value;
    }
}

void EmRegsSonyDSP::IrqMaskWrite(emuptr address, int size, uint32 value) {
    if (size != 2) {
        StdWrite(address, size, value);

        cerr << "0x" << hex << address << " : unsupported write size " << dec << size << endl
             << flush;

        return;
    }

    const bool irqLineOld = GetIrqLine();

    StdWrite(address, size, value);

    const bool irqLineNew = GetIrqLine();
    if (irqLineOld != irqLineNew) irqChange.Dispatch(irqLineNew);
}

void EmRegsSonyDSP::IrqStatusWrite(emuptr address, int size, uint32 value) {
    LOG_WRITE_ACCESS(address, size, value);

    cerr << "unsupported write to int status register" << endl << flush;
}

void EmRegsSonyDSP::ResetWrite(emuptr address, int size, uint32 value) {
    WRITE_REGISTER(REG_IPC_STATUS, IPC_STATUS_MASK);
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

    if (size != 2) {
        cerr << "0x" << hex << address << " : unsupported write size " << dec << size << endl
             << flush;

        return;
    }

    ClearInt(INT_IPC_DONE);
}

void EmRegsSonyDSP::IpcCmdWrite(emuptr address, int size, uint32 value) {
    LOG_WRITE_ACCESS(address, size, value);

    if (size != 2) {
        cerr << "0x" << hex << address << " : unsupported write size " << dec << size << endl
             << flush;
        return;
    }

    IpcDispatch(value);
}

void EmRegsSonyDSP::IpcDispatch(uint16 cmd) {
#ifdef LOGGING
    cerr << "DSP dispatch command 0x" << hex << cmd << dec << endl << flush;
#endif

    WRITE_REGISTER(REG_IPC_RESULT_1, 0);
    WRITE_REGISTER(REG_IPC_RESULT_2, 0);
    WRITE_REGISTER(REG_IPC_RESULT_3, 0);
    WRITE_REGISTER(REG_IPC_RESULT_4, 0);
    WRITE_REGISTER(REG_IPC_RESULT_5, 0);
    WRITE_REGISTER(REG_IPC_RESULT_6, 0);

    bool raiseInt = true;

    switch (cmd) {
        case IPC_COMMAND_UPLOAD_TYPE_1:
#ifdef LOGGING
            cerr << "DSP upload, type 1" << endl << flush;
#endif
            WRITE_REGISTER(REG_IPC_STATUS, (cmd << 10) & IPC_STATUS_MASK);
            break;

        case IPC_COMMAND_UPLOAD_TYPE_2:
#ifdef LOGGING
            cerr << "DSP upload, type 2" << endl << flush;
#endif
            WRITE_REGISTER(REG_IPC_STATUS, IPC_STATUS_MASK);
            break;

        case IPC_COMMAND_DSP_INIT:
#ifdef LOGGING
            cerr << "DSP init" << endl << flush;
#endif
            WRITE_REGISTER(REG_IPC_STATUS, (cmd << 10) & IPC_STATUS_MASK);
            break;

        case IPC_COMMAND_MS_SENSE:
#ifdef LOGGING
            cerr << "DSP memory stick sense" << endl << flush;
#endif
            WRITE_REGISTER(REG_IPC_STATUS, cmd & IPC_STATUS_MASK);
            break;

        case IPC_COMMAND_MS_READ_BOOT_BLOCK:
        case IPC_COMMAND_MS_READ_BOOT_BLOCK_ALT:
            DoCmdReadBootBlock(cmd);
            break;

        case IPC_COMMAND_MS_READ_OOB:
        case IPC_COMMAND_MS_READ_OOB_ALT:
            DoCmdReadOob(cmd);
            break;

        case IPC_COMMAND_MS_ERASE_BLOCK:
        case IPC_COMMAND_MS_ERASE_BLOCK_ALT: {
            uint16 block = READ_REGISTER(REG_IPC_ARG_1);
#ifdef LOGGING
            cerr << "erase block 0x" << hex << block << dec << endl << flush;
#endif

            memoryStick.GetRegisters().SetBlock(block);
            if (memoryStick.EraseBlock())
                WRITE_REGISTER(REG_IPC_STATUS, cmd & IPC_STATUS_MASK);
            else
                WRITE_REGISTER(REG_IPC_STATUS, (cmd & IPC_STATUS_MASK) | 0x0001);

            break;
        }

        case IPC_COMMAND_MS_READ_BLOCK_OOB:
        case IPC_COMMAND_MS_READ_BLOCK_OOB_ALT:
            DoCmdReadBlockOob(cmd);
            break;

        case IPC_COMMAND_MS_READ_BLOCK:
        case IPC_COMMAND_MS_READ_BLOCK_ALT:
            DoCmdReadBlock(cmd);
            break;

        case IPC_COMMAND_ESCAPE:
        case IPC_COMMAND_ESCAPE_ALT:
#ifdef LOGGING
            cerr << "IPC escape" << endl << flush;
#endif

            savedArguments[0] = READ_REGISTER(REG_IPC_ARG_1);
            savedArguments[1] = READ_REGISTER(REG_IPC_ARG_2);
            savedArguments[2] = READ_REGISTER(REG_IPC_ARG_3);
            savedArguments[3] = READ_REGISTER(REG_IPC_ARG_4);
            savedArguments[4] = READ_REGISTER(REG_IPC_ARG_5);
            savedArguments[5] = READ_REGISTER(REG_IPC_ARG_6);

            raiseInt = false;

            break;

        case IPC_COMMAND_MS_WRITE_BLOCK:
        case IPC_COMMAND_MS_WRITE_BLOCK_ALT:
            DoCmdWriteBlock(cmd);
            break;

        default:
            cerr << "unknown DSP command" << endl << flush;
            WRITE_REGISTER(REG_IPC_STATUS, (cmd & IPC_STATUS_MASK) | 0x000f);
            break;
    }

    if (raiseInt) RaiseInt(INT_IPC_DONE);
}

void EmRegsSonyDSP::DoCmdReadBootBlock(uint16 cmd) {
    WRITE_REGISTER(REG_IPC_STATUS, (cmd & IPC_STATUS_MASK) | 0x0001);

#ifdef LOGGING
    cerr << "read boot block" << endl << flush;
#endif

    uint16 shmBase = READ_REGISTER(REG_IPC_ARG_1) * 2;
    if (shmBase < SHM_SPACE_START) return;

    shmBase -= SHM_SPACE_START;
    if (shmBase + SHM_GET_BOOT_BLOCK_BLOB_SIZE > SHM_SPACE_SIZE) return;

    uint8* base = regs + SHM_SPACE_START + shmBase;
    memset(base, 0, SHM_GET_BOOT_BLOCK_BLOB_SIZE);

    MemoryStick::Registers& registers(memoryStick.GetRegisters());

    registers.SetBlock(MemoryStick::BOOT_BLOCK).SetPage(0);
    if (!memoryStick.PreparePage(base + SHM_BOOT_BLOCK_BASE, false)) return;

    registers.SetPage(1);
    if (!memoryStick.PreparePage(base + SHM_BOOT_BLOCK_BASE + 512, false)) return;

    if (cmd == IPC_COMMAND_MS_READ_BOOT_BLOCK_ALT) {
        WRITE_REGISTER(REG_IPC_RESULT_2, 0x4000);
    } else {
        WRITE_REGISTER(REG_IPC_RESULT_1, (registers.oob[0] << 8) | registers.oob[1]);
        WRITE_REGISTER(REG_IPC_RESULT_2, MemoryStick::BOOT_BLOCK_BACKUP);
    }

    WRITE_REGISTER(REG_IPC_STATUS, cmd & IPC_STATUS_MASK);
}

void EmRegsSonyDSP::DoCmdReadOob(uint16 cmd) {
    WRITE_REGISTER(REG_IPC_STATUS, (cmd & IPC_STATUS_MASK) | 0x0001);

#ifdef LOGGING
    cerr << "read OOB" << endl << flush;
#endif

    uint16 shmBase = READ_REGISTER(REG_IPC_ARG_3) * 2;
    if (shmBase < SHM_SPACE_START) return;

    shmBase -= SHM_SPACE_START;

    const uint32 blocksTotal = memoryStick.BlocksTotal();
    if (shmBase + 4 * blocksTotal > SHM_SPACE_SIZE) return;

    MemoryStick::Registers& registers(memoryStick.GetRegisters());
    registers.SetPage(0);

    uint8* base = regs + SHM_SPACE_START + shmBase;
    for (uint32 block = 0; block < blocksTotal; block++) {
        registers.SetBlock(block);
        memoryStick.PreparePage(nullptr, true);

        *(base++) = registers.oob[0];
        *(base++) = registers.oob[1];
        *(base++) = registers.oob[2];
        *(base++) = (registers.oob[3] == 0xff && registers.oob[2] == 0xff &&
                     cmd == IPC_COMMAND_MS_READ_BOOT_BLOCK)
                        ? 0xfe
                        : registers.oob[3];
    }

    WRITE_REGISTER(REG_IPC_STATUS, cmd & IPC_STATUS_MASK);
}

void EmRegsSonyDSP::DoCmdReadBlock(uint16 cmd) {
    WRITE_REGISTER(REG_IPC_STATUS, (cmd & IPC_STATUS_MASK) | 0x0001);

    uint16 block = READ_REGISTER(REG_IPC_ARG_1);
    uint16 shmBase = READ_REGISTER(REG_IPC_ARG_2) * 2;
    uint16 firstPage = READ_REGISTER(REG_IPC_ARG_3);
    uint16 lastPage = READ_REGISTER(REG_IPC_ARG_4);

#ifdef LOGGING
    cerr << hex << "block read block 0x" << block << " , first page 0x" << firstPage
         << " , last page 0x" << lastPage << " , pages per block 0x"
         << (int)memoryStick.PagesPerBlock() << dec << endl
         << flush;
#endif

    if (lastPage < firstPage || lastPage >= memoryStick.PagesPerBlock()) return;

    if (shmBase < SHM_SPACE_START) return;

    shmBase -= SHM_SPACE_START;
    if (shmBase + 512 * (lastPage - firstPage + 1) > SHM_SPACE_SIZE) return;

    MemoryStick::Registers& registers(memoryStick.GetRegisters());
    registers.SetBlock(block);

    uint8* base = regs + SHM_SPACE_START + shmBase;
    for (uint8 page = firstPage; page <= lastPage; page++) {
        registers.SetPage(page);
        memoryStick.PreparePage(base, false);

        base += 512;
    }

    WRITE_REGISTER(REG_IPC_STATUS, cmd & IPC_STATUS_MASK);
}

void EmRegsSonyDSP::DoCmdWriteBlock(uint16 cmd) {
    WRITE_REGISTER(REG_IPC_STATUS, (cmd & IPC_STATUS_MASK) | 0x0001);

    uint16 block;
    uint16 shmBase;
    uint16 firstPage;
    uint16 lastPage;
    uint16 oldBlock;
    uint16 logicalBlock;

    if (cmd == IPC_COMMAND_MS_WRITE_BLOCK) {
        block = READ_REGISTER(REG_IPC_ARG_1);
        shmBase = READ_REGISTER(REG_IPC_ARG_2) * 2;
        firstPage = READ_REGISTER(REG_IPC_ARG_3);
        lastPage = READ_REGISTER(REG_IPC_ARG_4);
        oldBlock = savedArguments[0];
        logicalBlock = savedArguments[2];
    } else {
        block = READ_REGISTER(REG_IPC_ARG_1);
        shmBase = READ_REGISTER(REG_IPC_ARG_4) * 2;
        firstPage = READ_REGISTER(REG_IPC_ARG_5);
        lastPage = READ_REGISTER(REG_IPC_ARG_6);
        oldBlock = READ_REGISTER(REG_IPC_ARG_2);
        logicalBlock = READ_REGISTER(REG_IPC_ARG_3);
    }

#ifdef LOGGING
    cerr << hex << "block write old block 0x" << oldBlock << " , new block 0x" << block
         << " , first page 0x" << firstPage << " , last page 0x" << lastPage
         << " , logical block 0x" << logicalBlock << " , shm base 0x" << shmBase << dec << endl
         << flush;
#endif

    if (oldBlock >= memoryStick.BlocksTotal() && oldBlock != 0xffff) {
        cerr << "old block out of bounds" << endl << flush;
        return;
    }

    if (block >= memoryStick.BlocksTotal()) {
        cerr << "new block out of bounds" << endl << flush;
        return;
    }

    if (firstPage < 0 || firstPage > lastPage) {
        cerr << "fist page out of bounds" << endl << flush;
        return;
    }

    if (lastPage >= memoryStick.PagesPerBlock()) {
        cerr << "last page out of bounds" << endl << flush;
        return;
    }

    if (shmBase < SHM_SPACE_START) return;
    shmBase -= SHM_SPACE_START;

    if (shmBase + 512 * memoryStick.PagesPerBlock() > SHM_SPACE_SIZE) return;

    MemoryStick::Registers& registers(memoryStick.GetRegisters());
    uint8* base = regs + SHM_SPACE_START + shmBase + firstPage * 512;

    registers.SetBlock(block).SetLogicalBlock(logicalBlock);

    for (uint8 page = firstPage; page <= lastPage; page++) {
        registers.SetPage(page);
        if (!memoryStick.ProgramPage(base)) return;

        base += 512;
    }

    WRITE_REGISTER(REG_IPC_STATUS, cmd & IPC_STATUS_MASK);
}

void EmRegsSonyDSP::DoCmdReadBlockOob(uint16 cmd) {
    WRITE_REGISTER(REG_IPC_STATUS, (cmd & IPC_STATUS_MASK) | 0x0001);

    uint16 block = READ_REGISTER(REG_IPC_ARG_1);
#ifdef LOGGING
    cerr << "read block oob block 0x" << hex << block << endl << flush;
#endif

    MemoryStick::Registers registers(memoryStick.GetRegisters());

    registers.SetBlock(block).SetPage(0);
    if (!memoryStick.PreparePage(nullptr, true)) return;

    WRITE_REGISTER(REG_IPC_RESULT_1, (registers.oob[0] << 8) | registers.oob[1]);
    WRITE_REGISTER(REG_IPC_STATUS, cmd & IPC_STATUS_MASK);
}
