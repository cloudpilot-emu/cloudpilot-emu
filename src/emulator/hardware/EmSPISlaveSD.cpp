#include "EmSPISlaveSD.h"

#include "EmCRC.h"
#include "ExternalStorage.h"

namespace {
    constexpr uint8 ERR_ILLEGAL_COMMAND = 0x04;
    constexpr uint8 ERR_CARD_IDLE = 0x01;
    constexpr uint8 ERR_PARAMETER = 0x40;

    constexpr uint8 DATA_TOKEN_DEFAULT = 0xfe;

    constexpr uint8 DATA_RESPONSE_ACCEPTED = 0x05;
    constexpr uint8 DATA_RESPONSE_WRITE_FAILED = 0x0d;

    CardImage* image() { return gExternalStorage.GetImageInSlot(EmHAL::Slot::sdcard); }
}  // namespace

void EmSPISlaveSD::Reset() {
    spiState = SpiState::notSelected;
    cardState = CardState::idle;
    lastCmd = 0;
    acmd = false;
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
            cout << "unsupported SPI shift width " << (int)bits << endl << flush;

            return 0xff;
    }
}

void EmSPISlaveSD::Enable(void) {
    if (gExternalStorage.IsMounted(EmHAL::Slot::sdcard)) {
        spiState = SpiState::rxCmdByte;
        lastCmd = 0;
    } else {
        cerr << "attempt enable SD without an inserted card" << endl << flush;
    }
}

void EmSPISlaveSD::Disable(void) { spiState = SpiState::notSelected; }

uint8 EmSPISlaveSD::DoExchange8(uint8 data) {
    if (!gExternalStorage.IsMounted(EmHAL::Slot::sdcard)) return 0x00;

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
                spiState = SpiState::txResult;
            }

            return 0xff;

        case SpiState::txResult: {
            uint8 dataOut = buffer[bufferIndex++];
            if (bufferIndex == bufferSize)
                spiState = cardState == CardState::writeTransaction ? SpiState::rxBlockWait
                                                                    : SpiState::rxCmdByte;

            return dataOut;
        }

        case SpiState::rxBlockWait:
            if (data == DATA_TOKEN_DEFAULT) {
                spiState = SpiState::rxBlock;
                BufferStart(514);
            };

            return 0xff;

        case SpiState::rxBlock:
            buffer[bufferIndex++] = data;
            if (bufferIndex == bufferSize) FinishWriteSingleBlock();

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

    switch (lastCmd) {
        case 0:
            PrepareR1(0x00);

            return;

        case 1:
            PrepareR1(0x00);
            cardState = CardState::initialized;

            return;

        case 9:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                DoReadCSD();
            }

            return;

        case 10:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                DoReadCID();
            }

            return;

        case 16:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else if (Param() != 512) {
                PrepareR1(ERR_PARAMETER);
            } else {
                PrepareR1(0x00);
            }

            return;

        case 17:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                DoReadSingleBlock();
            }

            return;

        case 24:
            if (cardState == CardState::idle) {
                PrepareR1(ERR_CARD_IDLE);
            } else {
                blockAddress = Param() >> 9;

                if (blockAddress < image()->BlockstTotal()) {
                    PrepareR1(0x00);
                    cardState = CardState::writeTransaction;
                } else {
                    PrepareR1(ERR_PARAMETER);
                }
            }

            return;

        case 55:
            PrepareR1(0x00);
            acmd = true;

            return;

        default:
            cout << "unsupported SD CMD" << (int)lastCmd << endl << flush;
            PrepareR1(ERR_ILLEGAL_COMMAND);

            return;
    }
}

void EmSPISlaveSD::DoAcmd() {
    cout << "unsupported SD ACMD" << (int)lastCmd << endl << flush;

    PrepareR1(ERR_ILLEGAL_COMMAND);
    acmd = false;
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

void EmSPISlaveSD::DoReadCSD() {
    uint8 csd[15];

    // Block size is hardcoded to 512 bytes
    const uint32 cSizeMult = 0x07, cSize = (image()->BlockstTotal() >> 9) - 1, blLen = 0x09,
                 sectorSize = 31;

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

void EmSPISlaveSD::FinishWriteSingleBlock() {
    BufferStart(0);

    if (image()->Write(buffer, blockAddress) == 1) {
        BufferAdd(DATA_RESPONSE_ACCEPTED, 0xff);
    } else {
        BufferAdd(DATA_RESPONSE_WRITE_FAILED);
    }

    cardState = CardState::initialized;
    spiState = SpiState::txResult;
}

uint32 EmSPISlaveSD::Param() const {
    return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[0];
}

void EmSPISlaveSD::PrepareR1(uint8 flags) {
    BufferStart(0);
    BufferAdd(0xff, flags & 0x7f);
}
