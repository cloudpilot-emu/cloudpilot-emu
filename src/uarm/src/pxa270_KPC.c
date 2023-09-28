//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa270_KPC.h"

#include <stdlib.h>
#include <string.h>

#include "pxa_IC.h"
#include "util.h"

#define PXA270_KPC_BASE 0x41500000ul
#define PXA270_KPC_SIZE 0x4c

struct PxaKpc {
    struct SocIc *ic;

    // regs
    uint32_t kpc, kpmk, kpas, kpasmkp[4];
    uint16_t kpkdi;
    uint8_t kpdkChanged : 1;

    // stimuli
    uint64_t prevKeys;
    uint8_t
        matrixKeys[8];  // array indexed by column[output]. bits indexed by row (input). 1 = pressed
    uint8_t directKeys;  // bitfield, 1 = down
    uint8_t numMatrixKeysPressed;
    uint16_t jogSta[2];  // same format as KPREC reg, except we use bits 12..13 for jog sensor
                         // status
};

static void pxaKpcPrvDoAutoScan(struct PxaKpc *kpc, bool causeIrq) {
    uint64_t nowKeys = 0;
    uint_fast8_t i, c, nrows, nkeys = 0;

    for (i = 0; i < 4; i++) kpc->kpasmkp[i] = 0;

    nrows = ((kpc->kpc >> 26) & 7) + 1;
    kpc->kpas &= ~0x7c0000fful;

    for (c = 0; c <= ((kpc->kpc >> 23) & 7); c++) {
        uint_fast8_t cval = kpc->matrixKeys[c];  // get all rows for this column

        cval &= (1 << nrows) - 1;  // mask off noexistent rows

        nowKeys <<= 8;
        nowKeys += cval;

        kpc->kpasmkp[c / 2] |= ((uint32_t)cval) << (16 * (c % 2));  // record the values

        if (!cval)
            continue;
        else if (cval & (cval - 1))
            nkeys += 2;
        else if (!nkeys++) {
            kpc->kpas |= c;
            kpc->kpas |= __builtin_ctz(cval) << 4;
        }
    }

    if (nkeys == 1)
        kpc->kpas |= 0x04000000ul;
    else if (nkeys)
        kpc->kpas |= 0x100000fful;

    if (causeIrq && nowKeys != kpc->prevKeys) kpc->kpc |= 0x00400000ul;

    kpc->prevKeys = nowKeys;
}

static void pxaKpcPrvIrqRecals(struct PxaKpc *kpc) {
    bool irq = false;

    irq = irq || ((kpc->kpc & 0x00401800ul) == 0x00401800ul);
    irq = irq || ((kpc->kpc & 0x00000023ul) == 0x00000023ul);

    socIcInt(kpc->ic, PXA_I_KEYPAD, irq);
}

static void pxaKpcPrvMatrixRecalc(struct PxaKpc *kpc, bool lastKeyChangeWasDown) {
    uint_fast8_t c, val = 0;

    // ignore due to lack of matrix keypad enable?
    if (!(kpc->kpc & 0x1000)) return;

    // auto scan?
    if (kpc->kpc & 0x20000000ul) {
        // ignore due to multiple?
        if ((kpc->kpc & 0x00200000ul) &&
            ((lastKeyChangeWasDown && kpc->numMatrixKeysPressed != 1) ||
             (!lastKeyChangeWasDown && kpc->numMatrixKeysPressed != 0)))
            return;

        // do a scan
        pxaKpcPrvDoAutoScan(kpc, true);
    } else {  // manual mode?

        // calculate
        for (c = 0; c < ((kpc->kpc >> 23) & 7); c++) {
            if (!(kpc->kpc & (1 << (12 + c)))) continue;

            val |= kpc->matrixKeys[c];
        }

        // mask off unused row
        val &= ~((1 << (((kpc->kpc >> 26) & 7) + 1)) - 1);

        // irq if nonzero
        if (val) kpc->kpc |= 0x00400000ul;

        kpc->kpmk = val;
    }
    // recalc irqs
    pxaKpcPrvIrqRecals(kpc);
}

static void pxaKpcPrvDirectKeysRecalc(struct PxaKpc *kpc) {
    kpc->kpc |= 0x00000020ul;

    // recalc irqs
    pxaKpcPrvIrqRecals(kpc);
}

static void pxaKpcPrvJogRecalcRecalc(struct PxaKpc *kpc) {
    kpc->kpc |= 0x00000020ul;

    // recalc irqs
    pxaKpcPrvIrqRecals(kpc);
}

static bool pxaKpcPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                void *buf) {
    struct PxaKpc *kpc = (struct PxaKpc *)userData;
    uint32_t val = 0;

    if (size != 4 || (pa & 7)) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa = (pa - PXA270_KPC_BASE) >> 3;

    if (write) val = *(uint32_t *)buf;

    switch (pa) {
        case 0x00 / 8:
            if (write) {
                kpc->kpc = (kpc->kpc & 0x00400020ul) | (val & 0x7fbffbdful);
                if (val & 0x80000000ul)
                    pxaKpcPrvDoAutoScan(kpc, false);
                else
                    pxaKpcPrvMatrixRecalc(kpc, false);  // manual mode?
            } else {
                val = kpc->kpc;
                kpc->kpc &= ~0x00400020ul;  // reset when read
                pxaKpcPrvIrqRecals(kpc);
            }
            break;

        case 0x08 / 8:
            if (write)
                ;  // ignored
            else {
                val = (kpc->kpdkChanged ? 0x80000000ul : 0) | ((~kpc->directKeys) & 0xff);
                if (kpc->kpc & 4) {
                    val &= ~0x03;
                    val |= (kpc->jogSta[0] >> 12) & 0x03;
                }
                if (kpc->kpc & 8) {
                    val &= ~0x0c;
                    val |= (kpc->jogSta[0] >> 10) & 0x0c;
                }
                kpc->kpdkChanged = 0;
            }
            break;

        case 0x10 / 8:
            if (write) {
                kpc->jogSta[0] = (kpc->jogSta[0] & 0x0300) | (val & 0xc0ff);
                kpc->jogSta[1] = (kpc->jogSta[1] & 0x0300) | ((val >> 16) & 0xc0ff);
            } else {
                val = kpc->jogSta[1] & 0xc0ff;
                val <<= 16;
                val += kpc->jogSta[0] & 0xc0ff;

                kpc->jogSta[0] &= ~0xc000;
                kpc->jogSta[1] &= ~0xc000;
            }
            break;

        case 0x18 / 8:
            if (write)
                ;  // ignored
            else {
                val = kpc->kpmk;
                kpc->kpmk &= ~0x80000000ul;
            }
            break;

        case 0x20 / 8:
            if (write)
                ;  // ignored
            else
                val = kpc->kpas;
            break;

        case 0x28 / 8:
        case 0x30 / 8:
        case 0x38 / 8:
        case 0x40 / 8:
            if (write)
                ;  // ignored
            else
                val = kpc->kpasmkp[pa - 0x28 / 8];
            break;

        case 0x48 / 8:
            if (write)
                kpc->kpkdi = val;
            else
                val = kpc->kpkdi;
            break;

        default:
            return false;
    }

    if (!write) *(uint32_t *)buf = val;

    return true;
}

struct PxaKpc *pxaKpcInit(struct ArmMem *physMem, struct SocIc *ic) {
    struct PxaKpc *kpc = (struct PxaKpc *)malloc(sizeof(*kpc));

    if (!kpc) ERR("cannot alloc KPC");

    memset(kpc, 0, sizeof(*kpc));
    kpc->kpkdi = 0x0064;
    kpc->kpas = 0xff;
    kpc->ic = ic;

    if (!memRegionAdd(physMem, PXA270_KPC_BASE, PXA270_KPC_SIZE, pxaKpcPrvMemAccessF, kpc))
        ERR("cannot add KPC to MEM\n");

    return kpc;
}

void pxaKpcMatrixKeyChange(struct PxaKpc *kpc, uint_fast8_t row, uint_fast8_t col, bool isDown) {
    bool cur = !!((kpc->matrixKeys[col] >> row) & 1);

    if (row >= 8) ERR("only eight matrix key rows exist\n");

    if (col >= 8) ERR("only eight matrix key cols exist\n");

    if (!cur != !isDown) {  // change

        kpc->matrixKeys[col] ^= 1 << row;
        if (isDown)
            kpc->numMatrixKeysPressed++;
        else
            kpc->numMatrixKeysPressed--;

        kpc->kpmk |= 0x80000000ul;

        pxaKpcPrvMatrixRecalc(kpc, isDown);
    }
}

void pxaKpcDirectKeyChange(struct PxaKpc *kpc, uint_fast8_t keyIdx, bool isDown) {
    bool cur = !!((kpc->directKeys >> keyIdx) & 1);

    if (keyIdx >= 8) ERR("only eight direct keys exist\n");

    if (!(kpc->kpc & 2)) fprintf(stderr, "setting direct keys when direct keypad is disabled\n");

    if (!cur != !isDown) {  // change

        kpc->directKeys ^= 1 << keyIdx;
        pxaKpcPrvDirectKeysRecalc(kpc);
    }
}

void pxaKpcJogInput(struct PxaKpc *kpc, uint_fast8_t jogIdx, bool up) {
    uint8_t cur = kpc->jogSta[jogIdx] & 0xff;

    if (jogIdx >= 2) ERR("only two jog dials exist\n");

    if (!(kpc->kpc & (1 << (2 + jogIdx))))
        fprintf(stderr, "setting jog value for something that configured as a jog dial\n");

    if (up) {
        static const uint8_t jogNext[] = {2, 0, 3, 1};

        if (cur == 0xff)
            kpc->jogSta[jogIdx] = (kpc->jogSta[jogIdx] & 0x4300) | 0x8000;
        else
            kpc->jogSta[jogIdx]++;

        // rotate it
        kpc->jogSta[jogIdx] = (kpc->jogSta[jogIdx] & 0xc0ff) |
                              ((uint32_t)jogNext[(kpc->jogSta[jogIdx] >> 12) & 3]) << 12;
    } else {
        static const uint8_t jogNext[] = {1, 3, 2, 0};

        if (cur == 0x00)
            kpc->jogSta[jogIdx] = (kpc->jogSta[jogIdx] & 0x8300) | 0x40ff;
        else
            kpc->jogSta[jogIdx]--;

        // rotate it
        kpc->jogSta[jogIdx] = (kpc->jogSta[jogIdx] & 0xc0ff) |
                              ((uint32_t)jogNext[(kpc->jogSta[jogIdx] >> 12) & 3]) << 12;
    }
    pxaKpcPrvJogRecalcRecalc(kpc);
}
