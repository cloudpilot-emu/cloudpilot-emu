#ifndef _SUSPEND_CONTEXT_NETWORK_RPC_H_
#define _SUSPEND_CONTEXT_NETWORK_RPC_H_

#include <functional>
#include <memory>

#include "EmCommon.h"
#include "SuspendContext.h"

class SuspendContextNetworkRpc : public SuspendContext {
   public:
    using successCallbackT = function<void(void* response, size_t size)>;
    using failCallbackT = function<void()>;

   public:
    SuspendContextNetworkRpc(uint8* request, size_t requestSize, successCallbackT onSuccess,
                             failCallbackT onFail);

    Kind GetKind() const override;

    void Cancel() override;

    std::pair<const uint8*, size_t> GetRequest();

    size_t GetRequestSize();
    const uint8* GetRequestData();

    void ReceiveResponse(void* response, size_t size);

   private:
    successCallbackT onSuccess;
    failCallbackT onFail;

    unique_ptr<uint8[]> request;
    size_t requestSize;
};

#endif  // _SUSPEND_CONTEXT_NETWORK_RPC_H_
