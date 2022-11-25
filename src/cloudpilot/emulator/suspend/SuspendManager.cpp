#include "SuspendManager.h"

#include "SuspendContext.h"

SuspendContext* SuspendManager::context{nullptr};

SuspendContext& SuspendManager::GetContext() { return *context; }

void SuspendManager::Resume() {
    delete context;
    context = nullptr;
}

void SuspendManager::Reset() {
    if (IsSuspended()) context->Cancel();
}
