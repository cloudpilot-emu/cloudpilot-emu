//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "ac97dev_WM9712L.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


enum WM9712REG {
    RESET = 0x00,
    OUT2VOL = 0x02,
    HPVOL = 0x04,
    MONOOUTVOL = 0x06,
    DACTONECTL = 0x06,
    PCBEEPIN = 0x08,
    PHONEVOL = 0x0a,
    MICVOL = 0x0e,
    LENEINVOL = 0x10,
    AUDAC = 0x12,
    SIDETONEVOL = 0x14,
    OUT3VOL = 0x16,
    DACVOL = 0x18,
    RECSEL = 0x1a,
    RECGAIN = 0x1c,
    GENPURPOSE = 0x20,
    DEC3DCTL = 0x22,
    PDOWN1 = 0x24,
    PDOWN2 = 0x26,
    EXTDAUDIOID = 0x28,
    EXTDAUDIOCTL = 0x2a,
    DACRATE = 0x2c,
    AUXDACRATE = 0x2e,
    ADCRATE = 0x32,
    SPDIFCLT = 0x3a,
    GPIOCFG = 0x4c,
    GPIOPOLTYP = 0x4e,
    GPIOSTICKY = 0x50,
    GPIOWAKE = 0x52,
    GPIOSTA = 0x54,
    GPIOSHARE = 0x56,
    ADDFUNC1 = 0x58,
    VENDORTEST = 0x5a,
    ADDFUNC2 = 0x5c,
    ALCCTL = 0x60,
    ALCNOISEGATE = 0x62,
    AUXDACINOUTCTL = 0x64,
    DIGI1 = 0x76,
    DIGI2 = 0x78,
    DIGI_RES = 0x7a,
    VID1 = 0x7c,
    VID2 = 0x7e,
};

enum WM9712LsampleIdx {
    WM9712LsampleIdxNone = 0,
    WM9712LsampleIdxX,
    WM9712LsampleIdxY,
    WM9712LsampleIdxPressure,
    WM9712LsampleIdxAux1,
    WM9712LsampleIdxAux2,
    WM9712LsampleIdxBmon,
    WM9712LsampleIdxAux3,
};

struct WM9712L {
    struct SocGpio *gpio;
    struct SocAC97 *ac97;
    int8_t penDownPin;

    uint16_t digiRegs[3];
    uint16_t addFunc1, vendorTest, addFunc2, pdown1, pdown2, extdCtl, recSel;

    // gpio
    uint16_t gpioCfg, gpioPolTyp, gpioSticky, gpioWake, gpioStatus, gpioSharing;

    // rates
    uint16_t dacRate, auxDacRate, adcRate;

    // volumes
    uint16_t volOut2, volHP, volMono, volPhone, volMic, volOut3, volLineIn, dacVol, recGain,
        volSidetone;

    // external stimuli
    uint16_t vAux[4];  // indexed by enum WM9705auxPin
    uint16_t penX, penY, penZ;
    bool penDown;

    // for state machine
    bool haveUnreadPenData;
    uint8_t cooIdx, numUnreadDatas;
    uint16_t otherTwo[2];
};

static void wm9712LprvGpioRecalc(struct WM9712L *wm) {
    if (wm->penDownPin >= 0)
        socGpioSetState(wm->gpio, wm->penDownPin, (wm->gpioSharing & 8) ? false : wm->penDown);
}

static bool wm9712LprvCodecRegR(void *userData, uint32_t regAddr, uint16_t *regValP) {
    struct WM9712L *wm = (struct WM9712L *)userData;
    enum WM9712REG which = (enum WM9712REG)regAddr;
    uint16_t val;

    switch (which) {
        case RESET:
            val = 0x6174;
            break;

        case OUT2VOL:
            val = wm->volOut2;
            break;

        case HPVOL:
            val = wm->volHP;
            break;

        case MONOOUTVOL:
            val = wm->volMono;
            break;

        case PHONEVOL:
            val = wm->volPhone;
            break;

        case MICVOL:
            val = wm->volMic;
            break;

        case LENEINVOL:
            val = wm->volLineIn;
            break;

        case SIDETONEVOL:
            val = wm->volSidetone;
            break;

        case OUT3VOL:
            val = wm->volOut3;
            break;

        case DACVOL:
            val = wm->dacVol;
            break;

        case RECSEL:
            val = wm->recSel;
            break;

        case RECGAIN:
            val = wm->recGain;
            break;

        case PDOWN1:
            val = wm->pdown1;
            break;

        case PDOWN2:
            val = wm->pdown2;
            break;

        case EXTDAUDIOID:
            val = 0x0405;
            break;

        case EXTDAUDIOCTL:
            val = wm->extdCtl;
            break;

        case DACRATE:
            val = wm->dacRate;
            break;

        case AUXDACRATE:
            val = wm->auxDacRate;
            break;

        case ADCRATE:
            val = wm->adcRate;
            break;

        case GPIOCFG:
            val = wm->gpioCfg;
            break;

        case GPIOPOLTYP:
            val = wm->gpioPolTyp;
            break;

        case GPIOSTICKY:
            val = wm->gpioSticky;
            break;

        case GPIOWAKE:
            val = wm->gpioWake;
            break;

        case GPIOSTA:
            val = wm->gpioStatus;
            break;

        case GPIOSHARE:
            val = wm->gpioSharing;
            break;

        case ADDFUNC1:
            val = wm->addFunc1;
            break;

        case VENDORTEST:
            val = wm->vendorTest;
            break;

        case ADDFUNC2:
            val = wm->addFunc2;
            break;

        case DIGI1:
            val = wm->digiRegs[0];
            break;

        case DIGI2:
            val = wm->digiRegs[1];
            break;

        case DIGI_RES:
            val = (wm->digiRegs[2] & 0x7fff) | (wm->penDown ? 0x8000 : 0);
            wm->haveUnreadPenData = false;
            // fprintf(stderr, "touch reply (reg) 0x%04x (%s %u %u)\n", val, (val & 0x8000) ? "DN" :
            // "UP", (val >> 12) & 7, val & 0xfff);
            break;

        case VID1:
            val = 0x574D;
            break;
        case VID2:
            val = 0x4C12;
            break;

        default:
            fprintf(stderr, "unknown reg read [0x%04x]\n", (unsigned)regAddr);
            return false;
    }

    // fprintf(stderr, "codec read [0x%04x] -> %04x\n", (unsigned)regAddr, (unsigned)val);
    *regValP = val;
    return true;
}

static bool wm9712LprvCodecRegW(void *userData, uint32_t regAddr, uint16_t val) {
    struct WM9712L *wm = (struct WM9712L *)userData;
    enum WM9712REG which = (enum WM9712REG)regAddr;

    // fprintf(stderr, "codec wri [0x%04x] -> [0x%04x]\n", (unsigned)val, (unsigned)regAddr);

    switch (which) {
        case OUT2VOL:
            wm->volOut2 = val;
            break;

        case HPVOL:
            wm->volHP = val;
            break;

        case MONOOUTVOL:
            wm->volMono = val;
            break;

        case PHONEVOL:
            wm->volPhone = val;
            break;

        case MICVOL:
            wm->volMic = val;
            break;

        case LENEINVOL:
            wm->volLineIn = val;
            break;

        case SIDETONEVOL:
            wm->volSidetone = val;
            break;

        case OUT3VOL:
            wm->volOut3 = val;
            break;

        case DACVOL:
            wm->dacVol = val;
            break;

        case RECSEL:
            wm->recSel = val;
            break;

        case RECGAIN:
            wm->recGain = val;
            break;

        case PDOWN1:
            wm->pdown1 = val;
            break;

        case PDOWN2:
            wm->pdown2 = val;
            break;

        case EXTDAUDIOCTL:
            wm->extdCtl = val;
            break;

        case DACRATE:
            wm->dacRate = val;
            break;

        case AUXDACRATE:
            wm->auxDacRate = val;
            break;

        case ADCRATE:
            wm->adcRate = val;
            break;

        case GPIOCFG:
            wm->gpioCfg = val;
            wm9712LprvGpioRecalc(wm);
            break;

        case GPIOPOLTYP:
            wm->gpioPolTyp = val;
            wm9712LprvGpioRecalc(wm);
            break;

        case GPIOSTICKY:
            wm->gpioSticky = val;
            break;

        case GPIOWAKE:
            wm->gpioWake = val;
            break;

        case GPIOSTA:
            wm->gpioStatus = val;
            break;

        case GPIOSHARE:
            wm->gpioSharing = val;
            wm9712LprvGpioRecalc(wm);
            break;

        case ADDFUNC1:
            wm->addFunc1 = val;
            break;

        case VENDORTEST:
            wm->vendorTest = val;
            break;

        case ADDFUNC2:
            wm->addFunc2 = val;
            break;

        case DIGI1:
            wm->digiRegs[0] = val;
            break;

        case DIGI2:
            wm->digiRegs[1] = val;
            break;

        default:
            fprintf(stderr, "unknown reg wri [0x%04x] -> [0x%04x]\n", (unsigned)val,
                    (unsigned)regAddr);
            return false;
    }
    return false;
}

static bool wm9712LprvUnusedRegR(void *userData, uint32_t regAddr, uint16_t *regValP) {
    return false;
}

static bool wm9712LprvUnusedRegW(void *userData, uint32_t regAddr, uint16_t val) { return false; }

struct WM9712L *wm9712LInit(struct SocAC97 *ac97, struct SocGpio *gpio, int8_t penDownPin) {
    struct WM9712L *wm = (struct WM9712L *)malloc(sizeof(*wm));

    if (!wm) ERR("cannot alloc WM9712L");

    memset(wm, 0, sizeof(*wm));

    wm->penDownPin = penDownPin;
    wm->ac97 = ac97;
    wm->gpio = gpio;

    // misc regs
    wm->addFunc1 = 0x0008;
    wm->extdCtl = 0x0410;

    // gpio regs
    wm->gpioCfg = 0xf83e;
    wm->gpioPolTyp = 0xffff;
    wm->gpioSharing = 0xf83e;
    wm->recSel = 0x3000;

    // volume regs
    wm->volOut2 = 0x8000;
    wm->volHP = 0x8000;
    wm->volMono = 0x8000;
    wm->volPhone = 0xc008;
    wm->volMic = 0x6808;
    wm->volLineIn = 0xe808;
    wm->volSidetone = 0xad00;
    wm->volOut3 = 0x8000;
    wm->dacVol = 0xe808;
    wm->recGain = 0x8000;

    // rates
    wm->dacRate = 0xbb80;
    wm->auxDacRate = 0xbb80;
    wm->adcRate = 0xbb80;

    socAC97clientAdd(ac97, Ac97PrimaryAudio, wm9712LprvCodecRegR, wm9712LprvCodecRegW, wm);
    socAC97clientAdd(ac97, Ac97SecondaryAudio, wm9712LprvUnusedRegR, wm9712LprvUnusedRegW, wm);
    socAC97clientAdd(ac97, Ac97PrimaryModem, wm9712LprvUnusedRegR, wm9712LprvUnusedRegW, wm);

    return wm;
}

static void wm9712LprvNewAudioPlaybackSample(struct WM9712L *wm, uint32_t samp) {
    if (samp != 0) {
        printf("first blood\n");
        exit(0);
    }
    // nothing for now
}

static bool wm9712LprvHaveAudioOutSample(struct WM9712L *wm, uint32_t *sampP) {
    *sampP = 0;
    return true;
}

static bool wm9712LprvHaveMicOutSample(struct WM9712L *wm, uint32_t *sampP) {
    *sampP = 0;
    return true;
}

static uint16_t wm9712LprvGetSample(struct WM9712L *wm, enum WM9712LsampleIdx which) {
    uint16_t ret;

    switch (which) {
        case WM9712LsampleIdxNone:
            ret = 0;
            break;

        case WM9712LsampleIdxX:
            ret = wm->penX;
            break;

        case WM9712LsampleIdxY:
            ret = wm->penY;
            break;

        case WM9712LsampleIdxPressure:
            ret = wm->penZ;
            break;

        case WM9712LsampleIdxAux1:
            ret = wm->vAux[WM9712LauxPinAux1];
            break;

        case WM9712LsampleIdxAux2:
            ret = wm->vAux[WM9712LauxPinAux2];
            break;

        case WM9712LsampleIdxBmon:
            ret = wm->vAux[WM9712LauxPinBmon];
            break;

        case WM9712LsampleIdxAux3:
            ret = wm->vAux[WM9712LauxPinAux4];
            break;

        default:
            ret = 0;
            break;
    }

    ret &= 0x0fff;

    ret |= ((uint16_t)which) << 12;

    return ret;
}

static bool wm9712LprvHaveModemOutSample(struct WM9712L *wm, uint32_t *sampP) {
    uint16_t retVal;

    // if we are not in slot mode, and we have unread data, we cannot proceed and bail out early
    if (!(wm->digiRegs[0] & 0x08) && wm->haveUnreadPenData && (wm->digiRegs[1] & 0x0200))
        return false;

    // if we are in slot mode but any slot except 5 is selected, provide no data - pxa255 cnanot get
    // it anyways, plus this is callback for modem data (slot5)
    if ((wm->digiRegs[0] & 0x08) && (wm->digiRegs[0] & 0x07)) return false;

    // if we have unread data, send it and do nothing else
    if (wm->numUnreadDatas) retVal = wm->otherTwo[2 - wm->numUnreadDatas--];
    // else, if needed do a sampling (poll is set or continuous is set and pdet is off or pen is
    // down
    else if ((wm->digiRegs[0] & 0x8000) ||
             ((wm->digiRegs[0] & 0x0400) && (wm->penDown || !(wm->digiRegs[1] & 0x0800)))) {
        enum WM9712LsampleIdx addrIdx = (enum WM9712LsampleIdx)((wm->digiRegs[0] >> 12) & 7);

        // clear poll immediately
        wm->digiRegs[0] &= ~0x8000;

        // see if we need a set
        if (wm->digiRegs[0] & 0x0800) {
            uint16_t y;

            // get x
            retVal = wm9712LprvGetSample(wm, WM9712LsampleIdxX);

            // get y
            y = wm9712LprvGetSample(wm, WM9712LsampleIdxY);

            // get third if needed
            if (addrIdx != WM9712LsampleIdxNone) {
                wm->otherTwo[0] = y;
                wm->otherTwo[1] = wm9712LprvGetSample(wm, addrIdx);
                wm->numUnreadDatas = 2;
            } else {
                wm->otherTwo[1] = y;
                wm->numUnreadDatas = 1;
            }
        }
        // else read one thing (even if "none")
        else {
            retVal = wm9712LprvGetSample(wm, addrIdx);
        }
    }
    // else no data
    else
        return false;

    // provide ret val (not necessarily in the slot)
    if (wm->digiRegs[0] & 0x08) {
        // fprintf(stderr, "touch reply (slot) 0x%04x (%s %u %u)\n", retVal, (retVal & 0x8000) ?
        // "DN" : "UP", (retVal >> 12) & 7, retVal & 0xfff);
        *sampP = retVal | (wm->penDown ? 0x8000 : 0);
        return true;
    }

    wm->digiRegs[2] = retVal;
    return false;
}

void wm9712Lperiodic(struct WM9712L *wm) {
    uint32_t val;

    if (socAC97clientClientWantData(wm->ac97, Ac97PrimaryAudio, &val))
        wm9712LprvNewAudioPlaybackSample(wm, val);

    if (wm9712LprvHaveAudioOutSample(wm, &val))
        socAC97clientClientHaveData(wm->ac97, Ac97PrimaryAudio, val);

    if (wm9712LprvHaveMicOutSample(wm, &val))
        socAC97clientClientHaveData(wm->ac97, Ac97SecondaryAudio, val);

    if (wm9712LprvHaveModemOutSample(wm, &val))
        socAC97clientClientHaveData(wm->ac97, Ac97PrimaryModem, val);

    wm9712LprvGpioRecalc(wm);
}

void wm9712LsetAuxVoltage(struct WM9712L *wm, enum WM9712LauxPin which, uint32_t mV) {
    // vref is 3.3V

    if (mV > 3300)
        mV = 0xfff;
    else
        mV = (mV * 4095 + 3300 / 2) / 3300;

    wm->vAux[which] = mV;
}

void wm9712LsetPen(struct WM9712L *wm, int16_t x, int16_t y,
                   int16_t press)  // raw ADC values, negative for pen up
{
    if (x >= 0 && y >= 0 && press >= 0) {
        wm->penX = x;
        wm->penY = y;
        wm->penZ = press;
        wm->penDown = true;
    } else {
        wm->penX = 0xfff;
        wm->penY = 0xfff;
        wm->penZ = 0;
        wm->penDown = false;
    }

    wm9712LprvGpioRecalc(wm);
}
