#ifndef PROXY_CLIENT_WS
#define PROXY_CLIENT_WS

#include "ProxyClient.h"

namespace proxyClientWs {
    ProxyClient* Create(const string& host, const long port, const string& path);
}

#endif  // PROXY_CLIENT_WS