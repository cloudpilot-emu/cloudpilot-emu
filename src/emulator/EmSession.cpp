#include "EmSession.h"

#include "EmBankSRAM.h"
#include "EmCPU.h"
#include "EmHAL.h"
#include "EmLowMem.h"
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
constexpr int MIN_CYCLES_BETWEEN_BUTTON_EVENTS = 400000;

EmSession* gSession = &_gSession;

bool EmSession::Initialize(EmDevice* device, const uint8* romImage, size_t romLength) {
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

    buttonEventQueue.Clear();
    lastButtonEventReadAt = 0;
}

bool EmSession::IsNested() const {
    EmAssert(nestLevel >= 0);
    return nestLevel > 0;
}

bool EmSession::IsExecutingSync() const { return IsNested() || waitingForSyscall; }

bool EmSession::IsPowerOn() { return !EmHAL::GetAsleep(); }

void EmSession::ReleaseBootKeys() {}

bool EmSession::ExecuteSpecial(bool checkForResetOnly) {
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

bool EmSession::CheckForBreak() const { return subroutineReturn || syscallDispatched; }

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
    subroutineReturn = true;

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

    EmValueChanger<bool> clearSubroutineReturn(subroutineReturn, false);
    EmValueChanger<int> increaseNestLevel(nestLevel, nestLevel + 1);

    while (!subroutineReturn) {
        cpu->Execute(0);
    }
}

bool EmSession::RunToSyscall() {
    EmValueChanger<bool> startRunToSyscall(waitingForSyscall, true);
    EmValueChanger<bool> resetSyscallDispatched(syscallDispatched, false);

    EmAssert(gCPU);

    while (!syscallDispatched) {
        uint32 cycles = cpu->Execute(0);

        systemCycles += cycles;
        additionalCycles += cycles;

        if (cpu->Stopped()) return false;
    }

    return true;
}

void EmSession::NotifySyscallDispatched() {
    syscallDispatched = true;

    EmAssert(gCPU);
    cpu->CheckAfterCycle();
}

bool EmSession::WaitingForSyscall() const { return waitingForSyscall; }

void EmSession::HandleInstructionBreak() { EmPatchMgr::HandleInstructionBreak(); }

void EmSession::QueuePenEvent(PenEvent evt) {
    if (!IsPowerOn()) return;

    if (penEventQueueIncoming.GetFree() == 0) penEventQueueIncoming.Get();

    penEventQueueIncoming.Put(evt);
}

bool EmSession::HasPenEvent() { return penEventQueue.GetUsed() > 0; }

PenEvent EmSession::NextPenEvent() { return HasPenEvent() ? penEventQueue.Get() : PenEvent(); }

void EmSession::QueueKeyboardEvent(KeyboardEvent evt) {
    if (!IsPowerOn()) return;

    if (keyboardEventQueueIncoming.GetFree() == 0) keyboardEventQueueIncoming.Get();

    keyboardEventQueueIncoming.Put(evt);
}

bool EmSession::HasKeyboardEvent() { return keyboardEventQueue.GetUsed() > 0; }

KeyboardEvent EmSession::NextKeyboardEvent() {
    return HasKeyboardEvent() ? keyboardEventQueue.Get() : KeyboardEvent('.');
}

bool EmSession::Wakeup() {
    bool isSafeToWakeup = RunToSyscall();

    if (isSafeToWakeup) EvtWakeup();

    return isSafeToWakeup;
}

void EmSession::PumpEvents() {
    if (systemCycles - lastEventPromotedAt < MIN_CYCLES_BETWEEN_EVENTS ||
        !gSystemState.IsUIInitialized())
        return;

    if (PromoteKeyboardEvent() || PromotePenEvent()) lastEventPromotedAt = systemCycles;
}

bool EmSession::PromoteKeyboardEvent() {
    if (keyboardEventQueueIncoming.GetUsed() == 0) return false;

    if (!Wakeup()) return false;

    if (keyboardEventQueue.GetFree() == 0) keyboardEventQueue.Get();
    keyboardEventQueue.Put(keyboardEventQueueIncoming.Get());

    return true;
}

bool EmSession::PromotePenEvent() {
    if (penEventQueueIncoming.GetUsed() == 0) return false;

    if (!Wakeup()) return false;

    if (penEventQueue.GetFree() == 0) penEventQueue.Get();
    penEventQueue.Put(penEventQueueIncoming.Get());

    return true;
}

void EmSession::QueueButtonEvent(ButtonEvent evt) {
    if (buttonEventQueue.GetFree() == 0) buttonEventQueue.Get();

    buttonEventQueue.Put(evt);
}

bool EmSession::HasButtonEvent() {
    return (systemCycles - lastButtonEventReadAt) < MIN_CYCLES_BETWEEN_BUTTON_EVENTS
               ? false
               : buttonEventQueue.GetUsed() != 0;
}

ButtonEvent EmSession::NextButtonEvent() {
    lastButtonEventReadAt = systemCycles;

    return buttonEventQueue.GetUsed() > 0
               ? buttonEventQueue.Get()
               : ButtonEvent(ButtonEvent::Button::invalid, ButtonEvent::Type::press);
}

uint32 EmSession::GetMemorySize() const { return gRAMBank_Size; }

uint8* EmSession::GetMemoryPtr() const { return gRAM_Memory; };

uint32 EmSession::GetRandomSeed() const { return EmLowMem::fgLowMem.globals.sysRandomSeed; }

uint8* EmSession::GetDirtyPagesPtr() const { return gRAM_DirtyPages; }

void EmSession::SetHotsyncUserName(string hotsyncUserName) const {
    gSystemState.SetHotsyncUserName(hotsyncUserName);

    if (gSystemState.IsUIInitialized()) {
        SetHotSyncUserName(hotsyncUserName.c_str());
    }
}
