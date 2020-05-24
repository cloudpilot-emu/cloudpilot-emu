#include "EmSession.h"

#include "EmCPU.h"
#include "EmMemory.h"

namespace {
    EmSession _gSession;
}

EmSession* gSession = &_gSession;

Bool EmSession::Initialize(EmDevice* device, const uint8* romImage, size_t romLength) {
    this->device.reset(device);

    cpu.reset(device->CreateCPU(this));

    if (!Memory::Initialize(romImage, romLength, device->MinRAMSize())) return false;

    Reset(EmResetType::kResetSoft);

    return true;
}

Bool EmSession::IsNested() { return false; }

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

Bool EmSession::CheckForBreak() { return false; }

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

EmDevice& EmSession::GetDevice() { return *device; }

void EmSession::Reset(EmResetType resetType) {
    EmAssert(cpu);

    Memory::Reset((resetType & kResetTypeMask) != kResetSys);
    cpu->Reset((resetType & kResetTypeMask) != kResetSys);

    bankResetScheduled = false;
    resetScheduled = false;
}
