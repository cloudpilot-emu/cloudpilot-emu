#include "SuspendContextSerialSync.h"

SuspendContextSerialSync::SuspendContextSerialSync() {}

SuspendContext::Kind SuspendContextSerialSync::GetKind() const {
    return SuspendContext::Kind::serialSync;
}

void SuspendContextSerialSync::Cancel() { ResumeExecution(); }

void SuspendContextSerialSync::Resume() { ResumeExecution(); }

bool SuspendContextSerialSync::RequiresStackAccess() { return false; }
