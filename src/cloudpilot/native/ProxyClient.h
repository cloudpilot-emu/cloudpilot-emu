#ifndef _PROXY_CLIENT_H_
#define _PROXY_CLIENT_H_

#include "EmCommon.h"

class ProxyClient {
   public:
    virtual ~ProxyClient() = default;

    virtual bool Connect() = 0;
    virtual void Disconnect() = 0;

    virtual bool Send(const uint8* message, size_t size) = 0;

    virtual std::pair<uint8*, size_t> Receive() = 0;

   protected:
    ProxyClient() = default;

   private:
    ProxyClient(const ProxyClient&) = delete;
    ProxyClient(ProxyClient&&) = delete;
    ProxyClient& operator=(const ProxyClient&) = delete;
    ProxyClient& operator=(ProxyClient&&) = delete;
};

#endif  // _WEBSOCKET_CLIENT_H_
