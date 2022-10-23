#include "MemoryStick.h"

#include "MemoryStickStructs.h"

namespace {
    constexpr uint8 STATUS_COMMAND_OK = 0x80;
    constexpr uint8 STATUS_ERR = 0x40;
    constexpr uint8 STATUS_READY_FOR_TRANSFER = 0x20;
    constexpr uint8 STATUS_INVALID_COMMAND = 0x01;

    constexpr uint8 ACCESS_BLOCK = 0x00;
    constexpr uint8 ACCESS_PAGE = 0x20;
    constexpr uint8 ACCESS_OOB_ONLY = 0x40;
    constexpr uint8 ACCESS_OOB_ONLY_NO_EEC = 0x80;

    constexpr uint8 TPC_SET_REGS_WINDOW = 0x08;
    constexpr uint8 TPC_REGS_WRITE = 0x0b;
    constexpr uint8 TPC_REGS_READ = 0x04;
    constexpr uint8 TPC_GET_INT = 0x07;
    constexpr uint8 TPC_SET_CMD = 0x0e;
    constexpr uint8 TPC_READ_LONG_DATA = 0x02;
    constexpr uint8 TPC_WRITE_LONG_DATA = 0x0d;

    constexpr uint8 CMD_READ = 0xaa;
    constexpr uint8 CMD_STOP = 0x33;
    constexpr uint8 CMD_ERASE = 0x99;
    constexpr uint8 CMD_RESET = 0x3c;
    constexpr uint8 CMD_PROGRAM = 0x55;

    bool determineLayoutWithBlockSize(size_t pagesTotal, uint32 pagesPerBlock, uint8& segments) {
        if (pagesTotal % pagesPerBlock != 0) return false;
        size_t blocks = pagesTotal / pagesPerBlock;

        if ((blocks + 2) % 496 != 0) return false;
        segments = (blocks + 2) / 496;

        uint16 blocksTotal = segments * 512;

        switch (blocksTotal) {
            case 512:
            case 1024:
            case 2048:
            case 4096:
            case 8192:
                return true;

            default:
                return false;
        }
    }

    bool determineLayout(size_t pagesTotal, uint8& pagesPerBlock, uint8& segments) {
        // The following thresholds are not correct, but this is not necessary;
        // determineLayoutWithBlockSize will do the full calculation.

        // 16 pages per block are supported only for stick with 2 segments max
        if (pagesTotal < 1024 * 16) {
            pagesPerBlock = 16;
            if (determineLayoutWithBlockSize(pagesTotal, pagesPerBlock, segments)) return true;
        }

        // 32 pages per block are supported only for sticks with more than one segment
        if (pagesTotal > 512 * 16) {
            pagesPerBlock = 32;
            return determineLayoutWithBlockSize(pagesTotal, pagesPerBlock, segments);
        }

        return false;
    }

    uint16 swap16(uint16 x) { return (x << 8) | (x >> 8); }
}  // namespace

MemoryStick::MemoryStick() { blockMap = new uint16[16 * 512]; }

MemoryStick::~MemoryStick() { delete[] blockMap; }

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

        case TPC_GET_INT:
            bufferOutSize = 1;
            bufferOut = reinterpret_cast<uint8*>(&reg) + 1;

            return;

        case TPC_SET_CMD:
            if (dataInCount != 1) {
                SetFlags(STATUS_ERR);
                return;
            }

            DoCmd(dataIn[0]);

            return;

        case TPC_READ_LONG_DATA:
            if (currentOperation == Operation::readOne ||
                currentOperation == Operation::readMulti) {
                bufferOutSize = 512;
                bufferOut = preparedPage;

                ClearFlags();
            } else {
                cerr << "TPC READ_LONG_DATA without anything to read" << endl << flush;

                bufferOutSize = 0;
                SetFlags(STATUS_ERR);
            }

            return;

        case TPC_WRITE_LONG_DATA:
            if (dataInCount != 512) {
                cerr << "not enough data for TPC_WRITE_LONG_DATA: " << dataInCount << endl << flush;

                SetFlags(STATUS_ERR);
                return;
            }

            switch (currentOperation) {
                case Operation::programMulti:
                    ProgramPage(dataIn);

                    if (++reg.page < pagesPerBlock) {
                        SetFlags(STATUS_READY_FOR_TRANSFER);
                    } else {
                        SetFlags(STATUS_COMMAND_OK);
                        currentOperation = Operation::none;
                    }

                    return;

                case Operation::programOne:
                    ProgramPage(dataIn);
                    currentOperation = Operation::none;

                    SetFlags(STATUS_COMMAND_OK);

                    return;

                default:
                    cerr << "TPC_WRITE_LONG_DATA without pending write" << endl << flush;
                    SetFlags(STATUS_ERR);

                    return;
            }

        default:
            cerr << "unsupported TPC 0x" << hex << (int)tpcId << dec << endl << flush;

            SetFlags(STATUS_ERR);
    }
}

void MemoryStick::FinishReadTpc() {
    bool wasPageRead = bufferOut == preparedPage;

    bufferOutSize = 0;
    bufferOut = nullptr;

    if (!wasPageRead) return;

    switch (currentOperation) {
        case Operation::readMulti:
            reg.page++;
            if (PreparePage(preparedPage, false)) {
                SetFlags(STATUS_READY_FOR_TRANSFER);
            } else {
                currentOperation = Operation::none;
                SetFlags(STATUS_COMMAND_OK);
            }
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

    memset(blockMap, 0xffff, sizeof(uint16) * 512 * 16);

    uint16 logicalBlock = 0;
    for (uint8 segment = 0; segment < segments; segment++)
        for (uint16 block = segment == 0 ? 2 : 0; block < 496; block++)
            blockMap[(segment << 9) | block] = logicalBlock++;

    cerr << "mounted memory stick image, " << (int)segments << " segments, " << (int)pagesPerBlock
         << " pages per block" << endl
         << flush;
}

void MemoryStick::Unmount() { this->cardImage = nullptr; }

MemoryStick::Registers& MemoryStick::GetRegisters() { return reg; }

void MemoryStick::DoCmd(uint8 commandByte) {
    switch (commandByte) {
        case CMD_READ:
            DoCmdRead();
            return;

        case CMD_STOP:
            switch (currentOperation) {
                case Operation::readMulti:
                    currentOperation = Operation::readOne;
                    SetFlags(STATUS_COMMAND_OK);

                    return;

                case Operation::programMulti: {
                    currentOperation = Operation::programOne;
                    SetFlags(STATUS_COMMAND_OK);

                    return;
                }

                default:
                    cerr << "cmd_stop without running multi page operation" << endl << flush;
                    SetFlags(STATUS_ERR);

                    return;
            }

        case CMD_ERASE:
            SetFlags(EraseBlock() ? STATUS_COMMAND_OK : STATUS_ERR);
            return;

        case CMD_RESET:
            Reset();
            cerr << "memory stick reset" << endl << flush;
            return;

        case CMD_PROGRAM:
            switch (reg.accessType) {
                case ACCESS_BLOCK:
                    currentOperation = Operation::programMulti;
                    SetFlags(STATUS_READY_FOR_TRANSFER);

                    return;

                case ACCESS_PAGE:
                    currentOperation = Operation::programOne;
                    SetFlags(STATUS_COMMAND_OK);

                    return;

                case ACCESS_OOB_ONLY:
                case ACCESS_OOB_ONLY_NO_EEC:
                    SetFlags(STATUS_COMMAND_OK);

                    return;

                default:
                    cerr << "unhandled access type 0x" << hex << (int)reg.accessType << dec << endl
                         << flush;

                    return;
            }

        default:
            cerr << "invalid command 0x" << hex << (int)commandByte << dec << endl << flush;
            SetFlags(STATUS_INVALID_COMMAND);

            return;
    }
}

void MemoryStick::DoCmdRead() {
    currentOperation = Operation::none;

    switch (reg.accessType) {
        case ACCESS_BLOCK:
            if (!PreparePage(preparedPage, false)) {
                cerr << "invalid parameters for read" << endl << flush;
                SetFlags(STATUS_ERR);

                return;
            }

            currentOperation = Operation::readMulti;
            SetFlags(STATUS_READY_FOR_TRANSFER);

            return;

        case ACCESS_PAGE:
            if (!PreparePage(preparedPage, false)) {
                cerr << "invalid parameters for read" << endl << flush;
                SetFlags(STATUS_ERR);

                return;
            }

            currentOperation = Operation::readOne;
            SetFlags(STATUS_COMMAND_OK | STATUS_READY_FOR_TRANSFER);

            return;

        case ACCESS_OOB_ONLY:
            if (!PreparePage(preparedPage, true)) {
                cerr << "invalid parameters for read" << endl << flush;
                SetFlags(STATUS_ERR);

                return;
            }

            SetFlags(STATUS_COMMAND_OK);

            return;

        default:
            cerr << "unhandled access type 0x" << hex << (int)reg.accessType << dec << endl
                 << flush;

            return;
    }
}

bool MemoryStick::PreparePage(uint8* destination, bool oobOnly) {
    if (reg.page >= pagesPerBlock) return false;

    const uint32 blockIndex = reg.blockLo | (reg.blockMid << 8) | (reg.blockHi << 16);
    if (blockIndex >= segments * 512) return false;

    if (blockIndex == BOOT_BLOCK | blockIndex == BOOT_BLOCK_BACKUP) {
        PreparePageBootBlock(reg.page, destination, oobOnly);
    } else {
        uint16 logicalBlock = blockMap[blockIndex];

        if (logicalBlock == 0xffff) {
            memset(reg.oob, 0xff, 9);
            if (!oobOnly) memset(destination, 0xff, 512);
        } else {
            reg.oob[0] = 0xff;
            reg.oob[1] = 0x3c;
            reg.oob[2] = logicalBlock >> 8;
            reg.oob[3] = logicalBlock;

            if (!oobOnly) cardImage->Read(destination, logicalBlock * pagesPerBlock + reg.page, 1);
        }
    }

    return true;
}

uint32 MemoryStick::BlocksTotal() const { return segments * 512; }

uint8 MemoryStick::PagesPerBlock() const { return pagesPerBlock; }

void MemoryStick::PreparePageBootBlock(uint8 page, uint8* destination, bool oobOnly) {
    reg.oob[0] = 0xff;
    reg.oob[1] = 0x38;
    reg.oob[2] = reg.oob[3] = 0;

    if (oobOnly) return;

    memset(destination, 0, 512);

    if (page == 0) {
        MsBootBlock* boot = reinterpret_cast<MsBootBlock*>(destination);

        boot->blkid = swap16(0x01);
        boot->ftlVer = 0x0101;
        boot->numBbis = 0x01;
        boot->bbi[0].start = 0;
        boot->bbi[0].len = 0;
        boot->bbi[0].type = 0x01;
        boot->msClass = 0x01;
        boot->msSubclass = 0x02;
        boot->kbPerBlock = swap16(pagesPerBlock >> 1);
        boot->numBlocks = swap16(segments * 512);
        boot->usableBlocks = swap16(segments * 496);
        boot->pageSize = swap16(512);
        boot->oobSize = 16;
        boot->manufTimeYearHi = 20;
        boot->manufTimeYearLo = 22;
        boot->manufTimeMonth = 9;
        boot->manufDateDay = 26;
        boot->assemblyMakerCode = 0x01;
        boot->formatType = 0x01;
    }

    if (page == 1) {
        destination[0] = destination[1] = 0xff;
    }
}

bool MemoryStick::EraseBlock() {
    const uint32 blockIndex = reg.blockLo | (reg.blockMid << 8) | (reg.blockHi << 16);
    if (blockIndex >= segments * 512) return false;

    blockMap[blockIndex] = 0xffff;

    return true;
}

bool MemoryStick::ProgramPage(uint8* data) {
    const uint32 blockIndex = reg.blockLo | (reg.blockMid << 8) | (reg.blockHi << 16);
    if (blockIndex >= segments * 512) {
        cerr << "attempt to program invalid block 0x" << hex << blockIndex << dec << endl << flush;
        return false;
    }

    if (reg.page >= pagesPerBlock) {
        cerr << "attempt to program invalid page 0x" << hex << (int)reg.page << dec << endl
             << flush;
        return false;
    }

    const uint16 logicalBlock = (reg.oob[2] << 8) | reg.oob[3];
    const uint32 logicalPage = logicalBlock * pagesPerBlock + reg.page;

    if (logicalPage >= cardImage->BlocksTotal()) {
        cerr << "attempt to write beyond card bounds: block 0x" << hex << logicalPage << dec << endl
             << flush;

        return false;
    }

    cardImage->Write(data, logicalPage, 1);

    if (reg.page == 0) blockMap[blockIndex] = logicalBlock;

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

MemoryStick::Registers& MemoryStick::Registers::SetBlock(uint32 block) {
    blockHi = block >> 16;
    blockMid = block >> 8;
    blockLo = block;

    return *this;
}

MemoryStick::Registers& MemoryStick::Registers::SetPage(uint8 page) {
    this->page = page;

    return *this;
}

MemoryStick::Registers& MemoryStick::Registers::SetLogicalBlock(uint16 block) {
    oob[2] = block >> 8;
    oob[3] = block;

    return *this;
}
