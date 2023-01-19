#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wdeprecated-copy-with-user-provided-copy"

#include "ProxyClient.h"

#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/websocket.hpp>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "Logging.h"
#include "json/ArduinoJson.h"

using namespace std;
using namespace std::chrono_literals;
using namespace std::placeholders;

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

namespace {
    constexpr int SERVER_VERSION = 3;

    // https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
    string UrlEncode(const string& value) {
        ostringstream escaped;
        escaped.fill('0');
        escaped << hex;

        for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
            string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << uppercase;
            escaped << '%' << setw(2) << int((unsigned char)c);
            escaped << nouppercase;
        }

        return escaped.str();
    }
}  // namespace

class ProxyClientImpl : public ProxyClient {
   public:
    ProxyClientImpl(const string& host, long port, const string& path)
        : host(host), port(to_string(port)), path(path), terminating(true) {}

    bool Connect() override {
        if (ws.is_open()) return true;

        boost::system::error_code err;

        auto const resolveResults = resolver.resolve(host, port, err);
        if (err) return false;

        string token;
        if (!Handshake(resolveResults, token)) {
            logging::printf("handshake failed");
            return false;
        }

        net::connect(ws.next_layer(), resolveResults.begin(), resolveResults.end(), err);
        if (err) return false;

        ws.handshake(host, path + "/network-proxy/connect?token=" + UrlEncode(token), err);
        if (err) return false;

        terminating = false;

        if (t.joinable()) t.join();
        t = thread(bind(&ProxyClientImpl::ThreadMain, this));

        return true;
    }

    void Disconnect() override {
        if (!ws.is_open()) return;

        {
            unique_lock<mutex> lock(terminatingMutex);

            while (!terminating) {
                ws.next_layer().cancel();

                terminatingCv.wait_for(lock, 10ms);
            }
        }

        if (t.joinable()) t.join();
    }

    bool Send(const uint8* message, size_t size) override {
        if (!ws.is_open()) return false;

        boost::system::error_code err;
        ws.binary(true);

        ws.write(net::buffer(message, size), err);

        return !err;
    }

    pair<uint8*, size_t> Receive() override {
        if (ws.is_open()) {
            unique_lock<mutex> lock(receiveMutex);

            while (!receiveBuffer && ws.is_open()) receiveCv.wait(lock);
        }

        return receiveBuffer ? make_pair(receiveBuffer.release(), receiveBufferSize)
                             : make_pair(nullptr, 0);
    }

   private:
    void ThreadMain() {
        boost::asio::spawn(io_context, bind(&ProxyClientImpl::ThreadLoop, this, _1));

        io_context.restart();
        io_context.run();

        logging::printf("websocket connection closed");
    }

    void ThreadLoop(boost::asio::yield_context yield) {
        while (ws.is_open()) {
            boost::beast::flat_buffer buffer;
            boost::system::error_code err;

            // Read a message into our buffer
            ws.async_read(buffer, yield[err]);

            {
                unique_lock<mutex> lock(receiveMutex);

                if (err) {
                    if (ws.is_open()) ws.async_close(websocket::close_reason(), yield[err]);
                }

                if (ws.is_open()) {
                    if (receiveBuffer) logging::printf("WARNING: discarding pending response");

                    receiveBufferSize = buffer.size();
                    receiveBuffer = make_unique<uint8[]>(receiveBufferSize);

                    memcpy(receiveBuffer.get(), buffer.cdata().data(), receiveBufferSize);
                }
            }

            receiveCv.notify_one();
        }

        {
            unique_lock<mutex> lock(terminatingMutex);

            terminating = true;
        }

        terminatingCv.notify_one();
    }

    bool Handshake(const tcp::resolver::results_type& resolveResults, string& token) {
        boost::system::error_code err;

        beast::tcp_stream stream(io_context);

        stream.connect(resolveResults, err);
        if (err) return false;

        http::request<http::string_body> req{http::verb::post, path + "/network-proxy/handshake",
                                             11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream, req, err);
        if (err) {
            stream.socket().shutdown(tcp::socket::shutdown_both, err);
            return false;
        }

        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;

        http::read(stream, buffer, res, err);
        if (err) {
            stream.socket().shutdown(tcp::socket::shutdown_both, err);
            return false;
        }

        stream.socket().shutdown(tcp::socket::shutdown_both, err);
        if (err && err != beast::errc::not_connected) return false;

        if (res.result_int() != 200) return false;

        string body;
        for (auto chunk : res.body().cdata())
            body.append(net::buffer_cast<const char*>(chunk), net::buffer_size(chunk));

        ArduinoJson::StaticJsonDocument<2048> response;
        if (ArduinoJson::deserializeJson(response, body) != ArduinoJson::DeserializationError::Ok ||
            !response.is<ArduinoJson::JsonObject>()) {
            return false;
        }

        if (!response.containsKey("version") || !response.containsKey("version")) return false;

        if (!response["version"].is<int>() || response["version"].as<int>() < SERVER_VERSION ||
            !response["compatVersion"].is<int>() ||
            response["compatVersion"].as<int>() > SERVER_VERSION) {
            logging::printf("server version mismatch");
            return false;
        }

        if (!response["token"].is<const char*>()) return false;
        token = response["token"].as<const char*>();

        return true;
    }

   private:
    net::io_context io_context;
    tcp::resolver resolver{io_context};
    websocket::stream<tcp::socket> ws{io_context};

    thread t;

    string host;
    string port;
    string path;

    unique_ptr<uint8[]> receiveBuffer;
    size_t receiveBufferSize;

    mutex receiveMutex;
    condition_variable receiveCv;

    bool terminating;
    mutex terminatingMutex;
    condition_variable terminatingCv;

   private:
    ProxyClientImpl(const ProxyClientImpl&) = delete;
    ProxyClientImpl(ProxyClientImpl&&) = delete;
    ProxyClientImpl& operator=(const ProxyClientImpl&) = delete;
    ProxyClientImpl& operator=(ProxyClientImpl&&) = delete;
};

ProxyClient* ProxyClient::Create(const string& host, long port, const string& path) {
    return new ProxyClientImpl(host, port, path);
}
