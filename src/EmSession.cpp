#include "EmSession.h"

#include "EmCPU.h"
#include "EmMemory.h"
#include "EmPalmOS.h"
#include "EmPatchMgr.h"
#include "EmSystemState.h"
#include "Miscellaneous.h"
#include "ROMStubs.h"

namespace {
    EmSession _gSession;
}

constexpr int MIN_CYCLES_BETWEEN_EVENTS = 10000;

EmSession* gSession = &_gSession;

Bool EmSession::Initialize(EmDevice* device, const uint8* romImage, size_t romLength) {
    this->device.reset(device);

    cpu.reset(device->CreateCPU(this));

    if (!Memory::Initialize(romImage, romLength, device->MinRAMSize())) return false;
    EmPalmOS::Initialize();

    Reset(EmResetType::kResetSoft);

    gSystemState.Reset();

    return true;
}

void EmSession::Reset(EmResetType resetType) {
    EmAssert(cpu);

    Memory::Reset((resetType & kResetTypeMask) != kResetSys);
    cpu->Reset((resetType & kResetTypeMask) != kResetSys);
    EmPalmOS::Reset();

    bankResetScheduled = false;
    resetScheduled = false;

    waitingForSyscall = false;
    syscallDispatched = false;

    additionalCycles = 0;
    systemCycles = 0;

    penEventQueue.Clear();
    keyboardEventQueue.Clear();
    penEventQueueIncoming.Clear();
    keyboardEventQueueIncoming.Clear();
    lastEventPromotedAt = 0;
}

Bool EmSession::IsNested() {
    EmAssert(nestLevel >= 0);
    return nestLevel > 0;
}

void EmSession::ReleaseBootKeys() {}

Bool EmSession::ExecuteSpecial(Bool checkForResetOnly) {
    if (resetScheduled) {
        resetScheduled = false;
        bankResetScheduled = false;

        this->Reset(resetType);
    }

    if (bankResetScheduled) {
        bankResetScheduled = false;

        Memory::ResetBankHandlers();
    }

    if (checkForResetOnly) return false;

    return false;
}

Bool EmSession::CheckForBreak() { return suspendCpuSubroutineReturn || syscallDispatched; }

void EmSession::ScheduleResetBanks() {
    bankResetScheduled = true;

    EmASSERT(cpu);
    cpu->CheckAfterCycle();
}

void EmSession::ScheduleReset(EmResetType resetType) {
    resetScheduled = true;
    this->resetType = resetType;

    EmASSERT(cpu);
    cpu->CheckAfterCycle();
}

void EmSession::ScheduleSubroutineReturn() {
    suspendCpuSubroutineReturn = true;

    EmAssert(cpu);
    cpu->CheckAfterCycle();
}

EmDevice& EmSession::GetDevice() { return *device; }

uint32 EmSession::RunEmulation(uint32 maxCycles) {
    EmAssert(cpu);

    PumpEvents();

    uint32 cycles = cpu->Execute(maxCycles);
    systemCycles += cycles;

    cycles += additionalCycles;
    additionalCycles = 0;

    return cycles;
}

void EmSession::ExecuteSubroutine() {
    EmAssert(cpu);
    EmAssert(nestLevel >= 0);

    EmValueChanger<bool> clearSuspendCpuSubroutineReturn(suspendCpuSubroutineReturn, false);
    EmValueChanger<int> increaseNestLevel(nestLevel, nestLevel + 1);

    while (!suspendCpuSubroutineReturn) {
        cpu->Execute(0);
    }
}

void EmSession::RunToSyscall() {
    EmValueChanger<bool> startRunToSyscall(waitingForSyscall, true);
    EmValueChanger<bool> resetSyscallDispatched(syscallDispatched, false);

    EmAssert(gCPU);

    while (!syscallDispatched) {
        uint32 cycles = cpu->Execute(0);

        systemCycles += cycles;
        additionalCycles += cycles;
    }
}

void EmSession::NotifySyscallDispatched() {
    syscallDispatched = true;

    EmAssert(gCPU);
    cpu->CheckAfterCycle();
}

bool EmSession::WaitingForSyscall() { return waitingForSyscall; }

void EmSession::HandleInstructionBreak() { EmPatchMgr::HandleInstructionBreak(); }

void EmSession::QueuePenEvent(PenEvent evt) {
    if (penEventQueueIncoming.GetFree() == 0) penEventQueueIncoming.Get();

    penEventQueueIncoming.Put(evt);
}

bool EmSession::HasPenEvent() { return penEventQueue.GetUsed() > 0; }

PenEvent EmSession::NextPenEvent() { return HasPenEvent() ? penEventQueue.Get() : PenEvent(); }

void EmSession::QueueKeyboardEvent(KeyboardEvent evt) {
    if (keyboardEventQueueIncoming.GetFree() == 0) keyboardEventQueueIncoming.Get();

    keyboardEventQueueIncoming.Put(evt);
}

bool EmSession::HasKeyboardEvent() { return keyboardEventQueue.GetUsed() > 0; }

KeyboardEvent EmSession::NextKeyboardEvent() {
    return HasKeyboardEvent() ? keyboardEventQueue.Get() : KeyboardEvent('.');
}

void EmSession::Wakeup() {
    RunToSyscall();
    EvtWakeup();
}

void EmSession::PumpEvents() {
    if (systemCycles - lastEventPromotedAt < MIN_CYCLES_BETWEEN_EVENTS ||
        !gSystemState.IsUIInitialized())
        return;

    if (PromoteKeyboardEvent() || PromotePenEvent()) lastEventPromotedAt = systemCycles;
}

bool EmSession::PromoteKeyboardEvent() {
    if (keyboardEventQueueIncoming.GetUsed() == 0) return false;

    if (keyboardEventQueue.GetFree() == 0) keyboardEventQueue.Get();
    keyboardEventQueue.Put(keyboardEventQueueIncoming.Get());

    Wakeup();

    return true;
}

bool EmSession::PromotePenEvent() {
    if (penEventQueueIncoming.GetUsed() == 0) return false;

    if (penEventQueue.GetFree() == 0) penEventQueue.Get();
    penEventQueue.Put(penEventQueueIncoming.Get());

    Wakeup();

    return true;
}
