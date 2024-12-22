#include "proxyClientWs.h"

#include <curl/curl.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <sstream>

#include "Defer.h"
#include "EmCommon.h"
#include "json/ArduinoJson.h"

#if LIBCURL_VERSION_NUM < 0x080000
namespace {
    class ProxyClientStub : public ProxyClient {
       public:
        ProxyClientStub() {
            cerr << "WARNING: cloudpilot-emu was built with libcurl "
                 << ((LIBCURL_VERSION_NUM >> 16) & 0xff) << "."
                 << ((LIBCURL_VERSION_NUM >> 8) & 0xff) << "." << (LIBCURL_VERSION_NUM & 0xff)
                 << " which is too old to support the network proxy." << endl;
            cerr << "At least version 8.0.0 is required for proxy support. Network proxy support "
                    "is disabled."
                 << endl;
        }

        bool Connect() override { return false; }
        void Disconnect() override {}

        bool Send(const uint8* message, size_t size) override { return false; }

        std::pair<uint8*, size_t> Receive() override { return {nullptr, 0}; }
    };
}  // namespace

ProxyClient* proxyClientWs::Create(const string& host, const long port, const string& path) {
    return new ProxyClientStub();
}
#else

namespace {
    constexpr long HANDSHAKE_TIMEOUT_MSEC = 5000;
    constexpr long CONNECT_TIMEOUT_MSEC = 5000;
    constexpr int SERVER_VERSION = 3;

    bool curlInitialized = false;
    bool curlValid = false;

    struct CurlResponseBuffer {
       public:
        CurlResponseBuffer(size_t capacity)
            : capacity(capacity), buffer(make_unique<uint8[]>(capacity)) {};

        size_t curlWrite(char* ptr, size_t nmemb) {
            const size_t count = min(nmemb, capacity - size);

            memcpy(buffer.get() + size, ptr, count);
            size += count;

            return count;
        }

        static size_t curlWriteCb(char* ptr, size_t size, size_t nmemb, void* userdata) {
            return reinterpret_cast<CurlResponseBuffer*>(userdata)->curlWrite(ptr, nmemb);
        }

        size_t capacity;
        unique_ptr<uint8[]> buffer;

        size_t size{0};
    };

    size_t curlEmptyReadCb(char* buffer, size_t size, size_t nitems, void* userdata) { return 0; }

    bool curlSupportsProtocol(const char* protocol) {
        auto versionInfo = curl_version_info(CURLVERSION_NOW);
        const char* const* it = versionInfo->protocols;

        while (*it) {
            if (strcmp(*(it++), protocol) == 0) return true;
        }

        return false;
    }

    class ProxyClientWs : public ProxyClient {
       public:
        ProxyClientWs(const string& host, const long port, const string& path)
            : host(host), port(port), path(path) {
            if (!curlInitialized) {
                curlInitialized = true;
                curlValid = curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK;

                if (!curlValid) {
                    cerr << "failed to initialize curl!" << endl;
                    return;
                } else {
                    atexit(curl_global_cleanup);
                }

                if (LIBCURL_VERSION_NUM < 0x080000) {
                    cerr << "libcurl is too old; you need at least version 8.0.0 for the proxy to "
                            "be enabled"
                         << endl;

                    curlValid = false;
                }

                if (!curlSupportsProtocol("ws")) {
                    cerr << "libcurl compiled without websocket support; proxy is not available"
                         << endl;
                    curlValid = false;
                }
            }
        }

        bool Connect() override {
            if (!curlValid) return false;

            string token;
            if (!Handshake(token)) {
                return false;
            }

            cout << "got token " << token << endl;

            CURL* handle = ConnectWebsocket(token);
            if (!handle) return false;

            cout << "connected to websocket" << endl;
            curl_easy_cleanup(handle);

            return false;
        }

        void Disconnect() override {}

        bool Send(const uint8* message, size_t size) override { return false; }

        std::pair<uint8*, size_t> Receive() override { return {nullptr, 0}; }

       private:
        bool Handshake(string& token) {
            CURL* handle = curl_easy_init();
            if (!handle) {
                cerr << "failed to init handshake" << endl;
                return false;
            }

            Defer closeHandle([=]() { curl_easy_cleanup(handle); });

            stringstream s;
            s << "http://" << host << ":" << port << path << "/network-proxy/handshake";

            if (curl_easy_setopt(handle, CURLOPT_URL, s.str().c_str()) != CURLE_OK) {
                cerr << "bad proxy URL" << endl;
                return false;
            }

            curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, HANDSHAKE_TIMEOUT_MSEC);
            curl_easy_setopt(handle, CURLOPT_POST, 1l);
            curl_easy_setopt(handle, CURLOPT_READFUNCTION, curlEmptyReadCb);

            CurlResponseBuffer response(2048);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, CurlResponseBuffer::curlWriteCb);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

            if (curl_easy_perform(handle) != CURLE_OK) {
                cerr << "failed to handshake with proxy in " << endl;
                return false;
            }

            long status = 666;
            curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &status);
            if (status != 200) {
                cerr << "handshake failed with HTTP " << status << endl;
                return false;
            }

            ArduinoJson::StaticJsonDocument<2048> json;
            if (ArduinoJson::deserializeJson(
                    json, reinterpret_cast<const char*>(response.buffer.get()), response.size) !=
                    ArduinoJson::DeserializationError::Ok ||
                !json.is<ArduinoJson::JsonObject>()) {
                cout << "invalid handshake response from proxy" << endl;
                return false;
            }

            if (!json["version"].is<int>() || json["version"].as<int>() < SERVER_VERSION ||
                !json["compatVersion"].is<int>() ||
                json["compatVersion"].as<int>() > SERVER_VERSION) {
                cerr << "handshake failed: version mismatch" << endl;
                return false;
            }

            if (!json["token"].is<const char*>()) {
                cerr << "no token in handshake" << endl;
                return false;
            }
            token = json["token"].as<const char*>();

            return true;
        }

       private:
        CURL* ConnectWebsocket(const string& token) {
            bool success = false;

            CURL* handle = curl_easy_init();
            if (!handle) {
                cerr << "failed to init connect" << endl;
                return nullptr;
            }

            Defer closeHandle([=]() {
                if (!success) curl_easy_cleanup(handle);
            });

            char* encodedToken = curl_easy_escape(handle, token.c_str(), token.size());
            if (!encodedToken) {
                cerr << "failed to encode token" << endl;
            }

            stringstream s;
            s << "ws://" << host << ":" << port << path
              << "/network-proxy/connect?token=" << encodedToken;

            curl_free(encodedToken);

            if (curl_easy_setopt(handle, CURLOPT_URL, s.str().c_str()) != CURLE_OK) {
                cerr << "bad connect URL" << endl;
                return nullptr;
            }

            if (curl_easy_setopt(handle, CURLOPT_CONNECT_ONLY, 2l) != CURLE_OK) {
                cerr << "failed to configure websocket" << endl;
                return nullptr;
            }

            curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, CONNECT_TIMEOUT_MSEC);

            if (curl_easy_perform(handle) != CURLE_OK) {
                cerr << "failed to connect to proxy" << endl;
                return nullptr;
            }

            success = true;
            return handle;
        }

       private:
        const string host;
        const long port;
        const string path;
    };

}  // namespace

ProxyClient* proxyClientWs::Create(const string& host, const long port, const string& path) {
    return new ProxyClientWs(host, port, path);
}
#endif