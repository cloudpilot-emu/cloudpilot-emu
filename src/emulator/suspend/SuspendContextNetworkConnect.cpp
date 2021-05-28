#include "SuspendContextNetworkConnect.h"

SuspendContextNetworkConnect::SuspendContextNetworkConnect(callbackT onSuccess, callbackT onFail)
    : onSuccess(onSuccess), onFail(onFail) {}

SuspendContext::Kind SuspendContextNetworkConnect::GetKind() const { return Kind::networkConnect; }

void SuspendContextNetworkConnect::Cancel() {
    onFail();
    ResumeExecution();
}

void SuspendContextNetworkConnect::Resume() {
    onSuccess();
    ResumeExecution();
}
