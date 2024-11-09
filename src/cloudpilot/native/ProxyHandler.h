#ifndef _PROXY_HANDLER_H_
#define _PROXY_HANDLER_H_

#include <optional>

#include "EmCommon.h"
#include "EmEvent.h"
#include "ProxyClient.h"
#include "SuspendContext.h"

class ProxyHandler {
   public:
    ProxyHandler(ProxyClient& client);

    void Initialize();

    void Teardown();

    void HandleSuspend();

   private:
    void HandleConnect(SuspendContext& context);
    void HandleRpc(SuspendContext& context);

    void OnDisconnectHandler();

   private:
    ProxyClient& client;
    optional<EmEvent<const string&>::HandleT> onDisconnectHandle;

   private:
    ProxyHandler(const ProxyHandler&) = delete;
    ProxyHandler(ProxyHandler&&) = delete;
    ProxyHandler& operator=(const ProxyHandler&) = delete;
    ProxyHandler& operator=(ProxyHandler&&) = delete;
};

#endif  // _PROXY_HANDLER_H_
