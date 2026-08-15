// clang-format off
#include "soc_generic_impl.h" // IWYU pragma: keep
// clang-format on

#include "soc_pv.h"

#include <cstdint>
#include <cstdio>

#include "CPU.h"
#include "RAM.h"
#include "ROM.h"
#include "device_type5.h"
#include "memory_buffer.h"
#include "patch68k.h"
#include "patch_dispatch.h"
#include "patches.h"
#include "peephole.h"
#include "pv_hypercall_interface.h"
#include "pv_ic.h"
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

namespace {
    uint32_t sanitizeRamSize(uint32_t ramSize) {
        uint32_t sanitizedSize = (ramSize >> 20) << 20;

        if (ramSize & ((1 << 20) - 1)) sanitizedSize += (1 << 20);

        return sanitizedSize;
    }

    void uartDebugWriteF(uint8_t chr, void *ctx) { fprintf(stderr, "%c", chr); }
}  // namespace

SocPV::SocPV(uint32_t ramSize, void *romData, const uint32_t romSize, int gdbPort) {
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

    pvUartSetWriteF(uartDebug, uartDebugWriteF, nullptr);

    powerOnState->Save(*this);
    SdEject();
}

uint32_t *SocPV::GetPendingFrame() { return nullptr; }

void SocPV::ResetPendingFrame() {}

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

        default:
            ERR("invalid client type\n");
    }
}

void SocPV::OnSetFramebufferDirty() {}

bool SocPV::OnSleep() { return false; }

void SocPV::OnWakeup() {}

void SocPV::OnSetAudioQueue(struct AudioQueue *audioQueue) {}

void SocPV::OnSetPcmOutputEnabled() {}

void SocPV::OnSetPcmSuspended() {}

void SocPV::OnSdInsert() {}

void SocPV::OnSdEject() {}

void SocPV::OnTouch(int x, int y) {}

void SocPV::OnEngageKey(KeyId key, bool down) {}

void SocPV::OnReset() {}

void SocPV::OnLoad(SavestateLoader<ChunkType> &loader) {
    pvIcLoad(ic, loader);
    pvTimerLoad(timer, loader);
}

template <typename T>
void SocPV::OnSave(T &savestate) {
    pvIcSave(ic, savestate);
    pvTimerSave(timer, savestate);
}

void SocPV::AllocateBuffers() {
    size_t memoryBufferSize = ramSize + MEMORY_BUFFER_GRANULARITY;

    bool success = memoryBufferAllocate(&bufferMemory, memoryBufferSize);
    success = success && memoryBufferGetSubBuffer(&bufferMemory, &bufferTinyRam, ramSize,
                                                  MEMORY_BUFFER_GRANULARITY);

    if (!success) ERR("failed to allocate memory buffer");
}

void SocPV::SetupScheduler() {
    // Timer: 1kHz;
    scheduler->ScheduleTask(SCHEDULER_TASK_TIMER, 1_sec / 1000ull, 1);

    // Pump event queues: 30 Hz
    scheduler->ScheduleTask(SCHEDULER_TASK_AUX_2, 1_sec / 30ull, 1);
}

template void SocGeneric<SocPV>::Save<Savestate<ChunkType>>(Savestate<ChunkType> &savestate);
template void SocGeneric<SocPV>::Save<SavestateProbe<ChunkType>>(
    SavestateProbe<ChunkType> &savestate);
