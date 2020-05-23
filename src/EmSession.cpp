#include "EmSession.h"

#include "EmCPU.h"

namespace {
    EmSession _gSession;
}

EmSession* gSession = &_gSession;

Bool EmSession::IsNested() { return false; }

Bool EmSession::ExecuteSpecial(Bool checkForResetOnly) { return false; }

Bool EmSession::CheckForBreak() { return false; }

void EmSession::ScheduleResetBanks() {
    bankResetScheduled = true;

    EmASSERT(cpuInstance);
    cpuInstance->CheckAfterCycle();
}

void EmSession::ScheduleReset(EmResetType resetType) {
    resetScheduled = true;
    this->resetType = resetType;

    EmASSERT(cpuInstance);
    cpuInstance->CheckAfterCycle();
}

EmDevice& EmSession::GetDevice() { return *device; }
