// clang-format off
#include "soc_generic_impl.h" // IWYU pragma: keep
// clang-format on

#include "soc_pv.h"

#include <cstdint>
#include <cstdio>
#include <memory>

#include "CPU.h"
#include "RAM.h"
#include "ROM.h"
#include "device_type5.h"
#include "memory_buffer.h"
#include "patch68k.h"
#include "patch_dispatch.h"
#include "patches.h"
#include "peephole.h"
#include "pv_audio.h"
#include "pv_display.h"
#include "pv_hypercall_interface.h"
#include "pv_ic.h"
#include "pv_keys.h"
#include "pv_rtc.h"
#include "pv_timer.h"
#include "pv_uart.h"
#include "scheduler.h"
#include "syscall_dispatch.h"

#define CPUID_V5T 0x4100a200

#define ROM_BASE 0x10000000
#define RAM_BASE 0x20000000

#define PV_TINYRAM_BASE 0xffff0000
#define PV_TINYRAM_SIZE 0x00000100

#define UART_DEBUG_BASE 0x30000100
#define UART_BASE 0x30000110

#define PCM_HZ 44300
#define PCM_SAMPLE_BATCH 128

using namespace std;

namespace {
    uint32_t sanitizeRamSize(uint32_t ramSize) {
        uint32_t sanitizedSize = (ramSize >> 20) << 20;

        if (ramSize & ((1 << 20) - 1)) sanitizedSize += (1 << 20);

        return sanitizedSize;
    }

    void uartDebugWriteF(uint8_t chr, void *ctx) { fprintf(stderr, "%c", chr); }
}  // namespace

SocPV::SocPV(uint32_t ramSize, void *romData, const uint32_t romSize, uint32_t displayWidth,
             uint32_t displayHeight, uint32_t displayDensity, int gdbPort)
    : displayWidth(displayWidth), displayHeight(displayHeight), displayDensity(displayDensity) {
    ramSize = sanitizeRamSize((ramSize));
    this->ramSize = ramSize;

    AllocateBuffers();
    SetupScheduler();

    cpu = cpuInit(ROM_BASE, mem, ARM_MEMORY_SYSTEM_MPU, false, false, gdbPort, CPUID_V5T,
                  0x0B16A16AUL, patchDispatch, pacePatch, systemState);

    patchDispatchSetCpu(patchDispatch, cpu);

    syscallDispatch = initSyscallDispatch(this);
    patchContext = registerPatches(patchDispatch, syscallDispatch, cpu, systemState);

    ram = ramInit(mem, this, RAM_BASE, ramSize, &bufferMemory, true);
    tinyRam = ramInit(mem, this, PV_TINYRAM_BASE, PV_TINYRAM_SIZE, &bufferTinyRam, false);
    rom = romInit(mem, ROM_BASE, romData, romSize);

    void *peepholeBuffer = romGetPeepholeBuffer(rom);

    pacePatchInit(pacePatch, ROM_BASE, peepholeBuffer, romSize);
    peepholeOptimize((uint32_t *)peepholeBuffer, romSize);
    patch68kInit(PATCH_68K_NVFS);

    ic = pvIcInit(cpu, mem);
    timer = pvTimerInit(mem, ic);
    uart = pvUartInit(mem, UART_BASE);
    uartDebug = pvUartInit(mem, UART_DEBUG_BASE);
    hypercallIface = pvHypercallInterfaceInit(cpu, ramSize);
    display = pvDisplayInit(mem, ram, &bufferClut, displayWidth, displayHeight, displayDensity);
    keys = pvKeysInit(mem, ic);
    rtc = pvRtcInit(mem, ic);
    audio = pvAudioInit(mem, ram, ic);

    pvUartSetWriteF(uartDebug, uartDebugWriteF, nullptr);

    framebuffer = make_unique<uint32_t[]>(displayWidth * displayHeight);

    powerOnState->Save(*this);
    SdEject();
}

uint32_t *SocPV::GetPendingFrame() {
    if (!framebufferDirty && !pvIsDirty(display)) return nullptr;
    if (!pvDisplayRenderFramebuffer(display, framebuffer.get())) return nullptr;

    return framebuffer.get();
}

void SocPV::ResetPendingFrame() {
    ClearFramebufferDirty();
    pvDisplayClearDirty(display);
}

enum DeviceType5 SocPV::GetDeviceType() { return deviceTypePV; }

void SocPV::SuspendTimerInterrupts(bool suspendInterrupts) {
    pvTimerSuspendInterrupts(timer, suspendInterrupts);
}

bool SocPV::LcdEnabled() { return true; }

uint32_t SocPV::DispatchTicks(uint32_t clientType, uint32_t batchedTicks) {
    switch (clientType) {
        case SCHEDULER_TASK_TIMER:
            pvTimerTick(timer);
            return 1;

        case SCHEDULER_TASK_AUX_2:
            PumpEventQueues();
            return 1;

        case SCHEDULER_TASK_RTC:
            pvRtcTick(rtc);
            return 1;

        case SCHEDULER_TASK_PCM:
            pvAudioPullSamples(audio, batchedTicks);
            return PCM_SAMPLE_BATCH;

        default:
            ERR("invalid client type\n");
    }
}

void SocPV::OnSetFramebufferDirty() {
    // NOP - we track that directly here
}

bool SocPV::OnSleep() { return false; }

void SocPV::OnWakeup() {}

void SocPV::OnSetAudioQueue(struct AudioQueue *audioQueue) { pvAudioSetQueue(audio, audioQueue); }

void SocPV::OnSetPcmOutputEnabled() {}

void SocPV::OnSetPcmSuspended() {
    scheduler->RescheduleTask(SCHEDULER_TASK_PCM, pcmSuspended ? 0 : PCM_SAMPLE_BATCH);
    if (!pcmSuspended) cpuSetSlowPath(cpu, SLOW_PATH_REASON_RESCHEDULE);
}

void SocPV::OnSdInsert() {}

void SocPV::OnSdEject() {}

void SocPV::OnTouch(int x, int y) {}

void SocPV::OnEngageKey(KeyId key, bool down) { pvKeysEngage(keys, key, down); }

void SocPV::OnReset() {}

void SocPV::OnLoad(SavestateLoader<ChunkType> &loader) {
    pvIcLoad(ic, loader);
    pvTimerLoad(timer, loader);
    pvDisplayLoad(display, loader);
    pvKeysLoad(keys, loader);
    pvAudioLoad(audio, loader);
}

template <typename T>
void SocPV::OnSave(T &savestate) {
    pvIcSave(ic, savestate);
    pvTimerSave(timer, savestate);
    pvDisplaySave(display, savestate);
    pvKeysSave(keys, savestate);
    pvAudioSave(audio, savestate);
}

void SocPV::AllocateBuffers() {
    size_t memoryBufferSize = ramSize + 2 * MEMORY_BUFFER_GRANULARITY;

    bool success = memoryBufferAllocate(&bufferMemory, memoryBufferSize);

    size_t offset = ramSize;

    success = success && memoryBufferGetSubBuffer(&bufferMemory, &bufferTinyRam, offset,
                                                  MEMORY_BUFFER_GRANULARITY);
    offset += MEMORY_BUFFER_GRANULARITY;

    success = success && memoryBufferGetSubBuffer(&bufferMemory, &bufferClut, offset,
                                                  MEMORY_BUFFER_GRANULARITY);

    if (!success) ERR("failed to allocate memory buffer");
}

void SocPV::SetupScheduler() {
    // Timer: 1kHz;
    scheduler->ScheduleTask(SCHEDULER_TASK_TIMER, 1_sec / 1000ull, 1);

    // RTC: 1Hz
    scheduler->ScheduleTask(SCHEDULER_TASK_RTC, 1_sec, 1);

    // Pump event queues: 30 Hz
    scheduler->ScheduleTask(SCHEDULER_TASK_AUX_2, 1_sec / 30ull, 1);

    scheduler->ScheduleTask(SCHEDULER_TASK_PCM, 1_sec / PCM_HZ, PCM_SAMPLE_BATCH);
}

template void SocGeneric<SocPV>::Save<Savestate<ChunkType>>(Savestate<ChunkType> &savestate);
template void SocGeneric<SocPV>::Save<SavestateProbe<ChunkType>>(
    SavestateProbe<ChunkType> &savestate);
