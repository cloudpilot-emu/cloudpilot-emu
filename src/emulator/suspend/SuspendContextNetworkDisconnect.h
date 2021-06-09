#ifndef _SUSPEND_CONTEXT_NETWORK_DISCONNECT_H_
#define _SUSPEND_CONTEXT_NETWORK_DISCONNECT_H_

#include <functional>

#include "EmCommon.h"
#include "SuspendContext.h"

class SuspendContextNetworkDisconnect : public SuspendContext {
    using callbackT = function<void()>;

   public:
    SuspendContextNetworkDisconnect(callbackT continueCb);

    Kind GetKind() const override;

    void Cancel() override;

    void Resume();

   private:
    callbackT onContinue;
};

#endif  // _SUSPEND_CONTEXT_NETWORK_DISCONNECT_H_
