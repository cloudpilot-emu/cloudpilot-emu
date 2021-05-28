#ifndef _WEBSOCKET_CLIENT_H_
#define _WEBSOCKET_CLIENT_H_

#include "EmCommon.h"

class WebsocketClientImpl;

class WebsocketClient {
   public:
    WebsocketClient(const string& host, const string& port);
    ~WebsocketClient();

    void Start();
    void Stop();

    bool IsRunning() const;
    void Join();
    void Send(const string& message);

   private:
    WebsocketClientImpl* impl{nullptr};

   private:
    WebsocketClient(const WebsocketClient&) = delete;
    WebsocketClient(WebsocketClient&&) = delete;
    WebsocketClient& operator=(const WebsocketClient&) = delete;
    WebsocketClient& operator=(WebsocketClient&&) = delete;
};

#endif  // _WEBSOCKET_CLIENT_H_
