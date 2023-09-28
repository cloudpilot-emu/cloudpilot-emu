//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_IC.h"
#include "soc_GPIO.h"
#include "util.h"

#define PXA_GPIO_BASE 0x40E00000UL
#define PXA_GPIO_SIZE 0x00001000UL

struct SocGpio {
    struct SocIc *ic;
    uint8_t socRev, nGpios;

    uint32_t latches[4];    // what pxa wants to be outputting
    uint32_t inputs[4];     // what pxa is receiving	[only set by the socGpioSetState() API]
    uint32_t levels[4];     // what pxa sees (it differs from above for IN pins)
    uint32_t dirs[4];       // 1 = output
    uint32_t riseDet[4];    // 1 = rise detect
    uint32_t fallDet[4];    // 1 = fall detect
    uint32_t detStatus[4];  // 1 = detect happened
    uint32_t AFRs[8];  // 1, 2, 3 = alt funcs. sometimes 1 is gpio and 0 is alt func (pxa 26x only)

    GpioChangedNotifF notifF[121];
    void *notifD[121];

    GpioDirsChangedF dirNotifF;
    void *dirNotifD;
};

static void socGpioPrvRecalcValues(struct SocGpio *gpio, uint_fast8_t which) {
    uint32_t val, newVal, oldVal = gpio->levels[which], t;

    if (which >= 4) {
        fprintf(stderr, "gpio overflow. halt\n");
        while (1)
            ;
    }

    val = gpio->dirs[which];
    newVal = (gpio->latches[which] & val) | (gpio->inputs[which] & (~val));

    // process change notifs
    t = newVal ^ oldVal;
    while (t) {
        uint32_t bit = 31 - __builtin_clz(t), num = 32 * which + bit;

        t &= ~(1 << bit);

        if (gpio->notifF[num])
            gpio->notifF[num](gpio->notifD[num], num, !!(oldVal & (1 << bit)),
                              !!(newVal & (1 << bit)));
    }

    // -- even AFRs read input value
    /*
    val = 3;
    bit = 1;
    for (i = 0; i < 16; i++, val <<= 2, bit <<= 1) {
            if (gpio->AFRs[(which << 1) + 0] & val)
                    newVal &=~ bit;	//all AFRs read as zero to CPU
    }

    val = 3;
    for (i = 16; i < 32; i++, val <<= 2, bit <<= 1) {

            if (gpio->socRev != 1 || which != 2 || i < 86 - 32) {

                    if (gpio->AFRs[(which << 1) + 1] & val)
                            newVal &=~ bit;	//all AFRs read as zero to CPU
            }
            else {

                    uint32_t valLoBit = val &~ (val << 1);

                    if ((gpio->AFRs[(which << 1) + 1] & val) == valLoBit)
                            newVal &=~ bit;	//all AFRs read as zero to CPU
            }
    }
    */

    gpio->levels[which] = newVal;

    if (newVal != oldVal) {
        uint32_t wentHi = newVal & ~oldVal;
        uint32_t wentLo = oldVal & ~newVal;

        gpio->detStatus[which] |= (wentHi & gpio->riseDet[which]) | (wentLo & gpio->fallDet[which]);
    }
}

static void socGpioPrvRecalcIntrs(struct SocGpio *gpio) {
    socIcInt(gpio->ic, PXA_I_GPIO_all,
             gpio->detStatus[3] || gpio->detStatus[2] || gpio->detStatus[1] ||
                 (gpio->detStatus[0] & ~3));
    socIcInt(gpio->ic, PXA_I_GPIO_1, (gpio->detStatus[0] & 2) != 0);
    socIcInt(gpio->ic, PXA_I_GPIO_0, (gpio->detStatus[0] & 1) != 0);
}

static bool socGpioPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                 void *buf) {
    struct SocGpio *gpio = (struct SocGpio *)userData;
    uint32_t val = 0, paOfst = pa & 3;
    bool dirsChanged = false;

    pa = (pa - PXA_GPIO_BASE) >> 2;

    if (write) {
        if (size != 4) {
            fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                    write ? "write" : "read", size, (unsigned long)pa);
            return false;  // we do not support non-word accesses
        }

        val = *(uint32_t *)buf;

        switch (pa) {
            case 64:
                pa -= 61;  // make the math work
                // fallthrough
            case 0:
            case 1:
            case 2:

                break;

            case 67:
                pa -= 61;  // make the math work
                // fallthrough
            case 3:
            case 4:
            case 5:
                pa -= 3;
                gpio->dirs[pa] = val;
                dirsChanged = true;
                goto recalc;

            case 70:
                pa -= 61;  // make the math work
                // fallthrough
            case 6:
            case 7:
            case 8:
                pa -= 6;
                gpio->latches[pa] |= val;
                goto recalc;

            case 73:
                pa -= 61;  // make the math work
                // fallthrough
            case 9:
            case 10:
            case 11:
                pa -= 9;
                gpio->latches[pa] &= ~val;
                goto recalc;

            case 76:
                pa -= 61;  // make the math work
                // fallthrough
            case 12:
            case 13:
            case 14:
                pa -= 12;
                gpio->riseDet[pa] = val;
                break;

            case 79:
                pa -= 61;  // make the math work
                // fallthrough
            case 15:
            case 16:
            case 17:
                pa -= 15;
                gpio->fallDet[pa] = val;
                break;

            case 82:
                pa -= 61;  // make the math work
                // fallthrough
            case 18:
            case 19:
            case 20:
                pa -= 18;
                gpio->detStatus[pa] &= ~val;
                goto trigger_intrs;

            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 29:
                pa -= 21;
                gpio->AFRs[pa] = val;
                pa /= 2;
                goto recalc;
        }

        goto done;

    recalc:
        socGpioPrvRecalcValues(gpio, pa);
        if (dirsChanged && gpio->dirNotifF) gpio->dirNotifF(gpio->dirNotifD);

    trigger_intrs:
        socGpioPrvRecalcIntrs(gpio);
    } else {
        switch (pa) {
            case 64:
                pa -= 61;  // make the math work
                // fallthrough
            case 0:
            case 1:
            case 2:
                val = gpio->levels[pa - 0];
                break;

            case 67:
                pa -= 61;  // make the math work
                // fallthrough
            case 3:
            case 4:
            case 5:
                val = gpio->dirs[pa - 3];
                break;

            case 70:
            case 73:
                pa -= 61;  // make the math work
                // fallthrough
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                val = 0;
                break;

            case 76:
                pa -= 61;  // make the math work
                // fallthrough
            case 12:
            case 13:
            case 14:
                val = gpio->riseDet[pa - 12];
                break;

            case 79:
                pa -= 61;  // make the math work
                // fallthrough
            case 15:
            case 16:
            case 17:
                val = gpio->fallDet[pa - 15];
                break;

            case 82:
                pa -= 61;  // make the math work
                // fallthrough
            case 18:
            case 19:
            case 20:
                val = gpio->detStatus[pa - 18];
                break;

            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
                val = gpio->AFRs[pa - 21];
                break;
        }

        // handle weird reads
        val >>= (8 * (paOfst & 3));

        if (size == 4)
            *(uint32_t *)buf = val;
        else if (size == 2)
            *(uint16_t *)buf = val;
        else if (size == 1)
            *(uint8_t *)buf = val;
    }

done:
    return true;
}

struct SocGpio *socGpioInit(struct ArmMem *physMem, struct SocIc *ic, uint_fast8_t socRev) {
    struct SocGpio *gpio = (struct SocGpio *)malloc(sizeof(*gpio));

    if (!gpio) ERR("cannot alloc GPIO");

    memset(gpio, 0, sizeof(*gpio));
    gpio->ic = ic;

    switch (gpio->socRev = socRev) {
        case 0:
            gpio->nGpios = 85;
            break;
        case 1:
            gpio->nGpios = 90;
            break;
        case 2:
            gpio->nGpios = 121;
            break;
    }

    if (!memRegionAdd(physMem, PXA_GPIO_BASE, PXA_GPIO_SIZE, socGpioPrvMemAccessF, gpio))
        ERR("cannot add GPIO to MEM\n");

    return gpio;
}

void socGpioSetState(struct SocGpio *gpio, uint_fast8_t gpioNum, bool on) {
    uint32_t set = gpioNum >> 5;
    uint32_t v = 1UL << (gpioNum & 0x1F);
    uint32_t *p;

    if (gpioNum >= gpio->nGpios) return;

    p = gpio->inputs + set;
    if (on)
        *p |= v;
    else
        *p &= ~v;

    socGpioPrvRecalcValues(gpio, set);
    socGpioPrvRecalcIntrs(gpio);
}

enum SocGpioState socGpioGetState(struct SocGpio *gpio, uint_fast8_t gpioNum) {
    uint32_t sSet = gpioNum >> 5;
    uint32_t bSet = gpioNum >> 4;
    uint32_t bShift = ((gpioNum & 0x0F) * 2);
    uint32_t sV = 1UL << (gpioNum & 0x1F);
    uint32_t bV = 3UL << bShift;
    uint_fast8_t afr;

    if (gpioNum >= gpio->nGpios) return SocGpioStateNoSuchGpio;

    afr = (gpio->AFRs[bSet] & bV) >> bShift;

    if (gpio->socRev == 1 && gpioNum > 85) {  // AFRS work a bit different here

        if (afr != 1) return (enum SocGpioState)(afr + SocGpioStateAFR0);
    } else {
        if (afr != 0) return (enum SocGpioState)(afr + SocGpioStateAFR0);
    }

    if (gpio->dirs[sSet] & sV)
        return (gpio->latches[sSet] & sV) ? SocGpioStateHigh : SocGpioStateLow;

    return SocGpioStateHiZ;
}

void socGpioSetNotif(struct SocGpio *gpio, uint_fast8_t gpioNum, GpioChangedNotifF notifF,
                     void *userData) {
    if (gpioNum >= gpio->nGpios) return;

    gpio->notifF[gpioNum] = notifF;
    gpio->notifD[gpioNum] = userData;
}

void socGpioSetDirsChangedNotif(struct SocGpio *gpio, GpioDirsChangedF notifF, void *userData) {
    gpio->dirNotifF = notifF;
    gpio->dirNotifD = userData;
}
