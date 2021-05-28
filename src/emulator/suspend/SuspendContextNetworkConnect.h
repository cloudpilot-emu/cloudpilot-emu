#ifndef _SUSPEND_CONTEXT_NETWORK_CONNECT_H_
#define _SUSPEND_CONTEXT_NETWORK_CONNECT_H_

#include <functional>

#include "EmCommon.h"
#include "SuspendContext.h"

class SuspendContextNetworkConnect : public SuspendContext {
    using callbackT = function<void()>;

   public:
    SuspendContextNetworkConnect(callbackT onSuccess, callbackT onFail);

    Kind GetKind() const override;

    void Cancel() override;

    void Resume();

   private:
    callbackT onSuccess;
    callbackT onFail;
};

#endif  // _SUSPEND_CONTEXT_NETWORK_CONNECT_H_
