#include "EmSPISlaveSD.h"

#include "ChunkHelper.h"
#include "EmCRC.h"
#include "ExternalStorage.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

// #define DUMP_COMMAND_STREAM

namespace {
    constexpr uint32 SAVESTATE_VERSION = 1;

    constexpr uint8 ERR_ILLEGAL_COMMAND = 0x04;
    constexpr uint8 ERR_CARD_IDLE = 0x01;
    constexpr uint8 ERR_PARAMETER = 0x40;

    constexpr uint8 DATA_TOKEN_DEFAULT = 0xfe;
    constexpr uint8 DATA_TOKEN_CMD25 = 0xfc;
    constexpr uint8 DATA_TOKEN_STOP = 0xfd;

    constexpr uint8 DATA_RESPONSE_ACCEPTED = 0xe5;
    constexpr uint8 DATA_RESPONSE_WRITE_FAILED = 0x0d;

    CardImage* image() { return gExternalStorage.GetImageInSlot(EmHAL::Slot::sdcard); }

    bool determineLayout(size_t blocksTotal, uint32& cSizeMult, uint32& cSize) {
        for (cSizeMult = 0; cSizeMult < 8; cSizeMult++) {
            if ((blocksTotal >> (cSizeMult + 2)) < 0x1000) break;
        }

        if (cSizeMult > 7) return false;

        cSize = (blocksTotal >> (cSizeMult + 2)) - 1;

        return (1 << (cSizeMult + 2)) * (cSize + 1) == blocksTotal;
    }
}  // namespace

void EmSPISlaveSD::Reset() {
    spiState = SpiState::notSelected;
    cardState = CardState::idle;
    lastCmd = 0;
    acmd = false;
    cmd12Countdown = 0;
}

void EmSPISlaveSD::Save(Savestate& savestate) { DoSave(savestate); }

void EmSPISlaveSD::Save(SavestateProbe& savestateProbe) { DoSave(savestateProbe); }

void EmSPISlaveSD::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::spiSlaveSD);
    if (!chunk) return;

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore SPISlaveSD: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

uint16 EmSPISlaveSD::DoExchange(uint16 control, uint16 data) {
    uint8 bits = (control & 0x0f) + 1;

    switch (bits) {
        case 16: {
            uint16 dataOut = DoExchange8(data >> 8) << 8;
            dataOut |= DoExchange8(data);

            return dataOut;
        }

        case 8:
            return DoExchange8(data);

        default:
            // cerr << "unsupported SPI shift width " << (int)bits << endl << flush;

            return 0xff;
    }
}

void EmSPISlaveSD::Enable(void) {
    if (spiState != SpiState::notSelected) return;

    if (gExternalStorage.IsMounted(EmHAL::Slot::sdcard)) {
        spiState = SpiState::rxCmdByte;
        lastCmd = 0;
    }
}

void EmSPISlaveSD::Disable(void) { spiState = SpiState::notSelected; }

template <typename T>
void EmSPISlaveSD::DoSave(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::spiSlaveSD);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

template <typename T>
void EmSPISlaveSD::DoSaveLoad(T& helper) {
    uint8 spiStateByte = static_cast<uint8>(spiState);
    uint8 cardStateByte = static_cast<uint8>(cardState);

    helper.Do(typename T::Pack8() << spiStateByte << cardStateByte << lastCmd << cmd12Countdown)
        .Do32(bufferSize)
        .Do32(bufferIndex)
        .Do32(currentBlock)
        .DoBool(acmd)
        .DoBuffer(buffer, sizeof(buffer));

    spiState = static_cast<SpiState>(spiStateByte);
    cardState = static_cast<CardState>(cardStateByte);
}

uint8 EmSPISlaveSD::DoExchange8(uint8 data) {
    if (!gExternalStorage.IsMounted(EmHAL::Slot::sdcard)) return 0x00;
    if (cardState == CardState::multiblockRead) HandleCmd12(data);

    switch (spiState) {
        case SpiState::notSelected:
            return 0xff;

        case SpiState::rxCmdByte:
            if ((data & 0xc0) == 0x40) {
                lastCmd = data & 0x3f;
                spiState = SpiState::rxArg;
                BufferStart(5);
            }

            return 0xff;

        case SpiState::rxArg:
            buffer[bufferIndex++] = data;

            if (bufferIndex == bufferSize) {
                DoCmd();
                spiState = SpiState::txData;
            }

            return 0xff;

        case SpiState::txData: {
            uint8 dataOut = buffer[bufferIndex++];
            if (bufferIndex == bufferSize) {
                switch (cardState) {
                    case CardState::writeTransaction:
                    case CardState::multiblockWrite:
                        spiState = SpiState::rxBlockWait;
                        break;

                    case CardState::multiblockRead:
                        spiState = SpiState::txBlockWait;
                        break;

                    default:
                        spiState = SpiState::rxCmdByte;
                }
            }

            return dataOut;
        }

        case SpiState::rxBlockWait:
            if (data == DATA_TOKEN_DEFAULT ||
                (data == DATA_TOKEN_CMD25 && cardState == CardState::multiblockWrite)) {
                spiState = SpiState::rxBlock;
                BufferStart(514);
            };

            if ((data == DATA_TOKEN_STOP && cardState == CardState::multiblockWrite)) {
                cardState = CardState::initialized;
                spiState = SpiState::rxCmdByte;
            }

            return 0xff;

        case SpiState::rxBlock:
            buffer[bufferIndex++] = data;
            if (bufferIndex == bufferSize) {
                if (cardState == CardState::multiblockWrite)
                    FinishWriteMultiblock();
                else
                    FinishWriteSingleBlock();
            }

            return 0xff;

        case SpiState::txBlockWait:
            spiState = SpiState::txData;
            DoReadMulitblock();

            return 0xff;
    }
}

void EmSPISlaveSD::BufferStart(uint32 count) {
    bufferSize = count;
    bufferIndex = 0;
}

void EmSPISlaveSD::BufferAddBlock(uint8 token, uint8* data, size_t size) {
    BufferAdd(token);

    memcpy(buffer + bufferSize, data, size);
    bufferSize += size;

    uint16 crc16 = crc::sdCRC16(data, size);
    BufferAdd(crc16 >> 8, crc16);
}

void EmSPISlaveSD::DoCmd() {
    if (acmd) return DoAcmd();
    acmd = false;

#ifdef DUMP_COMMAND_STREAM
    cerr << "received CMD" << (int)lastCmd << endl << flush;
#endif

    switch (lastCmd) {
        // Reset
        case 0:
            PrepareR1(ERR_CARD_IDLE);

            return;

        // Initialize (MMC)
        case 1:
            PrepareR1(0x00);
            cardState = CardState::initialized;

            return;

        // SDIO stuff. We do not support this.
        case 5:
            PrepareR1(ERR_ILLEGAL_COMMAND);

            return;

        // Read CSD
        case 9:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                DoReadCSD();
            }

            return;

        // Read CID
        case 10:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                DoReadCID();
            }

            return;

        // Read status register
        case 13:
            BufferStart(0);
            if (cardState == CardState::idle) {
                BufferAdd(0xff, ERR_CARD_IDLE, 0);
            } else {
                BufferAdd(0xff, 0, 0);
            }

            return;

        // Set block length. We only support 512 bytes.
        case 16:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else if (Param() != 512) {
                PrepareR1(ERR_PARAMETER);
            } else {
                PrepareR1(0x00);
            }

            return;

        // Single block read
        case 17:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                DoReadSingleBlock();
            }

            return;

        // Multiblock read
        case 18:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else if (Param() >> 9 >= image()->BlocksTotal()) {
                PrepareR1(ERR_PARAMETER);
            } else {
                cardState = CardState::multiblockRead;
                cmd12Countdown = 0;
                currentBlock = Param() >> 9;
                PrepareR1(0);
            }

            return;

        // Single block write
        case 24:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else if (Param() >> 9 >= image()->BlocksTotal()) {
                PrepareR1(ERR_PARAMETER);
            } else {
                currentBlock = Param() >> 9;

                PrepareR1(0x00);
                cardState = CardState::writeTransaction;
            }

            return;

        // Multiblock write
        case 25:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else if (Param() >> 9 >= image()->BlocksTotal()) {
                PrepareR1(ERR_PARAMETER);
            } else {
                cardState = CardState::multiblockWrite;
                currentBlock = Param() >> 9;
                PrepareR1(0);
            }

            return;

        // Escape to ACMD
        case 55:
            PrepareR1(0x00);
            acmd = true;

            return;

        // Read OCR
        case 58: {
            BufferStart(0);
            BufferAdd(0xff, 0x00, 0x00, 0xff, 0x80, 0x00);

            return;
        }

        default:
            cerr << "unsupported SD CMD" << (int)lastCmd << endl << flush;
            PrepareR1(ERR_ILLEGAL_COMMAND);

            return;
    }
}

void EmSPISlaveSD::DoAcmd() {
#ifdef DUMP_COMMAND_STREAM
    cerr << "received ACMD" << (int)lastCmd << endl << flush;
#endif

    acmd = false;

    switch (lastCmd) {
        // Set preerease block count. The spec flags the content of blocks
        // that have been marked for erasure but not subsequently written as
        // undefined, so we just ignore this command.
        case 23:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                PrepareR1(0x00);
            }

            return;

        // Init (SD style)
        case 41:
            PrepareR1(0x00);
            cardState = CardState::initialized;

            return;

        // Read SCR
        case 51:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                DoReadSCR();
            }

            return;

        default:
            cerr << "unsupported SD ACMD" << (int)lastCmd << endl << flush;
            PrepareR1(ERR_ILLEGAL_COMMAND);

            return;
    }
}

void EmSPISlaveSD::DoReadSingleBlock() {
    uint8 block[512];

    BufferStart(0);
    if (image()->Read(block, Param() >> 9, 1) != 1) {
        PrepareR1(ERR_PARAMETER);
        return;
    }

    PrepareR1(0x00);
    BufferAdd(0xff);

    BufferAddBlock(DATA_TOKEN_DEFAULT, block, 512);
}

void EmSPISlaveSD::DoReadMulitblock() {
    uint8 block[512];

    BufferStart(0);
    image()->Read(block, (currentBlock++) % image()->BlocksTotal(), 1);

    BufferAddBlock(DATA_TOKEN_DEFAULT, block, 512);
}

void EmSPISlaveSD::DoReadCSD() {
    // Block size is hardcoded to 512 bytes
    const uint32 blLen = 0x09, sectorSize = 0;
    uint32 cSizeMult, cSize;

    if (!determineLayout(image()->BlocksTotal(), cSizeMult, cSize)) {
        PrepareR1(ERR_PARAMETER);
        return;
    }

    uint8 csd[15];

    csd[0] = 0x00;
    csd[1] = 0x0e;                  // TAAC
    csd[2] = 0x00;                  // NSAC
    csd[3] = 0x5A;                  // TRAN_SPEED
    csd[4] = 0x5B;                  // CCC[4..11]
    csd[5] = blLen | 0x50;          // CCC[0..3], READ_BL_LEN
    csd[6] = (cSize >> 10) | 0x80;  // READ_BL_PARTIAL(1), WRITE_BLK_MISALIGN, READ_BLK_MISALIGN,
                                    // DSR_IMP, RESERVED, C_SIZE[10..11]
    csd[7] = cSize >> 2;            // C_SIZE[2..9]
    csd[8] = (cSize << 6) | 0x2D;   // C_SIZE[0..1], VDD_R_CURR_MIN (35mA), VDD_R_CURR_MAX(45mA)
    csd[9] =
        (cSizeMult >> 1) | 0xD8;  // VDD_W_CURR_MIN (60mA), VDD_W_CURR_MAX(80mA), C_SIZE_MULT[1..2]
    csd[10] = (sectorSize >> 1) |
              ((cSizeMult << 7) & 0xff);  // C_SIZE_MULT[0], ERASE_BLK_EN, SECTOR_SIZE[1..7]
    csd[11] = (sectorSize << 7) & 0xff;   // SECTOR_SIZE[0], WP_GRP_SIZE
    csd[12] = (blLen >> 2) | 8;     // WP_GRP_ENABLE, RESERVED, R2W_FACTOR(2), WRITE_BL_LEN[2..3]
    csd[13] = (blLen << 6) & 0xff;  // WRITE_BL_LEN[0..1], WRITE_BL_PARTIAL, RESERVED
    csd[14] = 0x00;  // FILE_FORMAT_GRP, COPY, PERM_WRITE_PROTECT, TMP_WRITE_PROTECT, FILE_FORMAT,
                     // RESERVED

    PrepareR1(0x00);
    BufferAdd(0xff);
    BufferAddBlock(DATA_TOKEN_DEFAULT, csd, sizeof(csd));
}

void EmSPISlaveSD::DoReadCID() {
    uint8 cid[16];

    cid[0] = 'D';  // MID
    cid[1] = 't';  // OID
    cid[2] = 'y';
    cid[3] = 'C';  // PNM
    cid[4] = 'l';
    cid[5] = 'p';
    cid[6] = 'l';
    cid[7] = 't';
    cid[8] = 32;    // PRV
    cid[9] = 0x00;  // PSN
    cid[10] = 0x00;
    cid[11] = 0x4A;
    cid[12] = 0xEC;
    cid[13] = 9;   // MDT.month = sept
    cid[14] = 22;  // MDT.year = 2022
    cid[15] = crc::sdCRC7(cid, 15) << 1;

    PrepareR1(0x00);
    BufferAdd(0xff);
    BufferAddBlock(DATA_TOKEN_DEFAULT, cid, sizeof(cid));
}

void EmSPISlaveSD::DoReadSCR() {
    uint8 scr[8];

    scr[0] = 0x01;  // SCR_STRUCTURE, SD_SPEC
    scr[1] = 0x25;  // DATA_STAT_AFTER_ERASE, SD_SECURITY, SD_BUS_WIDTHS
    scr[2] = 0x00;  // SD_SPEC3, EX_ SECURITY, SD_SPEC4, SD_SPECX[2..3]
    scr[3] = 0x03;  // SD_SPECX[0..1], RESERVED, CMD_SUPPORT (CMD23, CMD20)
    scr[4] = 0;     // rserved for manufacturer
    scr[5] = 0;     // rserved for manufacturer
    scr[6] = 0;     // rserved for manufacturer
    scr[7] = 0;     // rserved for manufacturer

    PrepareR1(0);
    BufferAdd(0xff);
    BufferAddBlock(DATA_TOKEN_DEFAULT, scr, 8);
}

void EmSPISlaveSD::FinishWriteSingleBlock() {
    BufferStart(0);

    if (image()->Write(buffer, currentBlock) == 1) {
        BufferAdd(DATA_RESPONSE_ACCEPTED, 0xff);
    } else {
        BufferAdd(DATA_RESPONSE_WRITE_FAILED);
    }

    cardState = CardState::initialized;
    spiState = SpiState::txData;
}

void EmSPISlaveSD::FinishWriteMultiblock() {
    BufferStart(0);

    image()->Write(buffer, (currentBlock++) % image()->BlocksTotal());
    BufferAdd(DATA_RESPONSE_ACCEPTED, 0xff);

    spiState = SpiState::txData;
}

void EmSPISlaveSD::HandleCmd12(uint8 data) {
    if (cmd12Countdown > 0 && --cmd12Countdown == 0) {
        cardState = CardState::initialized;
        spiState = SpiState::txData;
        BufferStart(0);
        PrepareR1(0);

        return;
    }

    if (data == 0x4c) {
        cmd12Countdown = 6;
    }
}

uint32 EmSPISlaveSD::Param() const {
    return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[0];
}

void EmSPISlaveSD::PrepareR1(uint8 flags) {
    BufferStart(0);
    BufferAdd(0xff, flags & 0x7f);
}

bool EmSPISlaveSD::IsSizeRepresentable(size_t blocksTotal) {
    uint32 cSizeMult, cSize;

    return determineLayout(blocksTotal, cSizeMult, cSize);
}
