//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_DMA.h"

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_IC.h"
#include "util.h"

#define PXA_DMA_BASE 0x40000000UL
#define PXA_DMA_SIZE 0x00002000UL

#define REG_DAR 0  // descriptor
#define REG_SAR 1  // source
#define REG_TAR 2  // dest
#define REG_CR 3   // command
#define REG_CSR 4  // status

struct PxaDmaChannel {
    uint32_t DAR;  // descriptor address register
    uint32_t SAR;  // source address register
    uint32_t TAR;  // target address register
    uint32_t CR;   // command register
    uint32_t CSR;  // control and status register

    uint8_t dsAddrWriten : 1;
    uint8_t dtAddrWriten : 1;
    uint8_t dcmdAddrWritten : 1;
};

struct SocDma {
    struct SocIc* ic;
    struct ArmMem* mem;

    uint32_t dalgn, dpcsr;
    uint32_t DINT;
    struct PxaDmaChannel channels[32];
    uint8_t CMR[75];  // channel map registers	[  we store lower 8 bits only :-)  ]
};

static void socDmaPrvChannelIrqRecalc(struct SocDma* dma, uint_fast8_t channel) {
    dma->DINT &= ~(1 << channel);

    if (dma->channels[channel].CSR & 0x20000000ul) {  // stop irq enabled?

        if (dma->channels[channel].CSR & 0x8) {  // channel in stoped state

            dma->DINT |= 1 << channel;
        }
    }
    if (dma->channels[channel].CSR & 4) {  // endintr requested and on?

        dma->DINT |= 1 << channel;
    }
    if (dma->channels[channel].CSR & 2) {  // startintr requested and on?

        dma->DINT |= 1 << channel;
    }
    if (dma->channels[channel].CSR & 1) {  // busints on?

        dma->DINT |= 1 << channel;
    }

    socIcInt(dma->ic, PXA_I_DMA, !!dma->DINT);
}

static void socDmaPrvChannelStop(struct SocDma* dma, struct PxaDmaChannel* ch) {
    ch->dsAddrWriten = 0;
    ch->dtAddrWriten = 0;
    ch->dcmdAddrWritten = 0;
    ch->CSR &= ~0x80000000ul;
    ch->CSR |= 0x08;
}

static bool socDmaPrvChannelRunningByCsrVal(uint32_t csr) {
    // previously was !(ch->CSR & 8)

    return (csr & 0x80000008ul) == 0x80000000ul;
}

static bool socDmaPrvChannelRunning(struct SocDma* dma, struct PxaDmaChannel* ch) {
    return socDmaPrvChannelRunningByCsrVal(ch->CSR);
}

static void socDmaPrvChannelDescrFetch(
    struct SocDma* dma,
    struct PxaDmaChannel* ch)  // you must call socDmaPrvChannelIrqRecalc() after this func
{
    uint32_t nextD, nextS, nextT, nextC, dar = ch->DAR & ~0x0f;

    if ((ch->DAR & 2) && (ch->CSR & 0x0400))  // branch mode
        dar += 32;

    if (!memAccess(dma->mem, dar + 0, 4, false, &nextD) ||
        !memAccess(dma->mem, dar + 4, 4, false, &nextS) ||
        !memAccess(dma->mem, dar + 8, 4, false, &nextT) ||
        !memAccess(dma->mem, dar + 12, 4, false, &nextC)) {
        // ERROR
        fprintf(stderr, "DMA descriptor fetch error\n");
        ch->CSR |= 1;  // signal bus error, not running

        socDmaPrvChannelStop(dma, ch);
    } else {
        ch->DAR = nextD;
        ch->SAR = nextS;
        ch->TAR = nextT;
        ch->CR = nextC;

        if (nextC & 0x00400000ul)  // start irq requested?
            ch->CSR |= 2;
    }

    // fprintf(stderr, "DESCR fetched from 0x%08x: {0x%08x 0x%08x 0x%08x 0x%08x}\n", dar, nextD,
    // nextS, nextT, nextC);
}

static bool socDmaPrvChannelCheckForEnd(
    struct SocDma* dma, uint_fast8_t channel)  // return true if irq need updating after what we did
{
    struct PxaDmaChannel* ch = &dma->channels[channel];
    bool irqUpdate = false;

    if (!socDmaPrvChannelRunning(dma, ch))  // stopped? not much to do...
        return false;

    if (!(ch->CR & 0x1fff)) {
        // fprintf(stderr, "channel %u ran out of data\n", channel);

        if (ch->CR & 0x00200000ul) {  // end irq requested?

            ch->CSR |= 4;
            irqUpdate = true;
        }

        if (ch->CSR & 0x40000000ul) {  // no descr fetch mode?	same as no descriptors

            // fprintf(stderr, "end cause no descr\n");
            socDmaPrvChannelStop(dma, ch);
            irqUpdate = true;
        } else if (ch->DAR & 1) {  // no more descriptors?

            // fprintf(stderr, "end cause DAR\n");
            socDmaPrvChannelStop(dma, ch);
            irqUpdate = true;
        } else {  // fetch next descriptor

            // fprintf(stderr, "fetching descriptor\n");
            socDmaPrvChannelDescrFetch(dma, ch);
            irqUpdate = true;
        }
    }

    return irqUpdate;
}

static bool socDmaPrvChannelDoBurst(
    struct SocDma* dma, uint_fast8_t channel)  // return true if irq need updating after what we did
{
    struct PxaDmaChannel* ch = &dma->channels[channel];
    uint32_t each = 1 << (((ch->CR >> 14) & 3) - 1);
    uint32_t num = 4 << ((ch->CR >> 16) & 3);

    if (!((ch->CR >> 14) & 3)) {
        fprintf(stderr, "DMA is on but WIDTH is misprogrammed\n");
        // this should never happen and is unpredictable on real HW. halt to allow debug
        while (1)
            ;
    }

    // we never transfer more than there is left
    if (num > (ch->CR & 0x1fff)) num = ch->CR & 0x1fff;

    if (num % each) {  // xfer size not multiple of xfer item sz?

        fprintf(stderr, "cannot xfer %u bytes using %u-byte piece. Halting\n", (unsigned)num,
                (unsigned)each);
        // this should never happen and is unpredictable on real HW. halt to allow debug
        while (1)
            ;
    }

    num /= each;  // convert from bytes to transfers

    // fprintf(stderr, "dma ch %u burst, %u bytes left before it\n", channel, ch->CR & 0x1fff);

    while (num--) {
        uint32_t src = ch->SAR;
        uint32_t dst = ch->TAR;
        uint32_t t;

        if (!memAccess(dma->mem, src, each, false, &t) ||
            !memAccess(dma->mem, dst, each, true, &t)) {
            fprintf(stderr, "DMA xfer bus error\n");
            dma->channels[channel].CSR |= 1;  // signl bus error, not running
            socDmaPrvChannelStop(dma, ch);
            return true;
        }

        if (ch->CR & 0x80000000ul) ch->SAR += each;
        if (ch->CR & 0x40000000ul) ch->TAR += each;
        ch->CR -= each;
    }

    // check for end
    return socDmaPrvChannelCheckForEnd(dma, channel);
}

static void socDmaPrvChannelActIfNeeded(struct SocDma* dma, uint_fast8_t channel) {
    bool irqUpdate = false, doWork = false, justOne = true;
    struct PxaDmaChannel* ch = &dma->channels[channel];

    if (!socDmaPrvChannelRunning(dma, ch))  // stopped? not much to do...
        return;

    // check for end
    if (socDmaPrvChannelCheckForEnd(dma, channel)) irqUpdate = true;

    // CSR.8 might change from above call, so this cannot be removed despite the earlier check

    if (socDmaPrvChannelRunning(dma, ch)) {  // if we are running, get working...

        // check for work
        if (ch->CSR & 0x100)  // request?
            doWork = true;

        if (!(ch->CR & 0x30000000ul)) {  // no flow control?

            doWork = true;
            justOne = false;  // do it all
        }

        if (doWork) {
            do {
                if (socDmaPrvChannelDoBurst(dma, channel)) irqUpdate = true;

            } while ((ch->CR & 0x1fff) && !justOne);
        }
    }

    if (irqUpdate) socDmaPrvChannelIrqRecalc(dma, channel);
}

static void socDmaPrvChannelMaybeStart(struct SocDma* dma, struct PxaDmaChannel* ch,
                                       uint32_t prevCsrVal) {
    if (socDmaPrvChannelRunning(dma, ch) && !socDmaPrvChannelRunningByCsrVal(prevCsrVal)) {
        // fprintf(stderr, "channel %u started\n", (unsigned)(ch - dma->channels));

        ch->CSR &= ~0x100;

        if (!(ch->CSR & 0x40000000ul)) {  // not no-fetch mode

            ch->CR &= ~0x1fff;  // so we fatch descr on first request

            socDmaPrvChannelDescrFetch(dma, ch);
        }
    }
}

static bool socDmaPrvChannelRegWrite(struct SocDma* dma, uint_fast8_t channel, uint_fast8_t reg,
                                     uint32_t val) {
    struct PxaDmaChannel* ch = &dma->channels[channel];
    bool checkForStart = false, maybeStart = false;
    uint32_t prevCsr = ch->CSR;

    if (reg == REG_DAR) {
        // fprintf(stderr, "DAR[%d] set to 0x%08x\n", channel, val);
        ch->DAR = val;

        // only in descr fetch mode
        if (val && !(ch->CSR & 0x40000000ul)) maybeStart = true;
    } else if (reg == REG_SAR) {
        if (ch->CSR & 0x40000000ul) checkForStart = true;
        ch->dsAddrWriten = 1;
        ch->SAR = val;
    } else if (reg == REG_TAR) {
        if (ch->CSR & 0x40000000ul) checkForStart = true;
        ch->dtAddrWriten = 1;
        ch->TAR = val;
    } else if (reg == REG_CR) {
        if (ch->CSR & 0x40000000ul) checkForStart = true;
        ch->dcmdAddrWritten = 1;
        ch->CR = val;
    } else {  // CSR

        uint32_t newVal = ((prevCsr & 0x0000031ful) | (val & 0xfcc00400ul)) & ~(val & 0x00000217ul);

        if (val & 0x02000000ul)
            newVal |= 0x0400;
        else if (val & 0x01000000ul)
            newVal &= ~0x0400;

        if (newVal & 0x00400000ul)  // MaskRun
            newVal = (newVal & ~0x80000000ul) | (prevCsr & 0x80000000ul);

        // fprintf(stderr, "csr %u write 0x%08x->0x%08x (via write of 0x%08x) \n", channel, prevCsr,
        // newVal, val);

        ch->CSR = newVal;

        socDmaPrvChannelMaybeStart(dma, &dma->channels[channel], prevCsr);

        if (!(newVal & 0x80000000ul) && (prevCsr & 0x80000000ul)) {  // just stopped

            // fprintf(stderr, "channel %u stopped\n", channel);
            socDmaPrvChannelStop(dma, ch);
        }

        socDmaPrvChannelIrqRecalc(dma, channel);

        // fprintf(stderr, "dma->DINT is now 0x%08x\n", dma->DINT);
    }

    if (checkForStart && ch->dsAddrWriten && ch->dtAddrWriten && ch->dcmdAddrWritten)
        maybeStart = true;

    if (maybeStart) {
        ch->CSR &= ~0x08;
        // fprintf(stderr, "maybe start 0x%08x -> 0x%08x\n", prevCsr, ch->CSR);
        socDmaPrvChannelMaybeStart(dma, ch, prevCsr);
    }

    return true;
}

static bool socDmaPrvChannelRegRead(struct SocDma* dma, uint_fast8_t channel, uint_fast8_t reg,
                                    uint32_t* retP) {
    switch (reg) {
        case REG_DAR:
            *retP = dma->channels[channel].DAR;
            break;

        case REG_SAR:
            *retP = dma->channels[channel].SAR;
            break;

        case REG_TAR:
            *retP = dma->channels[channel].TAR;
            break;

        case REG_CR:
            *retP = dma->channels[channel].CR;
            break;

        case REG_CSR:
            *retP = dma->channels[channel].CSR;
            break;

        default:
            return false;
    }

    return true;
}

void socDmaExternalReq(struct SocDma* dma, uint_fast8_t chNum, bool requested) {
    uint32_t cfg = dma->CMR[chNum];

    if (cfg & 0x80) {  // is it mapped to a channel at all?

        uint32_t ch = cfg & 0x1f;

        if (!requested)
            dma->channels[ch].CSR &= ~0x100;
        else if (!(dma->channels[ch].CSR & 0x100)) {  // already pending? do nothing

            dma->channels[ch].CSR |= 0x100;  // req pend
        }
    }
}

void socDmaPeriodic(struct SocDma* dma) {
    uint32_t i;

    for (i = 0; i < 32; i++) socDmaPrvChannelActIfNeeded(dma, i);
}

static bool socDmaPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                void* buf) {
    struct SocDma* dma = (struct SocDma*)userData;
    uint_fast8_t reg, set;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA_DMA_BASE) >> 2;

    if (write) {
        val = *(uint32_t*)buf;

        switch (pa >> 6) {  // weird, but quick way to avoid repeated if-then-elses. this is faster
            case 0:
                if (pa == 0x28) {
                    dma->dalgn = val;
                    break;
                } else if (pa == 0x28) {
                    dma->dpcsr = val;
                    break;
                } else if (pa == 0x3c) {
                    // no part of DINT is writeable but it MUST accept writes
                    break;
                }

                if (pa >= 32) return false;

                reg = REG_CSR;
                set = pa;
                if (!socDmaPrvChannelRegWrite(dma, set, reg, val)) return false;
                break;

            case 17:
                pa -= 1088;
                pa += 64;
                // fallthrough
            case 1:
                pa -= 64;
                if (pa >= 75) return false;
                dma->CMR[pa] = val;
                break;

            case 2:
            case 3:
                pa -= 128;
                set = pa >> 2;
                reg = pa & 3;
                if (set >= 32) return false;
                if (!socDmaPrvChannelRegWrite(dma, set, reg, val)) return false;
                break;

            default:
                return false;
        }
    } else {
        switch (pa >> 6) {  // weird, but quick way to avoide repeated if-then-elses. this is faster
            case 0:
                if (pa == 0x28) {
                    val = dma->dalgn;
                    break;
                } else if (pa == 0x28) {
                    val = dma->dpcsr;
                    break;
                } else if (pa == 0x3c) {
                    val = dma->DINT;
                    break;
                }

                if (pa >= 32) return false;

                reg = REG_CSR;
                set = pa;
                if (!socDmaPrvChannelRegRead(dma, set, reg, &val)) return false;
                break;

            case 17:
                pa -= 1088;
                pa += 64;
                // fallthrough
            case 1:
                pa -= 64;
                if (pa >= 75) return false;

                val = dma->CMR[pa];
                break;

            case 2:
            case 3:
                pa -= 128;
                set = pa >> 2;
                reg = pa & 3;
                if (set >= 32) return false;
                if (!socDmaPrvChannelRegRead(dma, set, reg, &val)) return false;
                break;

            default:
                return false;
        }

        *(uint32_t*)buf = val;
    }

    return true;
}

struct SocDma* socDmaInit(struct ArmMem* physMem, struct SocIc* ic) {
    struct SocDma* dma = (struct SocDma*)malloc(sizeof(*dma));
    uint_fast8_t i;

    if (!dma) ERR("cannot alloc DMA");

    memset(dma, 0, sizeof(*dma));
    dma->ic = ic;
    dma->mem = physMem;

    for (i = 0; i < 32; i++) dma->channels[i].CSR = 8;  // stopped or uninitialized

    if (!memRegionAdd(physMem, PXA_DMA_BASE, PXA_DMA_SIZE, socDmaPrvMemAccessF, dma))
        ERR("cannot add DMA to MEM\n");

    return dma;
}
