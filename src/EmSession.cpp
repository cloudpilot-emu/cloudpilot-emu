#include "EmSession.h"

#include "EmCPU.h"
#include "EmMemory.h"
#include "EmPalmOS.h"
#include "EmPatchMgr.h"
#include "EmPatchState.h"
#include "Miscellaneous.h"
#include "ROMStubs.h"

namespace {
    EmSession _gSession;
}

EmSession* gSession = &_gSession;

Bool EmSession::Initialize(EmDevice* device, const uint8* romImage, size_t romLength) {
    this->device.reset(device);

    cpu.reset(device->CreateCPU(this));

    if (!Memory::Initialize(romImage, romLength, device->MinRAMSize())) return false;
    EmPalmOS::Initialize();

    Reset(EmResetType::kResetSoft);
    gPatchState = EmPatchState();

    return true;
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

Bool EmSession::CheckForBreak() { return suspendCpuSubroutineReturn; }

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

void EmSession::Reset(EmResetType resetType) {
    EmAssert(cpu);

    Memory::Reset((resetType & kResetTypeMask) != kResetSys);
    cpu->Reset((resetType & kResetTypeMask) != kResetSys);
    EmPalmOS::Reset();

    bankResetScheduled = false;
    resetScheduled = false;
}

uint32 EmSession::RunEmulation(uint32 maxCycles) {
    EmAssert(cpu);

    return cpu->Execute(maxCycles);
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

void EmSession::HandleInstructionBreak() { EmPatchMgr::HandleInstructionBreak(); }

void EmSession::QueuePenEvent(PenEvent evt) {
    penEventQueue.Put(evt);
    EvtWakeup();
}

bool EmSession::HasPenEvent() { return penEventQueue.GetUsed() > 0; }

PenEvent EmSession::NextPenEvent() { return HasPenEvent() ? penEventQueue.Get() : PenEvent(); }
