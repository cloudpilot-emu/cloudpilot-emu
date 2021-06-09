#include "SuspendContextNetworkDisconnect.h"

SuspendContextNetworkDisconnect::SuspendContextNetworkDisconnect(callbackT continueCb)
    : onContinue(continueCb) {}

SuspendContext::Kind SuspendContextNetworkDisconnect::GetKind() const {
    return Kind::networkDisconnect;
}

void SuspendContextNetworkDisconnect::Cancel() {
    onContinue();
    ResumeExecution();
}

void SuspendContextNetworkDisconnect::Resume() {
    onContinue();
    ResumeExecution();
}
