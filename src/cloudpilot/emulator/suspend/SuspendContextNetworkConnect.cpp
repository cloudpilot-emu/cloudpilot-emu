#include "SuspendContextNetworkConnect.h"

SuspendContextNetworkConnect::SuspendContextNetworkConnect(successCallbackT onSuccess,
                                                           failCallbackT onFail)
    : onSuccess(onSuccess), onFail(onFail) {}

SuspendContext::Kind SuspendContextNetworkConnect::GetKind() const { return Kind::networkConnect; }

void SuspendContextNetworkConnect::Cancel() {
    onFail();
    ResumeExecution();
}

void SuspendContextNetworkConnect::Resume(const string& sessionId) {
    onSuccess(sessionId);
    ResumeExecution();
}
