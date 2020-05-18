#include "EmSession.h"

#include "EmCPU.h"

namespace {
    EmSession _gSession;
}

EmSession* gSession = &_gSession;

Bool EmSession::IsNested() { return false; }

Bool EmSession::ExecuteSpecial(Bool checkForResetOnly) { return false; }

Bool EmSession::CheckForBreak(void) { return false; }

void EmSession::ScheduleResetBanks(void) {
    bankResetScheduled = true;

    EmASSERT(cpuInstance);
    cpuInstance->CheckAfterCycle();
}

EmDevice& EmSession::GetDevice() { return *device; }
