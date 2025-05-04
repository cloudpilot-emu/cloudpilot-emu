//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

//  stdlib
#include <cstdlib>
#include <cstring>
#include <iostream>

// all PXAs
#include "pxa_AC97.h"
#include "pxa_DMA.h"
#include "pxa_GPIO.h"
#include "pxa_I2C.h"
#include "pxa_I2S.h"
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
#include "pxa270_IMC.h"
#include "pxa270_KPC.h"
#include "pxa270_UDC.h"
#include "pxa270_WMMX.h"

// Everything else
#include "CPU.h"
#include "Logging.h"
#include "MMU.h"
#include "RAM.h"
#include "ROM.h"
#include "SoC.h"
#include "audio_queue.h"
#include "cp15.h"
#include "cputil.h"
#include "device.h"
#include "keys.h"
#include "mem.h"
#include "memory_buffer.h"
#include "pace_patch.h"
#include "patch_dispatch.h"
#include "patches.h"
#include "peephole.h"
#include "queue.h"
#include "reschedule.h"
#include "savestate/savestateAll.h"
#include "scheduler.h"
#include "sdcard.h"
#include "soc_DMA.h"
#include "soc_IC.h"
#include "soc_SSP.h"
#include "soc_UART.h"
#include "syscall_dispatch.h"
#include "vSD.h"

#define SAVESTATE_VERSION 2

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

#define PCM_HZ_ENABLED 44300
#define PCM_HZ_DISABLED (44100 / 3)

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
    uint8_t socRev;

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

    uint64_t injectedTimeNsec;
    uint16_t paceBreakSyscall;

    bool cardInserted;
    char cardId[SD_CARD_ID_MAX_LEN + 1];

    struct AudioQueue *audioQueue;

    Queue<PenEvent> *penEventQueue;
    Queue<KeyEvent> *keyEventQueue;

    NAND *nand;
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

    MemoryBuffer bufferMemory;
    MemoryBuffer bufferNand;
    MemoryBuffer bufferLcd;
    MemoryBuffer bufferSram;

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

    Savestate<ChunkType> *savestate;

    uint32_t DispatchTicks(uint32_t clientType, uint32_t batchedTicks);

    void Load(SavestateLoader<ChunkType> &loader);

    template <typename T>
    void Save(T &t);

    template <typename T>
    void DoSaveLoad(T &chunkHelper, uint32_t version);
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
            cpuSetSlowPath(soc->cpu, SLOW_PATH_REASON_RESCHEDULE);
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

    // Pump event queues: 30 Hz
    scheduler->ScheduleTask(SCHEDULER_TASK_AUX_2, 1_sec / 30, 1);
}

static void schedulePcmTask(SoC *soc) {
    soc->scheduler->ScheduleTask(SCHEDULER_TASK_PCM,
                                 1_sec / (soc->enablePcmOutput ? PCM_HZ_ENABLED : PCM_HZ_DISABLED),
                                 soc->pcmSuspended ? 0 : 1);
}

static void socAllocateBuffers(SoC *soc) {
    size_t memoryBufferSize = deviceGetRamSize() + 2 * MEMORY_BUFFER_GRANULARITY;
    if (soc->socRev == 2) memoryBufferSize += SRAM_SIZE;

    bool success = memoryBufferAllocate(&soc->bufferMemory, memoryBufferSize);

    size_t offset = deviceGetRamSize();

    success = success && memoryBufferGetSubBuffer(&soc->bufferMemory, &soc->bufferNand, offset,
                                                  MEMORY_BUFFER_GRANULARITY);
    offset += MEMORY_BUFFER_GRANULARITY;

    success = success && memoryBufferGetSubBuffer(&soc->bufferMemory, &soc->bufferLcd, offset,
                                                  MEMORY_BUFFER_GRANULARITY);
    offset += MEMORY_BUFFER_GRANULARITY;

    if (soc->socRev == 2)
        success = success &&
                  memoryBufferGetSubBuffer(&soc->bufferMemory, &soc->bufferSram, offset, SRAM_SIZE);

    if (!success) ERR("failed to allocate memory buffers");
}

SoC *socInit(enum DeviceType deviceType, void *romData, const uint32_t romSize,
             uint8_t *nandContent, size_t nandSize, int gdbPort, uint_fast8_t socRev) {
    SoC *soc = (SoC *)malloc(sizeof(SoC));
    struct SocPeriphs sp = {};

    struct Reschedule rescheduleSoc = {.rescheduleCb = socPrvReschedule, .ctx = soc};

    memset(soc, 0, sizeof(*soc));
    soc->socRev = socRev;

    socAllocateBuffers(soc);

    soc->savestate = new Savestate<ChunkType>();

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

    patchDispatchSetCpu(soc->patchDispatch, soc->cpu);

    soc->syscallDispatch = initSyscallDispatch(soc);
    registerPatches(soc->patchDispatch, soc->syscallDispatch);

    soc->ram = ramInit(soc->mem, soc, RAM_BASE, deviceGetRamSize(), &soc->bufferMemory, true);
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
                                     deviceGetRamSize(), &soc->bufferMemory, false);
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

        soc->sram = ramInit(soc->mem, soc, SRAM_BASE, SRAM_SIZE, &soc->bufferSram, false);
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
    deviceGetDisplayConfiguration(deviceType, &displayConfiguration);

    soc->lcd = pxaLcdInit(soc->mem, soc, soc->ic, &soc->bufferLcd, displayConfiguration.width,
                          displayConfiguration.height);
    if (!soc->lcd) ERR("Cannot init PXA's LCD");

    soc->kp = keypadInit(soc->gpio, true);
    if (!soc->kp) ERR("Cannot init keypad controller");

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

    soc->dev = deviceSetup(deviceType, &sp, rescheduleSoc, soc->kp, soc->vSD, nandContent, nandSize,
                           &soc->bufferNand);
    if (!soc->dev) ERR("Cannot init device\n");

    soc->vSD = vsdInit(sdCardRead, sdCardWrite, 0);
    socSdEject(soc);

    soc->nand = sp.nand;

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

void socReset(struct SoC *soc) {
    cpuReset(soc->cpu, 0);
    pxaPwrClkReset(soc->pwrClk);

    socWakeup(soc, 0);
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
            if (!syscallDispatchInProgress(syscallDispatch)) devicePcmPeriodic(dev);
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

template <int breakReason, bool injected>
static FORCE_INLINE uint64_t socRunUntil(SoC *soc, struct ArmCpu *cpu, uint64_t maxCycles,
                                         uint64_t cyclesPerSecond) {
    uint64_t cycles = 0;

    while (cycles < maxCycles) {
        uint64_t cyclesToAdvance = soc->scheduler->CyclesToNextUpdate(cyclesPerSecond);

        if (cyclesToAdvance + cycles > maxCycles) cyclesToAdvance = maxCycles - cycles;

        uint64_t cyclesAdvanced;

        if constexpr (injected) {
            cyclesAdvanced = soc->sleeping ? cyclesToAdvance : cpuCycle<injected>(cpu, 1);
        } else {
            cyclesAdvanced =
                soc->sleeping ? cyclesToAdvance : cpuCycle<injected>(cpu, cyclesToAdvance);
        }

        soc->scheduler->Advance(cyclesAdvanced, cyclesPerSecond);
        cycles += cyclesAdvanced;

        if constexpr (breakReason > 0) {
            if (cpuGetSlowPathReason(cpu) & breakReason) break;
        }
    }

    return cycles;
}

uint64_t socRun(SoC *soc, uint64_t maxCycles, uint64_t cyclesPerSecond) {
    soc->paceBreakSyscall = 0;

    return socRunUntil<0, false>(soc, soc->cpu, maxCycles, cyclesPerSecond);
}

bool socRunToPaceSyscall(struct SoC *soc, uint16_t syscall, uint64_t maxCycles,
                         uint64_t cyclesPerSecond) {
    if (syscall == soc->paceBreakSyscall) return true;

    cpuSetBreakPaceSyscall(soc->cpu, syscall);
    const uint64_t cycles = socRunUntil<SLOW_PATH_REASON_PACE_SYSCALL_BREAK, false>(
        soc, soc->cpu, maxCycles, cyclesPerSecond);
    cpuSetBreakPaceSyscall(soc->cpu, 0);

    soc->injectedTimeNsec += (cycles * 1000000) / (cyclesPerSecond / 1000);

    if ((cpuGetSlowPathReason(soc->cpu) & SLOW_PATH_REASON_PACE_SYSCALL_BREAK) == 0) return false;

    soc->paceBreakSyscall = syscall;

    return true;
}

bool socExecuteInjected(struct SoC *soc, struct ArmCpu *cpu, uint64_t maxCycles,
                        uint64_t cyclesPerSecond) {
    const uint64_t cycles = socRunUntil<SLOW_PATH_REASON_INJECTED_CALL_DONE, true>(
        soc, cpu, maxCycles, cyclesPerSecond);

    soc->injectedTimeNsec += (cycles * 1000000) / (cyclesPerSecond / 1000);

    return cpuGetSlowPathReason(cpu) & SLOW_PATH_REASON_INJECTED_CALL_DONE;
}

uint64_t socGetInjectedTimeNsec(struct SoC *soc) { return soc->injectedTimeNsec; }

void socResetInjectedTimeNsec(struct SoC *soc) { soc->injectedTimeNsec = 0; }

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
    if (soc->enablePcmOutput) {
        soc->scheduler->RescheduleTask(SCHEDULER_TASK_PCM, pcmSuspended ? 0 : 1);

        if (!pcmSuspended) cpuSetSlowPath(soc->cpu, SLOW_PATH_REASON_RESCHEDULE);
    }
}

void socSetPcmOutputEnabled(struct SoC *soc, bool pcmOutputEnabled) {
    if (soc->audioQueue && pcmOutputEnabled) audioQueueClear(soc->audioQueue);
    if (pcmOutputEnabled == soc->enablePcmOutput) return;

    soc->enablePcmOutput = pcmOutputEnabled;
    schedulePcmTask(soc);
}

struct Buffer socGetRomData(struct SoC *soc) {
    return {.size = romGetSize(soc->rom), .data = romGetData(soc->rom)};
}

struct Buffer socGetNandData(struct SoC *soc) { return nandGetData(soc->nand); }

struct Buffer socGetNandDirtyPages(struct SoC *soc) { return nandGetDirtyPages(soc->nand); }

bool socIsNandDirty(struct SoC *soc) { return nandIsDirty(soc->nand); }

void socSetNandDirty(struct SoC *soc, bool isDirty) { nandSetDirty(soc->nand, isDirty); }

struct Buffer socGetMemoryData(struct SoC *soc) {
    return {.size = soc->bufferMemory.size, .data = soc->bufferMemory.buffer};
}

struct Buffer socGetMemoryDirtyPages(struct SoC *soc) {
    return {.size = soc->bufferMemory.dirtyPagesSize, .data = soc->bufferMemory.dirtyPages};
}

void socSdInsert(struct SoC *soc) {
    if (soc->cardInserted || !sdCardInitialized()) return;

    vsdReset(soc->vSD, sdCardSectorCount());
    pxaMmcInsert(soc->mmc, soc->vSD);
    deviceSetSdCardInserted(soc->dev, true);

    soc->cardInserted = true;
}

bool socSdRemount(struct SoC *soc) {
    if (!soc->cardInserted) return false;

    if (!sdCardInitialized() || strncmp(soc->cardId, sdCardGetId(), SD_CARD_ID_MAX_LEN) != 0) {
        socSdEject(soc);
        return false;
    } else {
        pxaMmcInsert(soc->mmc, soc->vSD);
    }

    return true;
}

void socSdEject(struct SoC *soc) {
    if (!soc->cardInserted) return;

    vsdReset(soc->vSD, 0);
    pxaMmcInsert(soc->mmc, nullptr);
    deviceSetSdCardInserted(soc->dev, false);

    soc->cardInserted = false;
}

bool socSdInserted(struct SoC *soc) { return soc->cardInserted; }

bool socSave(struct SoC *soc) { return soc->savestate->Save(*soc); }

bool socLoad(struct SoC *soc, size_t savestateSize, void *savestateData) {
    if (savestateSize == 0 || !savestateData) return true;

    Savestate<ChunkType> savestate;
    SavestateLoader<ChunkType> loader;

    savestate.Save(*soc);

    if (!loader.Load(savestateData, savestateSize, *soc)) {
        if (!loader.Load(savestate.GetBuffer(), savestate.GetSize(), *soc))
            ERR("failed to restore initial state after load failure");

        return false;
    }

    return true;
}

struct Buffer socGetSavestate(struct SoC *soc) {
    return {.size = soc->savestate->GetSize(), .data = soc->savestate->GetBuffer()};
}

enum DeviceType socGetDeviceType(struct SoC *soc) { return deviceGetType(soc->dev); }

void socDumpMMU(struct SoC *soc) { mmuDump(cpuGetMMU(soc->cpu)); }

struct ArmCpu *socGetCpu(struct SoC *soc) { return soc->cpu; }

struct SyscallDispatch *socGetSyscallDispatch(struct SoC *soc) { return soc->syscallDispatch; }

bool socIsPacePatched(struct SoC *soc) { return soc->pacePatch->enterPace; }

void SoC::Load(SavestateLoader<ChunkType> &loader) {
    pxaUartLoad(ffUart, loader, 0);
    if (socRev != 2) pxaUartLoad(hwUart, loader, 1);
    pxaUartLoad(stUart, loader, 2);
    pxaUartLoad(btUart, loader, 3);

    pxaSspLoad(ssp[0], loader, 0);
    pxaSspLoad(ssp[1], loader, 1);
    if (socRev > 0) pxaSspLoad(ssp[2], loader, 2);

    pxaGpioLoad(gpio, loader);
    pxaAC97Load(ac97, loader);
    pxaDmaLoad(dma, loader);
    pxaI2sLoad(i2s, loader);
    pxaI2cLoad(i2c, loader, 0);
    pxaIcLoad(ic, loader);
    pxaMemCtrlrLoad(memCtrl, loader);
    pxaPwrClkLoad(pwrClk, loader);
    if (socRev == 2) pxaI2cLoad(pwrI2c, loader, 1);

    pxaPwmLoad(pwm[0], loader, 0);
    pxaPwmLoad(pwm[1], loader, 1);
    if (socRev == 2) {
        pxaPwmLoad(pwm[2], loader, 2);
        pxaPwmLoad(pwm[3], loader, 3);
    }

    pxaTimrLoad(tmr, loader);
    pxaMmcLoad(mmc, loader);
    pxaRtcLoad(rtc, loader);
    pxaLcdLoad(lcd, loader);

    if (socRev == 2) {
        pxa255dspLoad(dsp, loader);
        pxa255UdcLoad(udc1, loader);
    } else {
        // no 27x for now
    }

    cpuLoad(cpu, loader);
    scheduler->Load(loader);
    deviceLoad(dev, loader);
    vsdLoad(vSD, loader);

    uint32_t version;
    Chunk *chunk = loader.GetChunk(ChunkType::pxaSoc, SAVESTATE_VERSION, "socPXA", version);
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper, version);

    schedulePcmTask(this);
    keypadReset(kp);
}

template <typename T>
void SoC::Save(T &savestate) {
    pxaUartSave(ffUart, savestate, 0);
    if (socRev != 2) pxaUartSave(hwUart, savestate, 1);
    pxaUartSave(stUart, savestate, 2);
    pxaUartSave(btUart, savestate, 3);

    pxaSspSave(ssp[0], savestate, 0);
    pxaSspSave(ssp[1], savestate, 1);
    if (socRev > 0) pxaSspSave(ssp[2], savestate, 2);

    pxaGpioSave(gpio, savestate);
    pxaAC97Save(ac97, savestate);
    pxaDmaSave(dma, savestate);
    pxaI2sSave(i2s, savestate);
    pxaI2cSave(i2c, savestate);
    pxaIcSave(ic, savestate);
    pxaMemCtrlrSave(memCtrl, savestate);
    pxaPwrClkSave(pwrClk, savestate);
    if (socRev == 2) pxaI2cSave(pwrI2c, savestate, 1);

    pxaPwmSave(pwm[0], savestate, 0);
    pxaPwmSave(pwm[1], savestate, 1);
    if (socRev == 2) {
        pxaPwmSave(pwm[2], savestate, 2);
        pxaPwmSave(pwm[3], savestate, 3);
    }

    pxaTimrSave(tmr, savestate);
    pxaMmcSave(mmc, savestate);
    pxaRtcSave(rtc, savestate);
    pxaLcdSave(lcd, savestate);

    if (socRev == 2) {
        pxa255dspSave(dsp, savestate);
        pxa255UdcSave(udc1, savestate);
    } else {
        // no 27x for now
    }

    cpuSave(cpu, savestate);
    scheduler->Save(savestate);
    deviceSave(dev, savestate);
    vsdSave(vSD, savestate);

    auto *chunk = savestate.GetChunk(ChunkType::pxaSoc, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    strncpy(cardId, sdCardGetId(), sizeof(cardId));
    cardId[sizeof(cardId) - 1] = '\0';

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper, SAVESTATE_VERSION);
}

template <typename T>
void SoC::DoSaveLoad(T &chunkHelper, uint32_t version) {
    chunkHelper.Do(typename T::BoolPack() << cardInserted << enablePcmOutput << sleeping)
        .DoBuffer(cardId, sizeof(cardId));

    uint64_t cyclesTotal = 0;
    if (version == 1) chunkHelper.Do64(cyclesTotal);
}
