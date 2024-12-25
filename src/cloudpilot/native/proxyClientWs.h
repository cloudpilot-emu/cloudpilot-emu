#ifndef PROXY_CLIENT_WS
#define PROXY_CLIENT_WS

#include "ProxyClient.h"

namespace proxyClientWs {
    ProxyClient* Create(const string& host, long port, const string& path, bool tls, bool insecure);
}

#endif  // PROXY_CLIENT_WS