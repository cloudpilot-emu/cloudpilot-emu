#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "ProxyClient.h"

#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std;
using namespace std::chrono_literals;
using namespace std::placeholders;

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;

class ProxyClientImpl : public ProxyClient {
   public:
    ProxyClientImpl(const string& host, long port, const string& path)
        : host(host), port(to_string(port)), path(path), terminating(true) {}

    bool Connect() override {
        if (ws.is_open()) return true;

        boost::system::error_code err;

        auto const resolveResults = resolver.resolve(host, port, err);
        if (err) return false;

        net::connect(ws.next_layer(), resolveResults.begin(), resolveResults.end(), err);
        if (err) return false;

        ws.handshake(host, path, err);
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
                    if (receiveBuffer)
                        cerr << "WARNING: discarding pending response" << endl << flush;

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
