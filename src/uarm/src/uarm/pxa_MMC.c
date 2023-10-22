//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_MMC.h"

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_DMA.h"
#include "util.h"

#define PXA_MMC_BASE 0x41100000UL
#define PXA_MMC_SIZE 0x00001000UL

struct PxaMmc {
    struct SocDma *dma;
    struct SocIc *ic;

    uint32_t arg;
    uint16_t stat, readTo, blkLen, numBlks;
    uint8_t spi, iMask, iReg, cmdat, clockSpeed, resTo, cmdReg;
    uint16_t respBuf[8];
    bool clockOn, cmdQueued, dataXferOngoing;

    struct VSD *vsd;

    // fifo
    uint32_t fifoBytes, fifoOfst;
    uint8_t blockFifo[1024];
};

static void pxaMmcPrvIrqUpdate(struct PxaMmc *mmc) {
    uint32_t irqs = mmc->iReg;

    // mask as per mask
    irqs &= ~mmc->iMask;

    // dma masks fifo irqs
    if (mmc->cmdat & 0x80) irqs &= ~0x60;

    socIcInt(mmc->ic, PXA_I_MMC, !!irqs);
}

static void pxaMmcPrvRecalcIregAndFifo(struct PxaMmc *mmc) {
    if (mmc->stat & 0x100)
        mmc->iReg &= ~0x10;
    else
        mmc->iReg |= 0x10;

    mmc->iReg &= ~7;
    mmc->iReg |= (mmc->stat >> 11) & 7;

    if ((mmc->cmdat & 4) && (mmc->cmdat & 8) && mmc->numBlks) {
        mmc->stat |= 0x40;
        mmc->iReg |= 0x40;
    }

    if ((mmc->cmdat & 4) && !(mmc->cmdat & 8) && (mmc->numBlks || mmc->fifoBytes)) {
        mmc->stat |= 0x80;
        mmc->iReg |= 0x20;
    }

    if (mmc->cmdat & 0x80) {
        socDmaExternalReq(mmc->dma, DMA_CMR_MMC_RX, !!(mmc->iReg & 0x20));
        socDmaExternalReq(mmc->dma, DMA_CMR_MMC_TX, !!(mmc->iReg & 0x40));
    }

    pxaMmcPrvIrqUpdate(mmc);
}

static void pxaMmcPrvDataXferNextBlockRead(struct PxaMmc *mmc) {
    enum SdDataReplyType ret;

    if (!mmc->dataXferOngoing) {
        fprintf(stderr, "Cannot read block if no xfer ongoing\n");
        return;
    }

    ret = vsdDataXferBlockFromCard(mmc->vsd, mmc->blockFifo, mmc->blkLen);
    switch (ret) {
        case SdDataErrWrongBlockSize:  // would manifest as a crc error
            mmc->stat |= 0x0008;       // crc read error
            break;

        case SdDataErrWrongCurrentState:  // would manifest as a timeout
        case SdDataErrBackingStore:
        default:
            mmc->stat |= 0x0001;
            break;

        case SdDataOk:
            mmc->fifoBytes = mmc->blkLen;
            mmc->fifoOfst = 0;
            mmc->numBlks--;
            break;
    }
}

static void pxaMmcPrvDataXferStart(struct PxaMmc *mmc) {
    mmc->dataXferOngoing = true;

    if (!mmc->blkLen || !mmc->numBlks) {
        fprintf(stderr, "MMC xfer cannot start with zero length (%u B x %u)\n", mmc->blkLen,
                mmc->numBlks);
        return;
    }

    if (mmc->cmdat & 8) {  // write

        mmc->fifoBytes = 0;
    } else {  // read

        pxaMmcPrvDataXferNextBlockRead(mmc);
    }
}

static bool pxaMmcPrvDataFifoW(struct PxaMmc *mmc, uint32_t val) {
    if (mmc->fifoBytes >= mmc->blkLen) {
        fprintf(stderr, "Cannot write over-full fifo\n");
        return false;
    }

    mmc->blockFifo[mmc->fifoBytes++] = val;

    if (mmc->fifoBytes == mmc->blkLen) {
        enum SdDataReplyType ret;

        if (!mmc->dataXferOngoing) {
            fprintf(stderr, "Cannot write block if no xfer ongoing\n");
            return false;
        }

        ret = vsdDataXferBlockToCard(mmc->vsd, mmc->blockFifo, mmc->blkLen);
        switch (ret) {
            case SdDataErrWrongBlockSize:     // would manifest as a crc error
            case SdDataErrWrongCurrentState:  // would manifest as a timeout but we report all as
                                              // crc errors
            case SdDataErrBackingStore:
            default:
                mmc->stat |= 0x0004;  // crc write error
                break;

            case SdDataOk:
                mmc->fifoBytes = 0;
                mmc->numBlks--;

                if (!mmc->numBlks) {
                    mmc->stat |= 0x0800;  // data xfer done
                    mmc->stat |= 0x1000;  // not busy
                    mmc->dataXferOngoing = false;
                }
                break;
        }
    }

    pxaMmcPrvRecalcIregAndFifo(mmc);

    return true;
}

static bool pxaMmcPrvDataFifoR(struct PxaMmc *mmc, uint32_t *valP) {
    if (!mmc->fifoBytes) {
        fprintf(stderr, "MMC unit fifo empty at read\n");
        return false;
    }

    *valP = mmc->blockFifo[mmc->fifoOfst++];
    mmc->fifoBytes--;

    if (!mmc->fifoBytes) {
        if (mmc->numBlks)
            pxaMmcPrvDataXferNextBlockRead(mmc);
        else {
            mmc->stat |= 0x0800;  // data xfer done
            mmc->dataXferOngoing = false;
        }
    }

    pxaMmcPrvRecalcIregAndFifo(mmc);
    return true;
}

static bool mmcSendCommand(struct PxaMmc *mmc) {
    bool success = false, doCrcCheck = true, withBusy = false, crcFail = false;
    enum SdReplyType ret;
    uint8_t reply[17];
    unsigned i, len;

    mmc->cmdQueued = false;

    if (mmc->vsd) {
        //	fprintf(stderr, "sending cmd %u (0x%08x)\n", mmc->cmdReg & 0x3f, mmc->arg);
        ret = vsdCommand(mmc->vsd, mmc->cmdReg & 0x3f, mmc->arg, reply);
    } else {
        //	fprintf(stderr, "MMC unit has no SD card - command ignored\n");
        ret = SdReplyNone;
    }

    switch (ret) {
        case SdReplyNone:
            len = 0;
            if ((mmc->cmdat & 3) == 0)  // no reply as expected
                success = true;
            else
                fprintf(stderr, "Got no reply, when %u bits were expected\n",
                        ((mmc->cmdat & 3) == 2) ? 136 : 48);
            break;

        case SdReply48bitsAndBusy:
            withBusy = true;
            // fallthrough

        case SdReply48bits:
            len = 5;
            if (mmc->cmdat & 1) {  // 48-bit reply as expected
                success = true;

                doCrcCheck = !(mmc->cmdat & 2);
            } else
                fprintf(stderr, "Got a 48-bit reply with, when %u bits were expected\n",
                        (mmc->cmdat & 3) ? 136 : 0);
            break;

        case SdReply136bits:
            len = 16;
            if ((mmc->cmdat & 3) == 2)  // 136-bit reply as expected
                success = true;
            else
                fprintf(stderr, "Got a 136-bit reply reply, when %u bits were expected\n",
                        (mmc->cmdat & 3) ? 48 : 0);
            break;
    }

    (void)withBusy;  // our card is never busy

    // response to handle?

    if (success && len && doCrcCheck) crcFail = vsdCRC7(reply, len) != reply[len];

    mmc->stat |= 0x2000;  // cmd-resp over
    if (crcFail) mmc->stat |= 0x20;

    if (!success)
        mmc->stat |= 0x02;  // resp timed out if wrong type
    else {
        reply[len] = 0;
        for (i = 0; i < len; i += 2)
            mmc->respBuf[i / 2] = (((uint32_t)reply[i]) << 8) + reply[i + 1];
    }

    // data to handle?
    if (success && (mmc->cmdat & 4)) pxaMmcPrvDataXferStart(mmc);

    // all cases except write, we're not busy by response time
    if (!success || !(mmc->cmdat & 4) || !(mmc->cmdat & 8)) mmc->stat |= 0x1000;  // not busy

    pxaMmcPrvRecalcIregAndFifo(mmc);

    return true;
}

static bool pxaMmcPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                void *buf) {
    struct PxaMmc *mmc = (struct PxaMmc *)userData;
    uint32_t val = 0;
    bool ret = true;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_MMC_BASE) >> 2;

    // if (write)
    //	fprintf(stderr, "MMC 0x%08x->[0x%08x]\n", *(uint32_t*)buf, (unsigned)(PXA_MMC_BASE + 4 *
    // pa));

    if (write) val = *(uint32_t *)buf;

    switch (pa) {
        case 0:
            if (!write) return false;
            switch (val & 3) {
                case 0:  // do nothing
                    break;
                case 1:  // stop clock
                    //				fprintf(stderr, "MMC: clock off\n");
                    mmc->clockOn = false;
                    mmc->stat &= ~0x100;
                    pxaMmcPrvRecalcIregAndFifo(mmc);
                    break;
                case 2:  // start clock
                    //				fprintf(stderr, "MMC: clock on\n");
                    mmc->clockOn = true;
                    mmc->stat |= 0x100;
                    pxaMmcPrvRecalcIregAndFifo(mmc);

                    if (mmc->cmdQueued) ret = mmcSendCommand(mmc);
                    break;
                default:
                    ret = false;
                    break;
            }
            pxaMmcPrvIrqUpdate(mmc);
            break;

        case 1:
            if (write) {
            }  // ignored
            else
                val = mmc->stat;
            break;

        case 2:
            if (write) {
                val &= 7;
                if (val == 7) {
                    ret = false;
                    break;
                }
                //			if (mmc->clockSpeed != val)
                //				fprintf(stderr, "MMC clock set to %u khz\n", 20000
                //>> val);
                mmc->clockSpeed = val;
            } else
                val = mmc->clockSpeed;
            break;

        case 3:
            if (write)
                mmc->spi = val & 0x0f;
            else
                val = mmc->spi;
            break;

        case 4:
            if (write) {
                mmc->cmdat = val;
                mmc->stat &= 0x100;
                mmc->stat |= 0x40;
                pxaMmcPrvRecalcIregAndFifo(mmc);

                if (mmc->clockOn)
                    ret = mmcSendCommand(mmc);
                else
                    mmc->cmdQueued = true;
            } else
                val = mmc->cmdat;
            break;

        case 5:
            if (write)
                mmc->resTo = val & 0x7f;
            else
                val = mmc->resTo;
            break;

        case 6:
            if (write)
                mmc->readTo = val;
            else
                val = mmc->readTo;
            break;

        case 7:
            if (write)
                mmc->blkLen = val & 0x3ff;
            else
                val = mmc->blkLen;
            break;

        case 8:
            if (write)
                mmc->numBlks = val;
            else
                val = mmc->numBlks;
            break;

        case 10:
            if (write) {
                mmc->iMask = val & 0x7f;
                pxaMmcPrvIrqUpdate(mmc);
            } else
                val = mmc->iMask;
            break;

        case 11:
            if (write) {
                // nothing
            } else {
                val = mmc->iReg;

                mmc->iReg &= ~0x60;
                pxaMmcPrvRecalcIregAndFifo(mmc);
            }
            break;

        case 12:
            if (write)
                mmc->cmdReg = 0x40 | (val & 0x3f);
            else
                val = mmc->cmdReg;
            break;

        case 13:
            if (write)
                mmc->arg = (mmc->arg & 0x0000fffful) | (val << 16);
            else
                val = mmc->arg >> 16;
            break;

        case 14:
            if (write)
                mmc->arg = (mmc->arg & 0xffff0000ul) | (val & 0xffff);
            else
                val = mmc->arg & 0xffff;
            break;

        case 15:  // rep buf
            if (write)
                ret = false;
            else {
                val = mmc->respBuf[0];
                memmove(mmc->respBuf, mmc->respBuf + 1,
                        sizeof(mmc->respBuf) - sizeof(*mmc->respBuf));
            }
            break;

        case 16:
            if (write)
                ret = false;
            else
                ret = pxaMmcPrvDataFifoR(mmc, &val);
            break;

        case 17:
            if (write)
                ret = pxaMmcPrvDataFifoW(mmc, val);
            else
                ret = false;
            break;

        default:
            ret = false;
            break;
    }

    if (ret) {
        if (!write) *(uint32_t *)buf = val;

        //	if (!write)
        //		fprintf(stderr, "MMC [0x%08x] -> 0x%08x\n", (unsigned)(PXA_MMC_BASE + 4 *
        //pa), val);
    }

    return ret;
}

struct PxaMmc *pxaMmcInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma) {
    struct PxaMmc *mmc = (struct PxaMmc *)malloc(sizeof(*mmc));

    if (!mmc) ERR("cannot alloc MMC");

    memset(mmc, 0, sizeof(*mmc));

    mmc->ic = ic;
    mmc->dma = dma;
    mmc->iMask = 0x7f;
    mmc->cmdat = 0x80;
    mmc->resTo = 0x40;
    mmc->cmdReg = 0x40;
    mmc->stat = 0x40;
    pxaMmcPrvRecalcIregAndFifo(mmc);

    if (!memRegionAdd(physMem, PXA_MMC_BASE, PXA_MMC_SIZE, pxaMmcPrvMemAccessF, mmc))
        ERR("cannot add MMC to MEM\n");

    return mmc;
}

void pxaMmcInsert(struct PxaMmc *mmc, struct VSD *vsd) { mmc->vsd = vsd; }
