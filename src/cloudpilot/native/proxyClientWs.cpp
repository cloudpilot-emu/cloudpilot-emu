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

namespace {
    constexpr long HANDSHAKE_TIMEOUT_MSEC = 5000;
    constexpr int SERVER_VERSION = 3;

    bool curlInitialized = false;

    struct CurlResponseBuffer {
       public:
        CurlResponseBuffer(size_t capacity)
            : capacity(capacity), buffer(make_unique<uint8[]>(capacity)) {};

        static size_t curlWriteCb(char* ptr, size_t size, size_t nmemb, void* userdata) {
            auto self = reinterpret_cast<CurlResponseBuffer*>(userdata);
            const size_t count = min(nmemb, self->capacity - self->size);

            memcpy(self->buffer.get() + self->size, ptr, count);
            self->size += count;

            return count;
        }

        size_t capacity;
        unique_ptr<uint8[]> buffer;

        size_t size{0};
    };

    size_t curlEmptyReadCb(char* buffer, size_t size, size_t nitems, void* userdata) { return 0; }

    class ProxyClientWs : public ProxyClient {
       public:
        ProxyClientWs(const string& host, const long port, const string& path)
            : host(host), port(port), path(path) {
            if (!curlInitialized) {
                curlInitialized = curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK;

                if (!curlInitialized) {
                    cerr << "failed to initialize curl!" << endl;
                } else {
                    atexit(curl_global_cleanup);
                }
            }
        }

        bool Connect() override {
            if (!curlInitialized) return false;

            string token;
            if (!Handshake(token)) {
                return false;
            }

            cout << "got token " << token << endl;
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
        const string host;
        const long port;
        const string path;
    };

}  // namespace

ProxyClient* proxyClientWs::Create(const string& host, const long port, const string& path) {
    return new ProxyClientWs(host, port, path);
}