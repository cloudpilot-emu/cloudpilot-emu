//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "vSD.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// this is not and will never be a full SD card emulator, deal with it!

enum RejectReason {
    InvalidInCurrentState,
    UnacceptableParam,
    UnknownCommand,
};

enum State {  // matches and must match SD spec (v2 spec page 67 has the table)
    StateIdle,
    StateReady,
    StateIdent,
    StateStby,
    StateTran,
    StateData,
    StateRcv,
    StatePrg,
    StateDis,
};

struct VSD {
    SdSectorR secR;
    SdSectorW secW;
    uint32_t nSec;
    enum State state;
    uint8_t busyCount;

    uint16_t rca;

    uint32_t expectedBlockSz;
    uint8_t dataBuf[64];  // for short data in/out ops that are not SD user data

    uint8_t expectDataToUs : 1;
    uint8_t hcCard : 1;
    uint8_t acmdShift : 1;
    uint8_t reportAcmdNext : 1;

    uint8_t initWaitLeft;
    uint32_t prevAcmd41Param;

    uint8_t curBuf[512];
    uint32_t curSec;
    uint16_t curBufLen;
    bool bufIsData;
    bool bufContinuous;

    bool haveExpectedNumBlocks;
    uint32_t numBlocksExpected;
};

bool vsdIsCardBusy(struct VSD *vsd) {
    if (!vsd->busyCount) return false;
    vsd->busyCount--;
    return true;
}

static void vsdCardReset(struct VSD *vsd) {
    vsd->expectedBlockSz = 0;
    vsd->expectDataToUs = 0;
    vsd->acmdShift = 0;
    vsd->rca = 0;
    vsd->reportAcmdNext = 0;
    vsd->state = StateIdle;
    vsd->prevAcmd41Param = 0;
    vsd->initWaitLeft = 20;
}

static uint32_t vsdCalcR1resp(struct VSD *vsd, uint32_t r1bicBits, uint32_t r1orrBits) {
    uint32_t r1 = 0x0100;  // ready for data...

    r1 |= ((uint32_t)vsd->state) << 9;
    if (vsd->acmdShift || vsd->reportAcmdNext) r1 |= 1 << 5;
    vsd->reportAcmdNext = 0;

    /// XXX: "ready for data" bit 8?

    r1 &= ~r1bicBits;
    r1 |= r1orrBits;

    return r1;
}

static enum SdReplyType vsdPrepareR6resp(struct VSD *vsd, uint8_t cmd, uint8_t *reply) {
    uint32_t r1 = vsdCalcR1resp(vsd, 0, 0);

    reply[0] = cmd;
    reply[1] = vsd->rca >> 8;
    reply[2] = vsd->rca;
    reply[3] = (((r1 >> 22) & 3) << 6) | (((r1 >> 19) & 1) << 5) | ((r1 >> 8) & 0x1f);
    reply[4] = r1;

    return SdReply48bits;
}

static enum SdReplyType vsdPrepareR1resp(struct VSD *vsd, uint8_t cmd, uint8_t *reply,
                                         uint32_t r1bicBits, uint32_t r1orrBits) {
    uint32_t r1 = vsdCalcR1resp(vsd, r1bicBits, r1orrBits);

    reply[0] = cmd;
    reply[1] = r1 >> 24;
    reply[2] = r1 >> 16;
    reply[3] = r1 >> 8;
    reply[4] = r1;

    return SdReply48bits;
}

static enum SdReplyType vsdRejectCmd(struct VSD *vsd, uint8_t cmd, uint32_t param, uint8_t *reply,
                                     enum RejectReason why, bool wasAcmd) {
    uint32_t r1orr = 1UL << 19;  // generic error

    fprintf(stderr, "Rejecting %sCMD%u(0x%08lx)\n", wasAcmd ? "A" : "", cmd, (unsigned long)param);

    switch (why) {
        case InvalidInCurrentState:
            fprintf(stderr, " -> invalid in current state %u\n", vsd->state);
            r1orr = 1UL << 22;
            break;

        case UnacceptableParam:
            fprintf(stderr, " -> parameter is unacceptable\n");
            r1orr = 1UL << 31;
            break;

        case UnknownCommand:
            fprintf(stderr, " -> unknown command\n");
            return SdReplyNone;

        default:
            fprintf(stderr, " -> some other reason\n");
            break;
    }

    return vsdPrepareR1resp(vsd, cmd, reply, 0, r1orr);
}

static void vsdGetCsdV1(struct VSD *vsd, uint8_t *reply) {
    uint32_t cSizeMult, cSize, blLen, nSec = vsd->nSec, nShift = 0, sectorSize = 31;  // why not?;

    // we need to sort out how to represent the card size. first fit in C_SIZE and get minimum
    // needed shift amount, round up as needed
    while (nSec > 0x1000 || nShift < 2) {
        if (nSec & 1)  // round up
            nSec++;

        nSec >>= 1;
        nShift++;
    }
    cSize = nSec - 1;
    // then allocate the shift bits as needed
    nShift -= 2;  // part of formula
    cSizeMult = nShift > 7 ? 7 : nShift;
    nShift -= cSizeMult;
    if (nShift > 1) {
        fprintf(stderr, "card too big to be a CSD v1 card\n");
        cSize = 0;
    }
    nShift += 9;  // 512B is the base unit
    blLen = nShift;

    // now we can produce a CSD reg
    reply[0] = 0x00;
    reply[1] = 0x0e;                  // TAAC
    reply[2] = 0x00;                  // NSAC
    reply[3] = 0x5A;                  // TRAN_SPEED
    reply[4] = 0x5B;                  // CCC[4..11]
    reply[5] = blLen | 0x50;          // CCC[0..3], READ_BL_LEN
    reply[6] = (cSize >> 10) | 0x80;  // READ_BL_PARTIAL(1), WRITE_BLK_MISALIGN, READ_BLK_MISALIGN,
                                      // DSR_IMP, RESERVED, C_SIZE[10..11]
    reply[7] = cSize >> 2;            // C_SIZE[2..9]
    reply[8] = (cSize << 6) | 0x2D;   // C_SIZE[0..1], VDD_R_CURR_MIN (35mA), VDD_R_CURR_MAX(45mA)
    reply[9] =
        (cSizeMult >> 1) | 0xD8;  // VDD_W_CURR_MIN (60mA), VDD_W_CURR_MAX(80mA), C_SIZE_MULT[1..2]
    reply[10] =
        (sectorSize >> 1) + (cSizeMult << 7);  // C_SIZE_MULT[0], ERASE_BLK_EN, SECTOR_SIZE[1..7]
    reply[11] = sectorSize << 7;               // SECTOR_SIZE[0], WP_GRP_SIZE
    reply[12] = (blLen >> 2) | 8;  // WP_GRP_ENABLE, RESERVED, R2W_FACTOR(2), WRITE_BL_LEN[2..3]
    reply[13] = blLen << 6;        // WRITE_BL_LEN[0..1], WRITE_BL_PARTIAL, RESERVED
    reply[14] = 0x00;  // FILE_FORMAT_GRP, COPY, PERM_WRITE_PROTECT, TMP_WRITE_PROTECT, FILE_FORMAT,
                       // RESERVED
}

static void vsdGetCsdV2(struct VSD *vsd, uint8_t *reply) {
    uint32_t cSize = (vsd->nSec + 1023) / 1024 - 1;  // convert to unit of 512K, sub 1 as per spec
    uint32_t writeBlLen = 9 /* hardwired in SDHC */, sectorSize = 31;  // why not?

    reply[0] = 0x40;  // CSD v2
    reply[1] = 0x0e;  // TAAC
    reply[2] = 0x00;  // NSAC
    reply[3] = 0x5A;  // TRAN_SPEED
    reply[4] = 0x5B;  // CCC[4..11]
    reply[5] = 0x59;  // CCC[0..3], READ_BL_LEN
    reply[6] =
        0x00;  // READ_BL_PARTIAL, WRITE_BLK_MISALIGN, READ_BLK_MISALIGN, DSR_IMP, RESERVED[2..5]
    reply[7] = cSize >> 16;             // RESERVED[0..1], C_SIZE[16..21]
    reply[8] = cSize >> 8;              // C_SIZE[8..15]
    reply[9] = cSize;                   // C_SIZE[0..7]
    reply[10] = sectorSize >> 1;        // RESERVED, ERASE_BLK_EN, SECTOR_SIZE[1..7]
    reply[11] = sectorSize << 7;        // SECTOR_SIZE[0], WP_GRP_SIZE
    reply[12] = (writeBlLen >> 2) | 8;  // WP_GRP_ENABLE, RESERVED, R2W_FACTOR(2),
                                        // WRITE_BL_LEN[2..3]
    reply[13] = writeBlLen << 6;        // WRITE_BL_LEN[0..1], WRITE_BL_PARTIAL, RESERVED
    reply[14] = 0x00;  // FILE_FORMAT_GRP, COPY, PERM_WRITE_PROTECT, TMP_WRITE_PROTECT, FILE_FORMAT,
                       // RESERVED
}

static void vsdGetCsd(struct VSD *vsd, uint8_t *reply) {
    if (vsd->hcCard)
        vsdGetCsdV2(vsd, reply);
    else
        vsdGetCsdV1(vsd, reply);
}

enum SdReplyType vsdCommand(struct VSD *vsd, uint8_t cmd, uint32_t param,
                            void *replyOut /* should be big enough for any reply */) {
    bool replNeedsCrc = true, wasAcmd = vsd->acmdShift;
    uint8_t *reply = (uint8_t *)replyOut;
    enum SdReplyType replTyp;

    vsd->acmdShift = 0;
    reply[0] = cmd;

    switch (cmd) {
        case 0:
            vsdCardReset(vsd);
            replTyp = SdReplyNone;
            break;

        case 2:
            if (vsd->state != StateReady) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState,
                                       false /* there is no ACMD2 */);
                break;
            }
            vsd->state = StateIdent;
            goto send_cid;

        case 10:
            if (vsd->state != StateStby) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState,
                                       false /* there is no ACMD10 */);
                break;
            }

        send_cid:
            reply[0] = cmd;
            reply[1] = 'D';  // MID
            reply[2] = 'G';  // OID
            reply[3] = 'r';
            reply[4] = ' ';  // PNM
            reply[5] = 'v';
            reply[6] = 'S';
            reply[7] = 'D';
            reply[8] = ' ';
            reply[9] = 32;     // PRV
            reply[10] = 0x00;  // PSN
            reply[11] = 0x00;
            reply[12] = 0x4A;
            reply[13] = 0xEC;
            reply[14] = 3;   // MDT.month = march
            reply[15] = 20;  // MDT.year = 2020
            replTyp = SdReply136bits;
            break;

        case 3:
            if (vsd->state != StateIdent && vsd->state != StateStby) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState,
                                       false /* there is no ACMD3 */);
                break;
            }
            vsd->state = StateStby;
            vsd->rca = 3;
            replTyp = vsdPrepareR6resp(vsd, cmd, reply);
            break;

        case 6:
            if (wasAcmd) {
                if (vsd->state != StateTran)
                    replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
                else
                    replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);  // success
            } else {
                if (vsd->state != StateTran)
                    replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
                else {
                    vsd->state = StateData;

                    // exists, required, but we do not support it in vSD yet
                    ERR("CMD6 not implemented\n");
                }
            }
            break;

        case 7:
            if (vsd->state == StateStby && vsd->rca == (param >> 16)) {  // were we addressed
                vsd->state = StateTran;
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
                if (replTyp == SdReply48bits) replTyp = SdReply48bitsAndBusy;
            } else if ((vsd->state == StateStby || vsd->state == StateTran ||
                        vsd->state == StateData || vsd->state == StatePrg) &&
                       vsd->rca != (param >> 16)) {  // we were not addressed

                vsd->state = StateStby;
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            } else
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState,
                                       false /* there is no ACMD7 */);
            break;

        case 8:
            if (vsd->state != StateIdle) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState,
                                       false /* there is no ACMD8 */);
                break;
            }
            if ((param & 0xffffff00ul) != 0x100) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnacceptableParam,
                                       false /* there is no ACMD8 */);
                break;
            }
            reply[0] = cmd;
            reply[1] = 0;
            reply[2] = 0;
            reply[3] = 1;
            reply[4] = param & 0xff;
            replTyp = SdReply48bits;
            break;

        case 9:
            if (vsd->state != StateStby) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState,
                                       false /* there is no ACMD9 */);
                break;
            }
            reply[0] = cmd;
            vsdGetCsd(vsd, reply + 1);
            replTyp = SdReply136bits;
            break;

        case 12:
            if (vsd->state != StateData && vsd->state != StateRcv)
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, false);
            else {  // we do not go busy ever in here
                vsd->state = StateTran;
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            }
            break;

        case 13:
            if (wasAcmd)
                // XXX: ACMD13 exists
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            else if (vsd->state != StateStby && vsd->state != StateTran &&
                     vsd->state != StateData && vsd->state != StateRcv && vsd->state != StatePrg &&
                     vsd->state != StateDis)
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            else
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            break;

        case 16:
            if (wasAcmd) {
                // ACMD16 exists but we do not know what it does
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            } else if (vsd->state != StateTran) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            } else if (param != 512) {
                fprintf(stderr, "WE ONLY SUPPORT 512-byte xfers for vSD\n");
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnacceptableParam, wasAcmd);
            } else {
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            }
            break;

        case 17:
            vsd->haveExpectedNumBlocks = false;
            // fallthrough
        case 18:
            if (wasAcmd) {
                // ACMD17/AMCD18 exist but we do not know what it does
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            } else if (vsd->state != StateTran) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            } else if (!vsd->hcCard &&
                       (param % 512)) {  // param must be a multiple of 512b for non-HC card

                replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnacceptableParam, wasAcmd);
            } else {
                if (!vsd->hcCard) param /= 512;

                vsd->curSec = param;
                vsd->state = StateData;
                vsd->bufIsData = true;
                vsd->bufContinuous = (cmd == 18);
                vsd->curBufLen = 512;
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            }
            break;

        case 23:  // set block count

            if (vsd->state != StateTran) {  // CMD23 and ACMD23 both require TRAN state

                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            } else if (wasAcmd) {  // ACMD23

                // we do not do any erasing as this is permitted. but we accept the command
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            } else {  // CMD23

                vsd->haveExpectedNumBlocks = !!param;
                vsd->numBlocksExpected = param;
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            }
            break;

        case 24:
            vsd->haveExpectedNumBlocks = false;
            // fallthrough
        case 25:
            if (wasAcmd) {
                // ACMD24/AMCD25 exist but we do not know what it does
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            } else if (vsd->state != StateTran) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
            } else if (!vsd->hcCard &&
                       (param % 512)) {  // param must be a multiple of 512b for non-HC card

                replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnacceptableParam, wasAcmd);
            } else {
                if (!vsd->hcCard) param /= 512;

                vsd->curSec = param;
                vsd->state = StateRcv;
                vsd->bufIsData = true;
                vsd->bufContinuous = (cmd == 25);
                vsd->curBufLen = 512;
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            }
            break;

        case 41:
            if (!wasAcmd) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnknownCommand, wasAcmd);
                break;
            }
            if (vsd->state != StateIdle) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
                break;
            }
            reply[0] = 0x3f;
            reply[1] = 0x00;  // ocr reg: 0
            reply[2] = 0xff;  // ocr reg: 2.8 - 3.6V
            reply[3] = 0x80;  // ocr reg: 2.7 - 2.8V
            reply[4] = 0x00;  // ocr reg: 0
            reply[5] = 0xff;
            replNeedsCrc = false;

            replTyp = SdReply48bits;
            vsd->reportAcmdNext = 1;
            if (!(param & 0x00FFFFFFul)) {  // inquiry command

                if (vsd->prevAcmd41Param) {  // inqury not first ??

                    replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnacceptableParam, wasAcmd);
                    break;
                } else {  // repy to inquiry

                    // data already properly in reply[]
                }
            } else {
                if (!vsd->prevAcmd41Param)
                    vsd->prevAcmd41Param = param;
                else if (vsd->prevAcmd41Param != param) {  // param shall be constant
                    replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnacceptableParam, wasAcmd);
                    break;
                }

                if (vsd->initWaitLeft)
                    vsd->initWaitLeft--;
                else if (!(param & 0x40000000ul) && vsd->hcCard)
                    fprintf(stderr, "HC card refusing to init without host-signalled support\n");
                else {
                    if (vsd->hcCard) reply[1] |= 0x40;

                    reply[1] |= 0x80;  // inited
                    vsd->state = StateReady;
                }
            }
            break;

        case 51:
            if (!wasAcmd) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnknownCommand, wasAcmd);
                break;
            } else if (vsd->state != StateTran) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState, wasAcmd);
                break;
            } else {
                // SCR reg

                vsd->curBuf[0] = vsd->hcCard ? 0x02 : 0x01;  // SCR_STRUCTURE, SD_SPEC
                vsd->curBuf[1] = 0x05;  // DATA_STAT_AFTER_ERASE, SD_SECURITY, SD_BUS_WIDTHS
                vsd->curBuf[2] = 0x00;  // SD_SPEC3, EX_ SECURITY, SD_SPEC4, SD_SPECX[2..3]
                vsd->curBuf[3] = 0x03;  // SD_SPECX[0..1], RESERVED, CMD_SUPPORT (CMD23, CMD20)
                vsd->curBuf[4] = 0;     // rserved for manufacturer
                vsd->curBuf[5] = 0;     // rserved for manufacturer
                vsd->curBuf[6] = 0;     // rserved for manufacturer
                vsd->curBuf[7] = 0;     // rserved for manufacturer

                vsd->bufIsData = false;
                vsd->curBufLen = 8;

                vsd->state = StateData;
                replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            }
            break;

        case 55:
            if (vsd->state == StateReady || vsd->state == StateIdent) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, InvalidInCurrentState,
                                       false /* there is no ACMD55 */);
                break;
            }

            if ((param >> 16) != vsd->rca) {
                replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnacceptableParam,
                                       false /* there is no ACMD55 */);
                break;
            }
            vsd->acmdShift = 1;
            replTyp = vsdPrepareR1resp(vsd, cmd, reply, 0, 0);
            break;

        default:  // send R1 response indicating invalid command
            replTyp = vsdRejectCmd(vsd, cmd, param, reply, UnknownCommand, wasAcmd);
            break;
    }

    if (replNeedsCrc) switch (replTyp) {
            case SdReply48bits:
            case SdReply48bitsAndBusy:
                reply[5] = vsdCRC7(reply, 5);
                break;

            case SdReply136bits:
                reply[16] = vsdCRC7(reply, 16);
                break;

            case SdReplyNone:
                // nothing
                break;
        }
    return replTyp;
}

enum SdDataReplyType vsdDataXferBlockToCard(struct VSD *vsd, const void *data, uint32_t blockSz) {
    if (vsd->state != StateRcv) {
        fprintf(stderr, "transfer to card impossible not in rcv stage\n");
        return SdDataErrWrongCurrentState;
    }

    if (blockSz != vsd->curBufLen) {
        fprintf(stderr, "transfer to card impossible with wrong block size\n");
        return SdDataErrWrongBlockSize;
    }

    // fprintf(stderr, "host to card xfer: %u bytes for sec %u\n", blockSz, vsd->curSec);

    if (vsd->bufIsData) {
        if (!vsd->secW(vsd->curSec, data)) {
            fprintf(stderr, "failed to write SD backing store sec %lu\n",
                    (unsigned long)vsd->curSec);
            return SdDataErrBackingStore;
        }
        if (vsd->haveExpectedNumBlocks && !--vsd->numBlocksExpected)
            vsd->bufContinuous = false;  // end it here

        if (vsd->bufContinuous)
            vsd->curSec++;
        else
            vsd->state = StateTran;
    } else {
        // data to be handled here
        fprintf(stderr, "unexpected data write\n");
        return SdDataErrWrongCurrentState;
    }

    return SdDataOk;
}

enum SdDataReplyType vsdDataXferBlockFromCard(struct VSD *vsd, void *data, uint32_t blockSz) {
    if (vsd->state != StateData) {
        fprintf(stderr, "transfer from card impossible not in data stage\n");
        return SdDataErrWrongCurrentState;
    }

    if (blockSz != vsd->curBufLen) {
        fprintf(stderr, "transfer from card impossible with wrong block size\n");
        return SdDataErrWrongBlockSize;
    }

    if (vsd->bufIsData) {
        if (!vsd->secR(vsd->curSec, data)) {
            fprintf(stderr, "failed to read SD backing store sec %lu\n",
                    (unsigned long)vsd->curSec);
            return SdDataErrBackingStore;
        }

        if (vsd->haveExpectedNumBlocks && !--vsd->numBlocksExpected)
            vsd->bufContinuous = false;  // end it here

        if (vsd->bufContinuous)
            vsd->curSec++;
        else
            vsd->state = StateTran;
    } else {
        memcpy(data, vsd->curBuf, blockSz);
        vsd->state = StateTran;
    }

    // fprintf(stderr, "card to host xfer: %u bytes\n", blockSz);

    return SdDataOk;
}

struct VSD *vsdInit(SdSectorR sR, SdSectorW sW, uint32_t nSec) {
    struct VSD *vsd = (struct VSD *)malloc(sizeof(struct VSD));

    if (vsd) {
        memset(vsd, 0, sizeof(struct VSD));

        vsd->secR = sR;
        vsd->secW = sW;
        vsd->nSec = nSec;

        vsd->hcCard = nSec > 4194304;  // >2GB cards or more are reported as SDHC

        vsdCardReset(vsd);
    }

    return vsd;
}

uint8_t vsdCRC7(uint8_t *data, uint32_t sz) {
    uint8_t crc = 0;
    static const uint8_t crc7tab[] = {
        0x00, 0x12, 0x24, 0x36, 0x48, 0x5a, 0x6c, 0x7e, 0x90, 0x82, 0xb4, 0xa6, 0xd8, 0xca, 0xfc,
        0xee, 0x32, 0x20, 0x16, 0x04, 0x7a, 0x68, 0x5e, 0x4c, 0xa2, 0xb0, 0x86, 0x94, 0xea, 0xf8,
        0xce, 0xdc, 0x64, 0x76, 0x40, 0x52, 0x2c, 0x3e, 0x08, 0x1a, 0xf4, 0xe6, 0xd0, 0xc2, 0xbc,
        0xae, 0x98, 0x8a, 0x56, 0x44, 0x72, 0x60, 0x1e, 0x0c, 0x3a, 0x28, 0xc6, 0xd4, 0xe2, 0xf0,
        0x8e, 0x9c, 0xaa, 0xb8, 0xc8, 0xda, 0xec, 0xfe, 0x80, 0x92, 0xa4, 0xb6, 0x58, 0x4a, 0x7c,
        0x6e, 0x10, 0x02, 0x34, 0x26, 0xfa, 0xe8, 0xde, 0xcc, 0xb2, 0xa0, 0x96, 0x84, 0x6a, 0x78,
        0x4e, 0x5c, 0x22, 0x30, 0x06, 0x14, 0xac, 0xbe, 0x88, 0x9a, 0xe4, 0xf6, 0xc0, 0xd2, 0x3c,
        0x2e, 0x18, 0x0a, 0x74, 0x66, 0x50, 0x42, 0x9e, 0x8c, 0xba, 0xa8, 0xd6, 0xc4, 0xf2, 0xe0,
        0x0e, 0x1c, 0x2a, 0x38, 0x46, 0x54, 0x62, 0x70, 0x82, 0x90, 0xa6, 0xb4, 0xca, 0xd8, 0xee,
        0xfc, 0x12, 0x00, 0x36, 0x24, 0x5a, 0x48, 0x7e, 0x6c, 0xb0, 0xa2, 0x94, 0x86, 0xf8, 0xea,
        0xdc, 0xce, 0x20, 0x32, 0x04, 0x16, 0x68, 0x7a, 0x4c, 0x5e, 0xe6, 0xf4, 0xc2, 0xd0, 0xae,
        0xbc, 0x8a, 0x98, 0x76, 0x64, 0x52, 0x40, 0x3e, 0x2c, 0x1a, 0x08, 0xd4, 0xc6, 0xf0, 0xe2,
        0x9c, 0x8e, 0xb8, 0xaa, 0x44, 0x56, 0x60, 0x72, 0x0c, 0x1e, 0x28, 0x3a, 0x4a, 0x58, 0x6e,
        0x7c, 0x02, 0x10, 0x26, 0x34, 0xda, 0xc8, 0xfe, 0xec, 0x92, 0x80, 0xb6, 0xa4, 0x78, 0x6a,
        0x5c, 0x4e, 0x30, 0x22, 0x14, 0x06, 0xe8, 0xfa, 0xcc, 0xde, 0xa0, 0xb2, 0x84, 0x96, 0x2e,
        0x3c, 0x0a, 0x18, 0x66, 0x74, 0x42, 0x50, 0xbe, 0xac, 0x9a, 0x88, 0xf6, 0xe4, 0xd2, 0xc0,
        0x1c, 0x0e, 0x38, 0x2a, 0x54, 0x46, 0x70, 0x62, 0x8c, 0x9e, 0xa8, 0xba, 0xc4, 0xd6, 0xe0,
        0xf2};

    while (sz--) crc = crc7tab[crc ^ *data++];

    return crc + 1;
}
