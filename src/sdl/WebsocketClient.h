#ifndef _WEBSOCKET_CLIENT_H_
#define _WEBSOCKET_CLIENT_H_

#include "EmCommon.h"

class WebsocketClientImpl;

class WebsocketClient {
   public:
    static WebsocketClient* Create(const string& host, const string& port);

    virtual ~WebsocketClient() = default;

    virtual bool Connect() = 0;
    virtual void Disconnect() = 0;

    virtual bool IsRunning() const = 0;
    virtual bool Send(const uint8* message, size_t size) = 0;

    virtual std::pair<uint8*, size_t> Receive() = 0;

   protected:
    WebsocketClient() = default;

   private:
    WebsocketClient(const WebsocketClient&) = delete;
    WebsocketClient(WebsocketClient&&) = delete;
    WebsocketClient& operator=(const WebsocketClient&) = delete;
    WebsocketClient& operator=(WebsocketClient&&) = delete;
};

#endif  // _WEBSOCKET_CLIENT_H_
