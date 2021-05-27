#include "Feature.h"

#include "SuspendContext.h"
#include "SuspendManager.h"

bool Feature::clipboardIntegration{false};
bool Feature::networkRedirection{false};

void Feature::SetClipboardIntegration(bool toggle) {
    clipboardIntegration = toggle;

    if (!toggle && SuspendManager::IsSuspended() &&
        (SuspendManager::GetContext().GetKind() == SuspendContext::Kind::clipboardCopy &&
         SuspendManager::GetContext().GetKind() == SuspendContext::Kind::clipboardPaste)) {
        SuspendManager::GetContext().Cancel();
    }
}

bool Feature::GetClipboardIntegration() { return clipboardIntegration; }

void Feature::SetNetworkRedirection(bool toggle) { networkRedirection = toggle; }

bool Feature::GetNetworkRedirection() { return networkRedirection; }
