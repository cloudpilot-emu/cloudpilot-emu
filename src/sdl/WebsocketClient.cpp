#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "WebsocketClient.h"

#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;

class WebsocketClientImpl : public WebsocketClient {
   public:
    WebsocketClientImpl(const string& host, const string& port) : host(host), port(port) {}

    bool Connect() override {
        if (ws.is_open()) return true;

        boost::system::error_code err;

        auto const resolveResults = resolver.resolve(host, port, err);
        if (err) return false;

        net::connect(ws.next_layer(), resolveResults.begin(), resolveResults.end(), err);
        if (err) return false;

        ws.handshake(host, "/", err);
        if (err) return false;

        if (t.joinable()) t.join();
        t = thread(bind(&WebsocketClientImpl::PollThread, this));

        return true;
    }

    void Disconnect() override {
        ws.next_layer().cancel();

        if (ws.is_open()) {
            boost::system::error_code err;
            ws.close(websocket::close_reason(), err);
        }

        if (t.joinable()) t.join();
    }

    bool IsRunning() const override { return ws.is_open(); }

    bool Send(const uint8* message, size_t size) override {
        if (!ws.is_open()) return false;

        boost::system::error_code err;
        ws.binary(true);

        ws.write(net::buffer(message, size), err);

        return !err;
    }

    std::pair<uint8*, size_t> Receive() override {
        if (ws.is_open()) {
            std::unique_lock<std::mutex> lock(receiveMutex);

            while (!receiveBuffer && ws.is_open()) receiveCv.wait(lock);
        }

        return receiveBuffer ? make_pair(receiveBuffer.release(), receiveBufferSize)
                             : make_pair(nullptr, 0);
    }

   private:
    void PollThread() {
        boost::asio::spawn(ioc, std::bind(&WebsocketClientImpl::Poll, this, std::placeholders::_1));

        ioc.restart();
        ioc.run();
    }

    void Poll(boost::asio::yield_context yield) {
        while (ws.is_open()) {
            boost::beast::flat_buffer buffer;
            boost::system::error_code err;

            // Read a message into our buffer
            ws.async_read(buffer, yield[err]);

            if (err) {
                if (ws.is_open()) ws.close(websocket::close_reason(), err);

                break;
            }

            {
                std::unique_lock<std::mutex> lock(receiveMutex);

                if (receiveBuffer) cerr << "WARNING: discarding pending response" << endl << flush;

                receiveBufferSize = buffer.size();
                receiveBuffer = make_unique<uint8[]>(receiveBufferSize);

                memcpy(receiveBuffer.get(), buffer.cdata().data(), receiveBufferSize);
            }

            receiveCv.notify_one();
        }

        receiveCv.notify_one();
    }

   private:
    net::io_context ioc;
    tcp::resolver resolver{ioc};
    websocket::stream<tcp::socket> ws{ioc};

    thread t;

    string host;
    string port;

    unique_ptr<uint8[]> receiveBuffer;
    size_t receiveBufferSize;

    std::mutex receiveMutex;
    std::condition_variable receiveCv;

   private:
    WebsocketClientImpl(const WebsocketClientImpl&) = delete;
    WebsocketClientImpl(WebsocketClientImpl&&) = delete;
    WebsocketClientImpl& operator=(const WebsocketClientImpl&) = delete;
    WebsocketClientImpl& operator=(WebsocketClientImpl&&) = delete;
};

WebsocketClient* WebsocketClient::Create(const string& host, const string& port) {
    return new WebsocketClientImpl(host, port);
}
