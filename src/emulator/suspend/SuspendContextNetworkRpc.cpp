
#include "SuspendContextNetworkRpc.h"

SuspendContextNetworkRpc::SuspendContextNetworkRpc(uint8* request, size_t requestSize,
                                                   successCallbackT onSuccess, failCallbackT onFail)
    : onSuccess(onSuccess), onFail(onFail), request(request), requestSize(requestSize) {}

SuspendContext::Kind SuspendContextNetworkRpc::GetKind() const { return Kind::networkRpc; }

void SuspendContextNetworkRpc::Cancel() {
    onFail();

    ResumeExecution();
}

std::pair<uint8*, size_t> SuspendContextNetworkRpc::GetRequest() {
    return make_pair(request.get(), requestSize);
}

void SuspendContextNetworkRpc::ReceiveResponse(uint8* buffer, size_t size) {
    onSuccess(buffer, size);

    ResumeExecution();
}
