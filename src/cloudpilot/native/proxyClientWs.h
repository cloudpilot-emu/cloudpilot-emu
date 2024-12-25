#ifndef PROXY_CLIENT_WS
#define PROXY_CLIENT_WS

#include <optional>

#include "ProxyClient.h"

namespace proxyClientWs {
    struct Config {
        string host;
        long port{80};
        string path;

        optional<string> username;
        optional<string> password;

        bool tls{false};
        optional<string> ca;
        bool insecure{false};
    };

    ProxyClient* Create(const Config& options);
}  // namespace proxyClientWs

#endif  // PROXY_CLIENT_WS