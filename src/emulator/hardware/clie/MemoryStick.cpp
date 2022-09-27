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
    // constexpr uint8 ACCESS_OOB_ONLY_NO_EEC = 0x80;

    constexpr uint8 TPC_SET_REGS_WINDOW = 0x08;
    constexpr uint8 TPC_REGS_WRITE = 0x0b;
    constexpr uint8 TPC_REGS_READ = 0x04;
    constexpr uint8 TPC_GET_INT = 0x07;
    constexpr uint8 TPC_SET_CMD = 0x0e;
    constexpr uint8 TPC_READ_LONG_DATA = 0x02;
    constexpr uint8 TPC_WRITE_LONG_DATA = 0x0c;

    constexpr uint8 CMD_READ = 0xaa;
    constexpr uint8 CMD_STOP = 0x33;
    constexpr uint8 CMD_ERASE = 0x99;

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

                bufferOutSize = 0;
                SetFlags(STATUS_ERR);
            }

            return;

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
            if (PreparePage(false))
                SetFlags(STATUS_READY_FOR_TRANSFER);
            else
                currentOperation = Operation::none;

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

void MemoryStick::TpcSetCommand(uint8 commandByte) {
    switch (commandByte) {
        case CMD_READ:
            currentOperation = Operation::none;

            switch (reg.accessType) {
                case ACCESS_BLOCK:
                    if (!PreparePage(false)) {
                        cerr << "invalid parameters for read" << endl << flush;
                        SetFlags(STATUS_ERR);

                        return;
                    }

                    currentOperation = Operation::readMulti;
                    SetFlags(STATUS_READY_FOR_TRANSFER);

                    return;

                case ACCESS_PAGE:
                    if (!PreparePage(false)) {
                        cerr << "invalid parameters for read" << endl << flush;
                        SetFlags(STATUS_ERR);

                        return;
                    }

                    currentOperation = Operation::readOne;
                    SetFlags(STATUS_COMMAND_OK | STATUS_READY_FOR_TRANSFER);

                    return;

                case ACCESS_OOB_ONLY:
                    if (!PreparePage(true)) {
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

        case CMD_STOP:
            if (currentOperation == Operation::readMulti) {
                currentOperation = Operation::readOne;
                SetFlags(STATUS_COMMAND_OK);
            } else {
                cerr << "cmd_stop without running multi page operation" << endl << flush;
                SetFlags(STATUS_ERR);
            }
            break;

        case CMD_ERASE:
            SetFlags(STATUS_COMMAND_OK);
            return;

        default:
            cerr << "invalid command 0x" << hex << (int)commandByte << dec << endl << flush;
            SetFlags(STATUS_INVALID_COMMAND);
    }
}

bool MemoryStick::PreparePage(bool oobOnly) {
    if (reg.page >= pagesPerBlock) return false;

    const uint32 blockIndex = reg.blockLo | (reg.blockMid << 8) | (reg.blockHi << 16);
    if (blockIndex >= segments * 512) return false;

    if (blockIndex == 0 | blockIndex == 1) {
        PreparePageBootBlock(reg.page, oobOnly);
    } else {
        uint16 logicalBlock = blockMap[blockIndex];
        reg.oob[0] = 0xff;
        reg.oob[1] = 0x3c;

        if (logicalBlock == 0xffff) {
            reg.oob[2] = reg.oob[3] = 0xff;

            if (!oobOnly) memset(preparedPage, 0, 512);
        } else {
            cerr << hex << "block=0x" << blockIndex << " logicalBlock=0x" << logicalBlock
                 << " page=0x" << (int)reg.page << dec << endl
                 << flush;
            reg.oob[2] = logicalBlock >> 8;
            reg.oob[3] = logicalBlock;

            if (!oobOnly) cardImage->Read(preparedPage, logicalBlock * pagesPerBlock + reg.page, 1);
        }
    }

    return true;
}

void MemoryStick::PreparePageBootBlock(uint8 page, bool oobOnly) {
    reg.oob[0] = 0xff;
    reg.oob[1] = 0x38;
    reg.oob[2] = reg.oob[3] = 0;

    if (oobOnly) return;

    memset(preparedPage, 0, 512);

    if (page == 0) {
        MsBootBlock* boot = reinterpret_cast<MsBootBlock*>(preparedPage);

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
        preparedPage[0] = preparedPage[1] = 0xff;
    }
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
