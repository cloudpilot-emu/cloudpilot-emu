#ifndef _SUSPEND_CONTEXT_NETWORK_RPC_H_
#define _SUSPEND_CONTEXT_NETWORK_RPC_H_

#include "EmCommon.h"
#include "SuspendContext.h"

class SuspendContextNetworkRpc : public SuspendContext {
   public:
    SuspendContextNetworkRpc() = default;

    Kind GetKind() const override;

    void Cancel() override;

    void ProcessMessage(void* message, size_t size);
};

#endif  // _SUSPEND_CONTEXT_NETWORK_RPC_H_
