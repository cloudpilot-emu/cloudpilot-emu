#include "Feature.h"

#include "SuspendContext.h"
#include "SuspendManager.h"

bool Feature::clipboardIntegration{false};

void Feature::SetClipboardIntegration(bool toggle) {
    clipboardIntegration = toggle;

    if (!toggle && SuspendManager::IsSuspended() &&
        (SuspendManager::GetContext().GetKind() == SuspendContext::Kind::clipboardCopy &&
         SuspendManager::GetContext().GetKind() == SuspendContext::Kind::clipboardPaste)) {
        SuspendManager::GetContext().Cancel();
    }
}

bool Feature::GetClipboardIntegration() { return clipboardIntegration; }
