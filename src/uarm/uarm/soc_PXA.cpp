// clang-format off
#include "soc_generic_impl.h" // IWYU pragma: keep
// clang-format on

#include "soc_PXA.h"

#include <cstddef>
#include <cstdlib>

#include "CPU.h"
#include "RAM.h"
#include "ROM.h"
#include "cputil.h"
#include "device.h"
#include "device_configuration.h"
#include "keys.h"
#include "pace_patch.h"
#include "patch68k.h"
#include "patch_dispatch.h"
#include "patches.h"
#include "peephole.h"
#include "pxa255_DSP.h"
#include "pxa255_UDC.h"
#include "pxa270_IMC.h"
#include "pxa270_KPC.h"
#include "pxa270_UDC.h"
#include "pxa270_WMMX.h"
#include "pxa_AC97.h"
#include "pxa_DMA.h"
#include "pxa_GPIO.h"
#include "pxa_I2C.h"
#include "pxa_I2S.h"
#include "pxa_IC.h"
#include "pxa_LCD.h"
#include "pxa_MMC.h"
#include "pxa_MemCtrl.h"
#include "pxa_PWM.h"
#include "pxa_PwrClk.h"
#include "pxa_RTC.h"
#include "pxa_SSP.h"
#include "pxa_TIMR.h"
#include "pxa_UART.h"
#include "reschedule.h"
#include "rom_info5.h"
#include "syscall_dispatch.h"

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

#define PCM_HZ_ENABLED 44300
#define PCM_HZ_DISABLED (44100 / 3)

SocPXA::SocPXA(enum DeviceType5 deviceType, uint32_t ramSize, void *romData, const uint32_t romSize,
               uint8_t *nandContent, size_t nandSize, int gdbPort, uint_fast8_t socRev) {
    RomInfo5 romInfo(romData, romSize);

    struct Reschedule rescheduleCb = {.rescheduleCb = RescheduleCB, .ctx = this};
    struct GetEmuTime getEmuTime = {.getTimeF = GetTimeCB, .userdata = this};

    this->socRev = socRev;

    if (ramSize == 0) ramSize = 16 << 20;
    if (!deviceConfigurationSupportsRamSize(deviceType, ramSize))
        ERR("unsupported RAM size %u\n", ramSize);

    this->ramSize = ramSize;
    this->ramBase = ramBase;

    AllocateBuffers();
    SetupScheduler();

    cpu = cpuInit(ROM_BASE, mem, ARM_MEMORY_SYSTEM_MMU, true /* xscale */, false /* omap */,
                  gdbPort, socRev ? ((socRev == 1) ? CPUID_PXA260 : CPUID_PXA270) : CPUID_PXA255,
                  0x0B16A16AUL, patchDispatch, pacePatch, systemState);

    patchDispatchSetCpu(patchDispatch, cpu);

    syscallDispatch = initSyscallDispatch(this);
    patchContext = registerPatches(patchDispatch, syscallDispatch, cpu, systemState);

    ram = ramInit(mem, this, RAM_BASE, ramSize, &bufferMemory, true);
    rom = romInit(mem, ROM_BASE, romData, romSize);

    void *peepholeBuffer = romGetPeepholeBuffer(rom);

    pacePatchInit(pacePatch, ROM_BASE, peepholeBuffer, romSize);
    peepholeOptimize((uint32_t *)peepholeBuffer, romSize);
    patch68kInit(romInfo.NeedsNandPatch() ? PATCH_68K_NVFS : 0);

    vSD = vsdInit(sdCardRead, sdCardWrite, 0);

    switch (deviceGetRamTerminationStyle()) {
        case RamTerminationMirror:

            // ram mirror for ram probe code
            ramMirror = ramInit(mem, this, RAM_BASE + ramSize, ramSize, &bufferMemory, false);
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

    ic = pxaIcInit(cpu, mem, this, socRev);
    dma = pxaDmaInit(mem, rescheduleCb, ic);

    if (socRev == 0 || socRev == 1) {
        dsp = pxa255dspInit(cpu);
    } else if (socRev == 2) {
        wmmx = pxa270wmmxInit(cpu);
        imc = pxaImcInit(mem);
        kpc = pxaKpcInit(mem, ic);
        sram = ramInit(mem, this, SRAM_BASE, SRAM_SIZE, &bufferSram, false);
    }

    gpio = pxaGpioInit(mem, ic, socRev);
    tmr = pxaTimrInit(mem, ic);
    rtc = pxaRtcInit(mem, ic);
    ffUart = pxaUartInit(mem, rescheduleCb, ic, PXA_FFUART_BASE, PXA_I_FFUART);
    stUart = pxaUartInit(mem, rescheduleCb, ic, PXA_STUART_BASE, PXA_I_STUART);
    btUart = pxaUartInit(mem, rescheduleCb, ic, PXA_BTUART_BASE, PXA_I_BTUART);

    if (socRev != 2) {
        hwUart = pxaUartInit(mem, rescheduleCb, ic, PXA_HWUART_BASE, PXA_I_HWUART);
    }

    pwrClk = pxaPwrClkInit(cpu, mem, this, socRev == 2);

    if (socRev == 2) {
        pwrI2c = pxaI2cInit(mem, ic, dma, PXA_PWR_I2C_BASE, PXA_I_PWR_I2C);
    }

    i2c = pxaI2cInit(mem, ic, dma, PXA_I2C_BASE, PXA_I_I2C);
    memCtrl = pxaMemCtrlrInit(mem, socRev);
    ac97 = pxaAC97Init(mem, ic, dma);

    // SSP/SSP1
    ssp[0] = pxaSspInit(mem, rescheduleCb, ic, dma, PXA_SSP1_BASE, PXA_I_SSP, DMA_CMR_SSP_RX);

    if (socRev == 0 || socRev == 1) {
        // NSSP
        ssp[1] = pxaSspInit(mem, rescheduleCb, ic, dma, PXA_NSSP_BASE, PXA_I_NSSP, DMA_CMR_NSSP_RX);
        udc1 = pxa255UdcInit(mem, ic, dma);
    }

    if (socRev == 1) {
        // ASSP
        ssp[2] = pxaSspInit(mem, rescheduleCb, ic, dma, PXA_ASSP_BASE, PXA_I_ASSP, DMA_CMR_ASSP_RX);
    }

    if (socRev == 2) {
        // SSP2
        ssp[1] = pxaSspInit(mem, rescheduleCb, ic, dma, PXA_SSP2_BASE, PXA_I_SSP2, DMA_CMR_SSP2_RX);

        // SSP3
        ssp[2] = pxaSspInit(mem, rescheduleCb, ic, dma, PXA_SSP3_BASE, PXA_I_SSP3, DMA_CMR_SSP3_RX);

        udc2 = pxa270UdcInit(mem, ic, dma);
    }

    i2s = pxaI2sInit(mem, ic, dma);

    pwm[0] = pxaPwmInit(mem, PXA_PWM0_BASE);
    pwm[1] = pxaPwmInit(mem, PXA_PWM1_BASE);

    if (socRev == 2) {
        pwm[2] = pxaPwmInit(mem, PXA_PWM2_BASE);
        pwm[3] = pxaPwmInit(mem, PXA_PWM3_BASE);
    }

    mmc = pxaMmcInit(mem, ic, dma);

    displayConfiguration =
        deviceConfigurationDisplayConfigForMode(deviceConfigurationDefaultDisplayMode(deviceType));

    lcd = pxaLcdInit(mem, this, ic, &bufferLcd, displayConfiguration.width,
                     displayConfiguration.height);

    kp = keypadInit(gpio, true);

    struct SocPeriphs sp = {};
    sp.mem = mem;
    sp.gpio = gpio;
    sp.i2c = i2c;
    sp.i2s = i2s;
    sp.ac97 = ac97;
    sp.ssp = ssp[0];
    sp.ssp2 = ssp[1];
    sp.ssp3 = ssp[2];
    if (socRev == 2) sp.kpc = kpc;
    sp.uarts[0] = ffUart;
    sp.uarts[1] = hwUart;
    sp.uarts[2] = stUart;
    sp.uarts[3] = btUart;

    dev = deviceSetup(deviceType, &sp, rescheduleCb, getEmuTime, kp, vSD, nandContent, nandSize,
                      &bufferNand);

    nand = sp.nand;

    powerOnState->Save(*this);
    SdEject();
}

uint32_t *SocPXA::GetPendingFrame() { return pxaLcdGetPendingFrame(lcd); }

void SocPXA::ResetPendingFrame() { return pxaLcdResetPendingFrame(lcd); }

DeviceType5 SocPXA::GetDeviceType() { return deviceGetType(dev); }

DisplayMode SocPXA::GetDisplayMode() {
    return deviceConfigurationDefaultDisplayMode(deviceGetType(dev));
}

void SocPXA::SuspendTimerInterrupts(bool suspendInterrupts) {
    pxaTimrSuspendInterrupts(tmr, suspendInterrupts);
}

bool SocPXA::LcdEnabled() { return pxaLcdIsEnabled(lcd); }

uint32_t SocPXA::DispatchTicks(uint32_t clientType, uint32_t batchedTicks) {
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
            pxaI2sPeriodic(i2s);
            return 1;

        case SCHEDULER_TASK_PCM:
            if (!syscallDispatchInProgress(syscallDispatch)) devicePcmPeriodic(dev);
            return (pcmSuspended && pcmEnabled) ? 0 : 1;

        case SCHEDULER_TASK_AUX_1:
            CycleBatch0();
            return Batch0Required() ? 1 : 0;

        case SCHEDULER_TASK_AUX_2:
            PumpEventQueues();
            return 1;

        default:
            ERR("invalid client type\n");
    }
}

void SocPXA::OnSetFramebufferDirty() { pxaLcdSetFramebufferDirty(lcd); }

bool SocPXA::OnSleep() {
    if (cpuHasPendingInterrupt(cpu)) return false;

    scheduler->RescheduleTaskAtLeast(SCHEDULER_TASK_TIMER, pxaTimrTicksToNextInterrupt(tmr));
    cpuSetSlowPath(cpu, SLOW_PATH_REASON_RESCHEDULE);

    return true;
}

void SocPXA::OnWakeup() {
    scheduler->RescheduleTaskAtLeast(SCHEDULER_TASK_TIMER, 1);
    cpuSetSlowPath(cpu, SLOW_PATH_REASON_RESCHEDULE);
}

void SocPXA::OnSetAudioQueue(struct AudioQueue *audioQueue) {
    deviceSetAudioQueue(dev, audioQueue);
}

void SocPXA::OnSetPcmOutputEnabled() { SchedulePcmTask(); }

void SocPXA::OnSetPcmSuspended() {
    if (!pcmEnabled) return;

    scheduler->RescheduleTask(SCHEDULER_TASK_PCM, pcmSuspended ? 0 : 1);
    if (!pcmSuspended) cpuSetSlowPath(cpu, SLOW_PATH_REASON_RESCHEDULE);
}

void SocPXA::OnSdInsert() {
    vsdReset(vSD, sdCardSectorCount());
    pxaMmcInsert(mmc, vSD);
    deviceSetSdCardInserted(dev, true);
}

void SocPXA::OnSdEject() {
    vsdReset(vSD, 0);
    pxaMmcInsert(mmc, nullptr);
    deviceSetSdCardInserted(dev, false);
}

void SocPXA::OnTouch(int x, int y) { deviceTouch(dev, x, y); }

void SocPXA::OnEngageKey(KeyId key, bool down) {
    deviceKey(dev, key, down);
    keypadKeyEvt(kp, key, down);
}

void SocPXA::OnReset() {
    pxaLcdResetPaletteBuffer(lcd);
    nandResetPageBuffer(nand);
}

void SocPXA::OnLoad(SavestateLoader<ChunkType> &loader) {
    vsdLoad(vSD, loader);

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

    if (socRev < 2) {
        pxa255dspLoad(dsp, loader);
        pxa255UdcLoad(udc1, loader);
    } else {
        // no 27x for now
    }

    deviceLoad(dev, loader);

    SchedulePcmTask();
    keypadReset(kp);
    SetFramebufferDirty();
}

template <typename T>
void SocPXA::OnSave(T &savestate) {
    vsdSave(vSD, savestate);

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

    if (socRev < 2) {
        pxa255dspSave(dsp, savestate);
        pxa255UdcSave(udc1, savestate);
    } else {
        // no 27x for now
    }

    deviceSave(dev, savestate);
}

void SocPXA::AllocateBuffers() {
    size_t memoryBufferSize = ramSize + 2 * MEMORY_BUFFER_GRANULARITY;
    if (socRev == 2) memoryBufferSize += SRAM_SIZE;

    bool success = memoryBufferAllocate(&bufferMemory, memoryBufferSize);

    size_t offset = ramSize;

    success = success && memoryBufferGetSubBuffer(&bufferMemory, &bufferNand, offset,
                                                  MEMORY_BUFFER_GRANULARITY);
    offset += MEMORY_BUFFER_GRANULARITY;

    success = success && memoryBufferGetSubBuffer(&bufferMemory, &bufferLcd, offset,
                                                  MEMORY_BUFFER_GRANULARITY);
    offset += MEMORY_BUFFER_GRANULARITY;

    if (socRev == 2)
        success =
            success && memoryBufferGetSubBuffer(&bufferMemory, &bufferSram, offset, SRAM_SIZE);

    if (!success) ERR("failed to allocate memory buffers");
}

void SocPXA::SetupScheduler() {
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

void SocPXA::SchedulePcmTask() {
    scheduler->ScheduleTask(SCHEDULER_TASK_PCM,
                            1_sec / (pcmEnabled ? PCM_HZ_ENABLED : PCM_HZ_DISABLED),
                            pcmSuspended ? 0 : 1);
}

void SocPXA::CycleBatch0() {
    pxaDmaPeriodic(dma);
    pxaUartProcess(ffUart);
    if (hwUart) pxaUartProcess(hwUart);
    pxaUartProcess(stUart);
    pxaUartProcess(btUart);
    for (int i = 0; i < 3; i++) {
        if (ssp[i]) pxaSspPeriodic(ssp[i]);
    }
    devicePeriodic(dev, DEVICE_PERIODIC_TIER0);
}

bool SocPXA::Batch0Required() {
    if (!dev) return true;

    if (pxaDmaTaskRequired(dma)) return true;

    for (int i = 0; i < 3; i++) {
        if (ssp[i] && pxaSspTaskRequired(ssp[i])) return true;
    }

    if (deviceTaskRequired(dev, DEVICE_PERIODIC_TIER0)) return true;

    if (pxaUartTaskRequired(ffUart) || pxaUartTaskRequired(stUart) || pxaUartTaskRequired(btUart))
        return true;

    if (hwUart && pxaUartTaskRequired(hwUart)) return true;

    return false;
}

void SocPXA::RescheduleCB(void *ctx, uint32_t task) {
    auto *self = reinterpret_cast<SocPXA *>(ctx);

    switch (task) {
        case RESCHEDULE_TASK_DEVICE_TIER0:
        case RESCHEDULE_TASK_SSP:
        case RESCHEDULE_TASK_UART:
        case RESCHEDULE_TASK_DMA:
            self->scheduler->RescheduleTask(SCHEDULER_TASK_AUX_1, 1);
            cpuSetSlowPath(self->cpu, SLOW_PATH_REASON_RESCHEDULE);
            break;
    }
}

uint64_t SocPXA::GetTimeCB(void *ctx) {
    return reinterpret_cast<SocPXA *>(ctx)->scheduler->GetTime();
}

template void SocGeneric<SocPXA>::Save<Savestate<ChunkType>>(Savestate<ChunkType> &savestate);
template void SocGeneric<SocPXA>::Save<SavestateProbe<ChunkType>>(
    SavestateProbe<ChunkType> &savestate);
