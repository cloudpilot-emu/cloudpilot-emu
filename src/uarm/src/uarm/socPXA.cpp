//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "CPU.h"
#include "MMU.h"
#include "RAM.h"
#include "ROM.h"
#include "SoC.h"
#include "cp15.h"
#include "mem.h"
#include "pace_patch.h"
#include "patch_dispatch.h"
#include "patches.h"
#include "scheduler.h"
#include "soc_AC97.h"
#include "soc_DMA.h"
#include "soc_GPIO.h"
#include "soc_I2C.h"
#include "soc_I2S.h"
#include "soc_IC.h"
#include "soc_SSP.h"
#include "soc_UART.h"
#include "syscall_dispatch.h"

// all PXAs
#include "pxa_DMA.h"
#include "pxa_LCD.h"
#include "pxa_MMC.h"
#include "pxa_MemCtrl.h"
#include "pxa_PWM.h"
#include "pxa_PwrClk.h"
#include "pxa_RTC.h"
#include "pxa_SSP.h"
#include "pxa_TIMR.h"
#include "pxa_UART.h"

// PXA25x/PXA26x
#include "pxa255_DSP.h"
#include "pxa255_UDC.h"

// PXA27x
#include <stdlib.h>
#include <string.h>

#include "../util.h"
#include "audio_queue.h"
#include "device.h"
#include "keys.h"
#include "peephole.h"
#include "pxa270_IMC.h"
#include "pxa270_KPC.h"
#include "pxa270_UDC.h"
#include "pxa270_WMMX.h"
#include "queue.h"
#include "reschedule.h"
#include "vSD.h"

#define CPUID_PXA255 0x69052D06ul  // spepping A0
#define CPUID_PXA260 0x69052D06ul  // spepping B1
#define CPUID_PXA270 0x49265013ul  // stepping C0

#define SRAM_BASE 0x5c000000ul
#define SRAM_SIZE 0x00040000ul

#define ROM_BASE 0x00000000UL
#define RAM_BASE 0xA0000000UL

#define PXA_I2C_BASE 0x40301680UL
#define PXA_PWR_I2C_BASE 0x40F00180UL

#define EVENT_QUEUE_CAPACITY 64

#define PCM_HZ_ENABLED 45000
#define PCM_HZ_DISABLED (44100 / 4)

struct PenEvent {
    bool penDown;
    int x, y;

    static PenEvent PenDown(int x, int y) { return {.penDown = true, .x = x, .y = y}; }

    static PenEvent PenUp() { return {.penDown = false, .x = -1, .y = -1}; }
};

struct KeyEvent {
    bool keyDown;
    enum KeyId key;

    static KeyEvent KeyDown(enum KeyId key) { return {.keyDown = true, .key = key}; }

    static KeyEvent KeyUp(enum KeyId key) { return {.keyDown = false, .key = key}; }
};

struct SoC {
    SocUart *ffUart, *hwUart, *stUart, *btUart;
    SocSsp *ssp[3];
    SocGpio *gpio;
    SocAC97 *ac97;
    SocDma *dma;
    SocI2s *i2s;
    SocI2c *i2c;
    SocIc *ic;

    bool mouseDown;
    bool enablePcmOutput;
    bool pcmSuspended;
    bool sleeping;
    uint64_t sleepAtTime;

    struct AudioQueue *audioQueue;

    Queue<PenEvent> *penEventQueue;
    Queue<KeyEvent> *keyEventQueue;

    PxaMemCtrlr *memCtrl;
    PxaPwrClk *pwrClk;
    SocI2c *pwrI2c;
    PxaPwm *pwm[4];
    PxaTimr *tmr;
    PxaMmc *mmc;
    PxaRtc *rtc;
    PxaLcd *lcd;

    union {
        // 25x/26x
        struct {
            Pxa255dsp *dsp;
            Pxa255Udc *udc1;
        };
        // PXA27x
        struct {
            Pxa270wmmx *wmmx;
            Pxa270Udc *udc2;
            PxaImc *imc;
            PxaKpc *kpc;
        };
    };

    ArmRam *sram;
    ArmRam *ram;
    ArmRam *ramMirror;       // mirror for ram termination
    ArmRom *ramWriteIgnore;  // write ignore for ram termination
    ArmRom *rom;
    ArmMem *mem;
    ArmCpu *cpu;
    Scheduler<SoC> *scheduler;
    PacePatch *pacePatch;
    PatchDispatch *patchDispatch;
    SyscallDispatch *syscallDispatch;

    Keypad *kp;
    VSD *vSD;

    Device *dev;

    uint32_t DispatchTicks(uint32_t clientType, uint32_t batchedTicks);
};

extern "C" {
static uint_fast16_t socUartPrvRead(void *userData) {
    uint_fast16_t v;
    int r;

    r = socExtSerialReadChar();

    if (r == CHAR_CTL_C)
        v = UART_CHAR_BREAK;
    else if (r == CHAR_NONE)
        v = UART_CHAR_NONE;
    else if (r >= 0x100)
        v = UART_CHAR_NONE;  // we canot send this char!!!
    else
        v = r;

    return v;
}
}

extern "C" {
static void socUartPrvWrite(uint_fast16_t chr, void *userData) {
    if (chr == UART_CHAR_NONE) return;

    socExtSerialWriteChar(chr);
}
}

extern "C" {
static void socPrvReschedule(void *ctx, uint32_t task) {
    struct SoC *soc = (struct SoC *)ctx;

    switch (task) {
        case RESCHEDULE_TASK_DEVICE_TIER0:
        case RESCHEDULE_TASK_SSP:
        case RESCHEDULE_TASK_UART:
        case RESCHEDULE_TASK_DMA:
            soc->scheduler->RescheduleTask(SCHEDULER_TASK_AUX_1, 1);
            break;
    }
}
}

static void socSetupScheduler(Scheduler<SoC> *scheduler) {
    // Timer: 3.6864 MHz
    scheduler->ScheduleTask(SCHEDULER_TASK_TIMER, 1_sec / 3686400ULL, 1);

    // RTC: 1 Hz
    scheduler->ScheduleTask(SCHEDULER_TASK_RTC, 1_sec, 1);

    // LCD: one frame every 64 ticks, 3 ticks per frame, 60 FPS -> 11.52 kHz
    scheduler->ScheduleTask(SCHEDULER_TASK_LCD, 1_sec / (64 * 3 * 60), 1);

    // Periodic tasks 0: every 36 timer ticks -> 102.4 kHz
    scheduler->ScheduleTask(SCHEDULER_TASK_AUX_1, 36_sec / 3686400ULL, 1);

    // PCM -> run at 44.3 kHz (higher than 44.1 kHz to create backpressure and
    // avoid underruns)
    scheduler->ScheduleTask(SCHEDULER_TASK_PCM, 1_sec / PCM_HZ_DISABLED, 1);

    if (deviceI2sConnected()) {
        // I2S -> run at 44.1 kHz
        scheduler->ScheduleTask(SCHEDULER_TASK_I2S, 1_sec / 44100, 1);
    }

    // Pump event queues: 25 Hz
    scheduler->ScheduleTask(SCHEDULER_TASK_AUX_2, 1_sec / 25, 1);
}

SoC *socInit(void *romData, const uint32_t romSize, uint32_t sdNumSectors, SdSectorR sdR,
             SdSectorW sdW, uint8_t *nandContent, size_t nandSize, int gdbPort,
             uint_fast8_t socRev) {
    SoC *soc = (SoC *)malloc(sizeof(SoC));
    struct SocPeriphs sp = {};
    uint32_t *ramBuffer;

    struct Reschedule rescheduleSoc = {.rescheduleCb = socPrvReschedule, .ctx = soc};

    memset(soc, 0, sizeof(*soc));

    soc->pacePatch = createPacePatch();

    soc->scheduler = new Scheduler<SoC>(*soc);
    socSetupScheduler(soc->scheduler);

    soc->penEventQueue = new Queue<PenEvent>(EVENT_QUEUE_CAPACITY);
    soc->keyEventQueue = new Queue<KeyEvent>(EVENT_QUEUE_CAPACITY);

    soc->mem = memInit();
    if (!soc->mem) ERR("Cannot init physical memory manager");

    soc->patchDispatch = initPatchDispatch();

    soc->cpu = cpuInit(ROM_BASE, soc->mem, true /* xscale */, false /* omap */, gdbPort,
                       socRev ? ((socRev == 1) ? CPUID_PXA260 : CPUID_PXA270) : CPUID_PXA255,
                       0x0B16A16AUL, soc->patchDispatch, soc->pacePatch);
    if (!soc->cpu) ERR("Cannot init CPU");

    soc->syscallDispatch = initSyscallDispatch(soc->cpu);
    registerPatches(soc->patchDispatch, soc->syscallDispatch);

    ramBuffer = (uint32_t *)malloc(deviceGetRamSize());
    if (!ramBuffer) ERR("cannot alloc RAM space\n");

    soc->ram = ramInit(soc->mem, soc, RAM_BASE, deviceGetRamSize(), ramBuffer, true);
    if (!soc->ram) ERR("Cannot init RAM");

    soc->rom = romInit(soc->mem, ROM_BASE, romData, romSize);
    if (!soc->rom) ERR("Cannot init ROM1");

    void *peepholeBuffer = romGetPeepholeBuffer(soc->rom);
    if (!peepholeBuffer) ERR("unable to obtain peephole buffer");

    pacePatchInit(soc->pacePatch, ROM_BASE, peepholeBuffer, romSize);
    peepholeOptimize((uint32_t *)peepholeBuffer, romSize);

    switch (deviceGetRamTerminationStyle()) {
        case RamTerminationMirror:

            // ram mirror for ram probe code
            soc->ramMirror = ramInit(soc->mem, soc, RAM_BASE + deviceGetRamSize(),
                                     deviceGetRamSize(), ramBuffer, false);
            if (!soc->ramMirror) ERR("Cannot init RAM mirror");
            break;

        case RamTerminationWriteIgnore:
            ERR("termination not supported");
            break;

        case RamTerminationNone:
            break;

        default:
            __builtin_unreachable();
            break;
    }

    soc->ic = socIcInit(soc->cpu, soc->mem, soc, socRev);
    if (!soc->ic) ERR("Cannot init PXA's IC");

    soc->dma = socDmaInit(soc->mem, rescheduleSoc, soc->ic);
    if (!soc->dma) ERR("Cannot init PXA's DMA");

    if (socRev == 0 || socRev == 1) {
        soc->dsp = pxa255dspInit(soc->cpu);
        if (!soc->dsp) ERR("Cannot init PXA255's DSP");
    } else if (socRev == 2) {
        soc->wmmx = pxa270wmmxInit(soc->cpu);
        if (!soc->wmmx) ERR("Cannot init PXA270's WMMX");

        soc->imc = pxaImcInit(soc->mem);
        if (!soc->imc) ERR("Cannot init PXA270's IMC");

        soc->kpc = pxaKpcInit(soc->mem, soc->ic);
        if (!soc->kpc) ERR("Cannot init PXA270's KPC");

        // SRAM
        ramBuffer = (uint32_t *)malloc(SRAM_SIZE);
        if (!ramBuffer) ERR("cannot alloc SRAM space\n");

        soc->sram = ramInit(soc->mem, soc, SRAM_BASE, SRAM_SIZE, ramBuffer, false);
        if (!soc->ram) ERR("Cannot init SRAM");
    }

    soc->gpio = socGpioInit(soc->mem, soc->ic, socRev);
    if (!soc->gpio) ERR("Cannot init PXA's GPIO");

    soc->tmr = pxaTimrInit(soc->mem, soc->ic);
    if (!soc->tmr) ERR("Cannot init PXA's OSTIMER");

    soc->rtc = pxaRtcInit(soc->mem, soc->ic);
    if (!soc->rtc) ERR("Cannot init PXA's RTC");

    soc->ffUart = socUartInit(soc->mem, rescheduleSoc, soc->ic, PXA_FFUART_BASE, PXA_I_FFUART);
    if (!soc->ffUart) ERR("Cannot init PXA's FFUART");

    if (socRev != 2) {
        soc->hwUart = socUartInit(soc->mem, rescheduleSoc, soc->ic, PXA_HWUART_BASE, PXA_I_HWUART);
        if (!soc->hwUart) ERR("Cannot init PXA's HWUART");
    }

    soc->stUart = socUartInit(soc->mem, rescheduleSoc, soc->ic, PXA_STUART_BASE, PXA_I_STUART);
    if (!soc->stUart) ERR("Cannot init PXA's STUART");

    soc->btUart = socUartInit(soc->mem, rescheduleSoc, soc->ic, PXA_BTUART_BASE, PXA_I_BTUART);
    if (!soc->btUart) ERR("Cannot init PXA's BTUART");

    soc->pwrClk = pxaPwrClkInit(soc->cpu, soc->mem, soc, socRev == 2);
    if (!soc->pwrClk) ERR("Cannot init PXA's PWRCLKMGR");

    if (socRev == 2) {
        soc->pwrI2c = socI2cInit(soc->mem, soc->ic, soc->dma, PXA_PWR_I2C_BASE, PXA_I_PWR_I2C);
        if (!soc->pwrI2c) ERR("Cannot init PXA Pwr's I2C\n");
    }

    soc->i2c = socI2cInit(soc->mem, soc->ic, soc->dma, PXA_I2C_BASE, PXA_I_I2C);
    if (!soc->i2c) ERR("Cannot init PXA's I2C");

    soc->memCtrl = pxaMemCtrlrInit(soc->mem, socRev);
    if (!soc->memCtrl) ERR("Cannot init PXA's MEMC");

    soc->ac97 = socAC97Init(soc->mem, soc->ic, soc->dma);
    if (!soc->ac97) ERR("Cannot init PXA's AC97");

    // SSP/SSP1
    soc->ssp[0] = socSspInit(soc->mem, rescheduleSoc, soc->ic, soc->dma, PXA_SSP1_BASE, PXA_I_SSP,
                             DMA_CMR_SSP_RX);
    if (!soc->ssp[0]) ERR("Cannot init PXA's SSP1");

    if (socRev == 0 || socRev == 1) {
        // NSSP
        soc->ssp[1] = socSspInit(soc->mem, rescheduleSoc, soc->ic, soc->dma, PXA_NSSP_BASE,
                                 PXA_I_NSSP, DMA_CMR_NSSP_RX);
        if (!soc->ssp[1]) ERR("Cannot init PXA's NSSP");

        soc->udc1 = pxa255UdcInit(soc->mem, soc->ic, soc->dma);
        if (!soc->udc1) ERR("Cannot init PXA255's UDC");
    }

    if (socRev == 1) {
        // ASSP
        soc->ssp[2] = socSspInit(soc->mem, rescheduleSoc, soc->ic, soc->dma, PXA_ASSP_BASE,
                                 PXA_I_ASSP, DMA_CMR_ASSP_RX);
        if (!soc->ssp[2]) ERR("Cannot init PXA26x's ASSP");
    }

    if (socRev == 2) {
        // SSP2
        soc->ssp[1] = socSspInit(soc->mem, rescheduleSoc, soc->ic, soc->dma, PXA_SSP2_BASE,
                                 PXA_I_SSP2, DMA_CMR_SSP2_RX);
        if (!soc->ssp[1]) ERR("Cannot init PXA27x's SSP2");

        // SSP3
        soc->ssp[2] = socSspInit(soc->mem, rescheduleSoc, soc->ic, soc->dma, PXA_SSP3_BASE,
                                 PXA_I_SSP3, DMA_CMR_SSP3_RX);
        if (!soc->ssp[2]) ERR("Cannot init PXA27x's SSP3");

        soc->udc2 = pxa270UdcInit(soc->mem, soc->ic, soc->dma);
        if (!soc->udc2) ERR("Cannot init PXA270's UDC");
    }

    soc->i2s = socI2sInit(soc->mem, soc->ic, soc->dma);
    if (!soc->i2s) ERR("Cannot init PXA's I2S");

    soc->pwm[0] = pxaPwmInit(soc->mem, PXA_PWM0_BASE);
    if (!soc->pwm[0]) ERR("Cannot init PXA's PWM0");

    soc->pwm[1] = pxaPwmInit(soc->mem, PXA_PWM1_BASE);
    if (!soc->pwm[1]) ERR("Cannot init PXA's PWM1");

    if (socRev == 2) {
        soc->pwm[2] = pxaPwmInit(soc->mem, PXA_PWM2_BASE);
        if (!soc->pwm[2]) ERR("Cannot init PXA's PWM2");

        soc->pwm[3] = pxaPwmInit(soc->mem, PXA_PWM3_BASE);
        if (!soc->pwm[3]) ERR("Cannot init PXA's PWM3");
    }

    soc->mmc = pxaMmcInit(soc->mem, soc->ic, soc->dma);
    if (!soc->mmc) ERR("Cannot init PXA's MMC");

    DeviceDisplayConfiguration displayConfiguration;
    deviceGetDisplayConfiguration(&displayConfiguration);

    soc->lcd =
        pxaLcdInit(soc->mem, soc, soc->ic, displayConfiguration.width, displayConfiguration.height);
    if (!soc->lcd) ERR("Cannot init PXA's LCD");

    soc->kp = keypadInit(soc->gpio, true);
    if (!soc->kp) ERR("Cannot init keypad controller");

    if (sdNumSectors) {
        soc->vSD = vsdInit(sdR, sdW, sdNumSectors);
        if (!soc->vSD) ERR("Cannot init vSD");

        pxaMmcInsert(soc->mmc, soc->vSD);
    }

    sp.mem = soc->mem;
    sp.gpio = soc->gpio;
    sp.i2c = soc->i2c;
    sp.i2s = soc->i2s;
    sp.ac97 = soc->ac97;
    sp.ssp = soc->ssp[0];
    sp.ssp2 = soc->ssp[1];
    sp.ssp3 = soc->ssp[2];
    if (socRev == 2) sp.kpc = soc->kpc;
    sp.uarts[0] = soc->ffUart;
    sp.uarts[1] = soc->hwUart;
    sp.uarts[2] = soc->stUart;
    sp.uarts[3] = soc->btUart;

    soc->dev = deviceSetup(&sp, rescheduleSoc, soc->kp, soc->vSD, nandContent, nandSize);
    if (!soc->dev) ERR("Cannot init device\n");

    if (sp.dbgUart) socUartSetFuncs(sp.dbgUart, socUartPrvRead, socUartPrvWrite, soc->hwUart);

    /*
            var gpio = {latches: [0x30000, 0x1400001, 0x200], inputs: [0x786c06, 0x100, 0x0],
       levels: [0x7b6c04, 0x1400101, 0x200], dirs: [0xcf878178, 0xffd1beff, 0x1ffff], riseDet:
       [0x1680, 0x40000, 0x0], fallDet: [0x786c04, 0x0, 0x0], AFRs: [0x4, 0xa500000a, 0x60008010,
       0xaaa00000, 0xaa0000a, 0x0]};


            var i;

            document.write("<table
       border=1><tr><td>GPIO</td><td>DIR</td><td>VAL</td><td>AFR</td><td>EDGE</td><td>NOTES</td></tr>");

            for (i = 0; i <= 80; i++) {

                    document.write("<tr><td>" + i + "</td>");

              if (gpio.dirs[i >> 5] & (1 << (i & 31))) {
                    document.write("<td>OUT</td>");
                    document.write("<td>" + ((gpio.latches[i >> 5] & (1 << (i & 31))) ? "HI" : "LO")
       + "</td>");
              }
              else {
                    document.write("<td>IN</td><td></td>");
              }

              document.write("<td>" + ((gpio.AFRs[i >> 4] >> ((i & 15) * 2)) & 3) + "</td>");
              document.write("<td>");

              if (gpio.riseDet[i >> 5] & (1 << (i & 31)))
                    document.write("RE ");
              if (gpio.fallDet[i >> 5] & (1 << (i & 31)))
                    document.write("FE ");


              document.write("</td>");

              document.write("<td></td></tr>");
            }


            document.write("</table>");
    */

    return soc;
}

void socKeyDown(SoC *soc, enum KeyId key) { soc->keyEventQueue->Push(KeyEvent::KeyDown(key)); }

void socKeyUp(SoC *soc, enum KeyId key) { soc->keyEventQueue->Push(KeyEvent::KeyUp(key)); }

void socPenDown(SoC *soc, int x, int y) { soc->penEventQueue->Push(PenEvent::PenDown(x, y)); }

void socPenUp(SoC *soc) { soc->penEventQueue->Push(PenEvent::PenUp()); }

static void socPumpEventQueues(SoC *soc) {
    if (soc->penEventQueue->GetSize() > 0) {
        PenEvent evt(soc->penEventQueue->Pop());

        if (evt.penDown) {
            while (soc->penEventQueue->GetSize() > 0 && soc->penEventQueue->Peek().penDown) {
                evt = soc->penEventQueue->Pop();
            }
        }

        if (evt.penDown || soc->mouseDown) {
            soc->mouseDown = evt.penDown;
            deviceTouch(soc->dev, evt.x, evt.y);
        }
    }

    if (soc->keyEventQueue->GetSize() > 0) {
        KeyEvent evt(soc->keyEventQueue->Pop());

        deviceKey(soc->dev, evt.key, evt.keyDown);
        keypadKeyEvt(soc->kp, evt.key, evt.keyDown);
    }
}

void socSleep(SoC *soc) {
    if (soc->sleeping) return;

    soc->sleeping = true;
    soc->scheduler->RescheduleTaskAtLeast(SCHEDULER_TASK_TIMER,
                                          pxaTimrTicksToNextInterrupt(soc->tmr));

    cpuSetSleeping(soc->cpu);

    // soc->sleepAtTime = soc->scheduler->GetTime();
    // printf("sleep\n");
}

void socWakeup(SoC *soc, uint8_t wakeupSource) {
    if (!soc->sleeping) return;

    soc->sleeping = false;
    soc->scheduler->RescheduleTaskAtLeast(SCHEDULER_TASK_TIMER, 1);

    cpuWakeup(soc->cpu);

    // printf("wakeupt after %llu nsec from %u\n", soc->scheduler->GetTime() - soc->sleepAtTime,
    //        (int)wakeupSource);
}

static void socCycleBatch0(struct SoC *soc) {
    socDmaPeriodic(soc->dma);
    socUartProcess(soc->ffUart);
    if (soc->hwUart) socUartProcess(soc->hwUart);
    socUartProcess(soc->stUart);
    socUartProcess(soc->btUart);
    for (int i = 0; i < 3; i++) {
        if (soc->ssp[i]) socSspPeriodic(soc->ssp[i]);
    }
    devicePeriodic(soc->dev, DEVICE_PERIODIC_TIER0);
}

static bool socPrvBatch0Required(struct SoC *soc) {
    if (!soc->dev) return true;

    if (socDmaTaskRequired(soc->dma)) return true;

    for (int i = 0; i < 3; i++) {
        if (soc->ssp[i] && socSspTaskRequired(soc->ssp[i])) return true;
    }

    if (deviceTaskRequired(soc->dev, DEVICE_PERIODIC_TIER0)) return true;

    if (socUartTaskRequired(soc->ffUart) || socUartTaskRequired(soc->stUart) ||
        socUartTaskRequired(soc->btUart))
        return true;

    if (soc->hwUart && socUartTaskRequired(soc->hwUart)) return true;

    return false;
}

uint32_t SoC::DispatchTicks(uint32_t clientType, uint32_t batchedTicks) {
    switch (clientType) {
        case SCHEDULER_TASK_TIMER:
            pxaTimrTick(tmr, batchedTicks);

            return sleeping ? pxaTimrTicksToNextInterrupt(tmr) : 1;

        case SCHEDULER_TASK_RTC:
            pxaRtcTick(rtc);
            return 1;

        case SCHEDULER_TASK_LCD:
            pxaLcdTick(lcd);
            return 1;

        case SCHEDULER_TASK_I2S:
            socI2sPeriodic(i2s);
            return 1;

        case SCHEDULER_TASK_PCM:
            devicePcmPeriodic(dev);
            return (pcmSuspended && enablePcmOutput) ? 0 : 1;

        case SCHEDULER_TASK_AUX_1:
            socCycleBatch0(this);
            return socPrvBatch0Required(this) ? 1 : 0;

        case SCHEDULER_TASK_AUX_2:
            socPumpEventQueues(this);
            return 1;

        default:
            ERR("invalid client type\n");
    }
}

uint64_t socRun(SoC *soc, uint64_t maxCycles, uint64_t cyclesPerSecond) {
    uint64_t cycles = 0;

    while (cycles < maxCycles) {
        uint64_t cyclesToAdvance = soc->scheduler->CyclesToNextUpdate(cyclesPerSecond);
        if (cyclesToAdvance + cycles > maxCycles) cyclesToAdvance = maxCycles - cycles;

        const uint64_t cyclesAdvanced =
            soc->sleeping ? cyclesToAdvance : cpuCycle(soc->cpu, cyclesToAdvance);

        soc->scheduler->Advance(cyclesAdvanced, cyclesPerSecond);
        cycles += cyclesAdvanced;
    }

    return cycles;
}

uint32_t *socGetPendingFrame(SoC *soc) { return pxaLcdGetPendingFrame(soc->lcd); }

void socResetPendingFrame(SoC *soc) { return pxaLcdResetPendingFrame(soc->lcd); }

void socSetFramebufferDirty(struct SoC *soc) { pxaLcdSetFramebufferDirty(soc->lcd); }

bool socSetFramebuffer(struct SoC *soc, uint32_t start, uint32_t size) {
    if (start < RAM_BASE || start - RAM_BASE + size > deviceGetRamSize()) {
        fprintf(stderr, "framebuffer not in RAM\n");
        size = 0;
    }

    ramSetFramebuffer(soc->ram, start, size);

    return size != 0;
}

void socSetAudioQueue(struct SoC *soc, struct AudioQueue *audioQueue) {
    deviceSetAudioQueue(soc->dev, audioQueue);

    soc->audioQueue = audioQueue;
}

void socSetPcmSuspended(struct SoC *soc, bool pcmSuspended) {
    if (soc->pcmSuspended == pcmSuspended) return;

    soc->pcmSuspended = pcmSuspended;
    if (soc->enablePcmOutput)
        soc->scheduler->RescheduleTask(SCHEDULER_TASK_PCM, pcmSuspended ? 0 : 1);
}

void socSetPcmOutputEnabled(struct SoC *soc, bool pcmOutputEnabled) {
    if (pcmOutputEnabled == soc->enablePcmOutput) return;

    soc->enablePcmOutput = pcmOutputEnabled;
    soc->scheduler->ScheduleTask(SCHEDULER_TASK_PCM,
                                 1_sec / (pcmOutputEnabled ? PCM_HZ_ENABLED : PCM_HZ_DISABLED),
                                 soc->pcmSuspended ? 0 : 1);

    if (soc->audioQueue) audioQueueClear(soc->audioQueue);
}
