#ifndef _PROXY_HANDLER_H_
#define _PROXY_HANDLER_H_

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
    void HandleDisconnect(SuspendContext& context);
    void HandleRpc(SuspendContext& context);

   private:
    ProxyClient& client;

   private:
    ProxyHandler(const ProxyHandler&) = delete;
    ProxyHandler(ProxyHandler&&) = delete;
    ProxyHandler& operator=(const ProxyHandler&) = delete;
    ProxyHandler& operator=(ProxyHandler&&) = delete;
};

#endif  // _PROXY_HANDLER_H_
