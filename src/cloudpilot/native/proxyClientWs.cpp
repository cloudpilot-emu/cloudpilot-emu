#include "proxyClientWs.h"

class ProxyClientWs : public ProxyClient {
   public:
    ProxyClientWs(const string& host, const long port, const string& path) {}

    bool Connect() override { return false; }

    void Disconnect() override {}

    bool Send(const uint8* message, size_t size) override { return false; }

    std::pair<uint8*, size_t> Receive() override { return {nullptr, 0}; }
};

ProxyClient* proxyClientWs::Create(const string& host, const long port, const string& path) {
    return new ProxyClientWs(host, port, path);
}