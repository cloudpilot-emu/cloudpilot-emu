#include "proxyClientNative.h"

#include <condition_variable>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <mutex>
#include <optional>
#include <vector>

#include "EmCommon.h"
#include "networkBackend.h"

namespace {
    extern "C" void rpcResultCb(unsigned int sessiondId, const void* data, size_t len,
                                void* context);

    class ProxyClientNative : public ProxyClient {
       public:
        ProxyClientNative() { net_setRpcCallback(&::rpcResultCb, this); }

        bool Connect() override {
            if (currentSession) net_closeSession(*currentSession);

            currentSession = net_openSession();

            return true;
        }

        void Disconnect() override {
            if (currentSession) net_closeSession(*currentSession);
            currentSession = nullopt;
        }

        bool Send(const uint8* message, size_t size) override {
            if (!currentSession) {
                cerr << "ERROR: rpc send without pending session" << endl;
                return false;
            }

            return net_dispatchRpc(*currentSession, message, size);
        }

        pair<uint8*, size_t> Receive() override {
            unique_lock lock(receiveMutex);

            while (!hasPendingResponse) receiveCv.wait(lock);

            uint8* data = responseData.release();
            hasPendingResponse = false;

            return {data, responseSize};
        }

        void RpcResultCb(unsigned int sessiondId, const void* data, size_t len) {
            if (!currentSession) {
                cerr << "ERROR: rpc response without pending session" << endl;
                return;
            }

            if (*currentSession != sessiondId) {
                cerr << "ERROR: session ID mismatch" << endl;
                return;
            }

            unique_lock lock(receiveMutex);

            if (hasPendingResponse)
                cerr << "ERROR: rpc response while there is pending data" << endl;

            responseData = make_unique<uint8[]>(len);

            memcpy(responseData.get(), data, len);
            responseSize = len;
            hasPendingResponse = true;

            receiveCv.notify_one();
        }

       private:
        optional<uint32> currentSession;

        unique_ptr<uint8[]> responseData;
        size_t responseSize{0};
        bool hasPendingResponse{false};

        mutex receiveMutex;
        condition_variable receiveCv;
    };

    void rpcResultCb(unsigned int sessiondId, const void* data, size_t len, void* context) {
        reinterpret_cast<ProxyClientNative*>(context)->RpcResultCb(sessiondId, data, len);
    }
}  // namespace

ProxyClient* proxyClientNative::Create() { return new ProxyClientNative(); }