#include "MemoryStick.h"

namespace {
    constexpr uint8 STATUS_COMMAND_OK = 0x80;
    constexpr uint8 STATUS_ERR = 0x40;
    constexpr uint8 STATUS_READY_FOR_TRANSFER = 0x20;
    constexpr uint8 STATUS_INVALID_COMMAND = 0x01;

    constexpr uint8 ACCESS_BLOCK = 0x00;
    constexpr uint8 ACCESS_PAGE = 0x20;
    // constexpr uint8 ACCESS_OOB_ONLY = 0x40;
    // constexpr uint8 ACCESS_OOB_ONLY_NO_EEC = 0x80;

    constexpr uint8 TPC_SET_REGS_WINDOW = 0x08;
    constexpr uint8 TPC_REGS_WRITE = 0x0b;
    constexpr uint8 TPC_REGS_READ = 0x04;
    constexpr uint8 TPC_SET_CMD = 0x0e;
    constexpr uint8 TPC_READ_LONG_DATA = 0x02;
    constexpr uint8 TPC_WRITE_LONG_DATA = 0x0c;

    constexpr uint8 CMD_READ = 0xaa;
    constexpr uint8 CMD_STOP = 0x33;

    bool determineLayoutWithBlockSize(size_t pagesTotal, uint32 pagesPerBlock, uint8& segments) {
        if (pagesTotal % pagesPerBlock != 0) return false;
        size_t blocks = pagesTotal / pagesPerBlock;

        if ((blocks + 2) % 496 != 0) return false;
        segments = (blocks + 2) / 496;

        return segments <= 16;
    }

    bool determineLayout(size_t pagesTotal, uint8& pagesPerBlock, uint8& segments) {
        pagesPerBlock = 16;
        if (determineLayoutWithBlockSize(pagesTotal, pagesPerBlock, segments)) return true;

        pagesPerBlock = 32;
        return determineLayoutWithBlockSize(pagesTotal, pagesPerBlock, segments);
    }
}  // namespace

void MemoryStick::Reset() {
    memset(&reg, 0, sizeof(reg));

    reg.msType = 0xff;
    reg.msCategory = 0xff;
    reg.msClass = 0xff;

    writeWindowStart = 0x10;
    writeWindowSize = 15;

    readWindowStart = 0x00;
    readWindowSize = 0x1e;

    bufferOutSize = 0;
    bufferOut = nullptr;

    currentOperation = Operation::none;
}

void MemoryStick::ExecuteTpc(uint8 tpcId, uint32 dataInCount, uint8* dataIn) {
    bufferOutSize = 0;
    if (!cardImage) return;

    switch (tpcId) {
        case TPC_REGS_WRITE: {
            if (dataInCount != writeWindowSize || writeWindowStart < offsetof(Registers, cfg) ||
                writeWindowSize + writeWindowStart > sizeof(Registers)) {
                SetFlags(STATUS_ERR);
                return;
            }

            uint8* registerFile = reinterpret_cast<uint8*>(&reg);
            memcpy(registerFile + writeWindowStart, dataIn, writeWindowSize);

            ClearFlags();
            return;
        }

        case TPC_REGS_READ: {
            if (readWindowStart + readWindowSize >= sizeof(Registers)) {
                SetFlags(STATUS_ERR);
                return;
            }

            bufferOutSize = readWindowSize;
            bufferOut = reinterpret_cast<uint8*>(&reg) + readWindowStart;

            return;
        }

        case TPC_SET_CMD:
            if (dataInCount != 1) {
                SetFlags(STATUS_ERR);
                return;
            }

            TpcSetCommand(dataIn[0]);

            return;

        case TPC_READ_LONG_DATA:
            if (currentOperation == Operation::readOne ||
                currentOperation == Operation::readMulti) {
                bufferOutSize = 512;
                bufferOut = preparedPage;

                ClearFlags();
            } else {
                cerr << "TPC READ_LONG_DATA without anything to read" << endl << flush;
                SetFlags(STATUS_ERR);
            }

            return;

        default:
            cerr << "unsupported TPC 0x" << hex << (int)tpcId << dec << endl << flush;

            SetFlags(STATUS_ERR);
    }
}

void MemoryStick::FinishReadTpc() {
    bufferOutSize = 0;
    bufferOut = nullptr;

    switch (currentOperation) {
        case Operation::readMulti:
            if (!PreparePage()) currentOperation = Operation::none;

            break;

        case Operation::readOne:
            currentOperation = Operation::none;
            SetFlags(STATUS_COMMAND_OK);
            break;

        case Operation::none:
            break;

        default:
            EmAssert(false);
    }
}

uint8* MemoryStick::GetDataOut() { return bufferOut; }

uint32 MemoryStick::GetDataOutSize() { return bufferOutSize; }

void MemoryStick::Mount(CardImage* cardImage) {
    EmAssert(determineLayout(cardImage->BlocksTotal(), pagesPerBlock, segments));

    this->cardImage = cardImage;

    cerr << "mounted memory stick image, " << (int)segments << " segments, " << (int)pagesPerBlock
         << " pages per block" << endl
         << flush;
}

void MemoryStick::Unmount() { this->cardImage = nullptr; }

void MemoryStick::TpcSetCommand(uint8 commandByte) {
    switch (commandByte) {
        case CMD_READ:
            if (PreparePage()) {
                switch (reg.accessType) {
                    case ACCESS_BLOCK:
                        currentOperation = Operation::readMulti;
                        break;

                    case ACCESS_PAGE:
                        currentOperation = Operation::readOne;
                        break;

                    default:
                        cerr << "unhandled access type 0x" << hex << reg.accessType << dec << endl
                             << flush;
                        currentOperation = Operation::none;
                        break;
                }
            } else {
                cerr << "invalid parameters for read" << endl << flush;
                SetFlags(STATUS_ERR);
            }

            return;

        case CMD_STOP:
            if (currentOperation == Operation::readMulti) {
                currentOperation = Operation::readOne;
                SetFlags(STATUS_COMMAND_OK);
            } else {
                cerr << "cmd_stop without running multi page operation" << endl << flush;
                SetFlags(STATUS_ERR);
            }
            break;

        default:
            cerr << "invalid command 0x" << hex << (int)commandByte << dec << endl << flush;
            SetFlags(STATUS_INVALID_COMMAND);
    }
}

bool MemoryStick::PreparePage() {
    if (reg.page >= pagesPerBlock) return false;

    const uint32 blockIndex = reg.blockLo | (reg.blockMid << 8) | (reg.blockHi << 16);
    if (blockIndex >= segments * 512) return false;

    memset(preparedPage, 0, 512);

    SetFlags(STATUS_READY_FOR_TRANSFER);

    return true;
}

void MemoryStick::SetFlags(uint8 flags) {
    reg.intFlags = flags;
    if (flags) irq.Dispatch();
}

void MemoryStick::ClearFlags() { reg.intFlags = 0; }

MemoryStick::TpcType MemoryStick::GetTpcType(uint8 tpcId) {
    switch (tpcId) {
        case TPC_SET_REGS_WINDOW:
        case TPC_REGS_WRITE:
        case TPC_SET_CMD:
        case TPC_WRITE_LONG_DATA:
            return TpcType::write;

        case TPC_REGS_READ:
        case TPC_READ_LONG_DATA:
        default:
            return TpcType::read;
    }
}

bool MemoryStick::IsSizeRepresentable(size_t pagesTotal) {
    uint8 pagesPerBlock, segments;

    return determineLayout(pagesTotal, pagesPerBlock, segments);
}
