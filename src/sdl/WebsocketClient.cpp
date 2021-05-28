#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#define BOOST_NO_EXCEPTIONS
#define BOOST_NO_RTTI
#define BOOST_NO_TYPEID
#define BOOST_BEAST_CORE_IMPL_BASIC_STREAM_HPP

#include "WebsocketClient.h"

#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <cstdio>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;

void boost::throw_exception(std::exception const&) {
    EmAssert(false);
    exit(1);
}

void boost::throw_exception(std::exception const& exc, boost::source_location const&) {
    throw_exception(exc);
}

class WebsocketClientImpl {
   public:
    WebsocketClientImpl(const string& host, const string& port) : host(host), port(port) {}

    void Start() {
        if (running) return;

        boost::system::error_code err;

        auto const resolveResults = resolver.resolve(host, port, err);
        if (err) return;

        net::connect(ws.next_layer(), resolveResults.begin(), resolveResults.end(), err);
        if (err) return;

        ws.handshake(host, "/", err);
        if (err) return;

        t = thread(bind(&WebsocketClientImpl::Poll, this));

        running = true;
    }

    void Stop() {
        if (!running) return;

        boost::system::error_code err;
        ws.close(websocket::close_reason(), err);

        t.join();

        running = false;
    }

    bool IsRunning() const { return running; }

    void Join() { t.join(); }

    void Send(const uint8* message, size_t size) {
        if (!running) return;

        boost::system::error_code err;
        ws.binary(true);
        ws.write(net::buffer(message, size), err);
    }

   private:
    void Poll() {
        while (running) {
            boost::beast::flat_buffer buffer;
            boost::system::error_code err;

            // Read a message into our buffer
            ws.read(buffer, err);
            if (err) {
                running = false;
                break;
            }

            cout << "received: " << flush;
            write(STDOUT_FILENO, buffer.cdata().data(), buffer.cdata().size());
            cout << endl << flush;
        }
    }

   private:
    net::io_context ioc;
    tcp::resolver resolver{ioc};
    websocket::stream<tcp::socket> ws{ioc};

    thread t;

    string host;
    string port;

    atomic<bool> running;

   private:
    WebsocketClientImpl(const WebsocketClientImpl&) = delete;
    WebsocketClientImpl(WebsocketClientImpl&&) = delete;
    WebsocketClientImpl& operator=(const WebsocketClientImpl&) = delete;
    WebsocketClientImpl& operator=(WebsocketClientImpl&&) = delete;
};

WebsocketClient::WebsocketClient(const string& host, const string& port) {
    impl = new WebsocketClientImpl(host, port);
}

WebsocketClient::~WebsocketClient() { delete impl; }

void WebsocketClient::Start() { return impl->Start(); }

bool WebsocketClient::IsRunning() const { return impl->IsRunning(); }

void WebsocketClient::Join() { return impl->Join(); }

void WebsocketClient::Send(const uint8* message, size_t size) { return impl->Send(message, size); }

void WebsocketClient::Stop() { impl->Stop(); }
