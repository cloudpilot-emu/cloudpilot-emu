#include "SuspendManager.h"

#include "SuspentContext.h"

SuspendContext* SuspendManager::context{nullptr};

bool SuspendManager::IsSuspended() { return context != nullptr; }

SuspendContext& SuspendManager::GetContext() { return *context; }

void SuspendManager::Resume() {
    delete context;
    context = nullptr;
}

void SuspendManager::Reset() {
    if (IsSuspended()) context->Cancel();
}
