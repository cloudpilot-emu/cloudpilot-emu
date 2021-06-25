#ifndef _SUSPEND_CONTEXT_NETWORK_CONNECT_H_
#define _SUSPEND_CONTEXT_NETWORK_CONNECT_H_

#include <functional>

#include "EmCommon.h"
#include "SuspendContext.h"

class SuspendContextNetworkConnect : public SuspendContext {
    using successCallbackT = function<void(const string&)>;
    using failCallbackT = function<void()>;

   public:
    SuspendContextNetworkConnect(successCallbackT onSuccess, failCallbackT onFail);

    Kind GetKind() const override;

    void Cancel() override;

    void Resume(const string& sessionId);

   private:
    successCallbackT onSuccess;
    failCallbackT onFail;
};

#endif  // _SUSPEND_CONTEXT_NETWORK_CONNECT_H_
