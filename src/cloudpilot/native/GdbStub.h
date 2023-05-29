#ifndef _GDB_STUB_H_
#define _GDB_STUB_H_

#include <string>

#include "EmCommon.h"

class GdbStub {
   public:
    enum class ConnectionState { socketClosed, listening, connected };

    enum class RunState { running, stopped };

   public:
    GdbStub();
    ~GdbStub();

    ConnectionState GetConnectionState() const;
    RunState GetRunState() const;

    void Listen(uint32 port);
    void Stop();

    void Cycle(int timeout);

   private:
    void AcceptConnection(int timeout);

    void ReceivePacket();

   private:
    ConnectionState connectionState{ConnectionState::socketClosed};
    RunState runState{RunState::stopped};

    int sock{0};

    uint8* pktBuf{nullptr};
    size_t pktBufSz, pktBufUsed{0};

    std::string stopReason;

   private:
    GdbStub(const GdbStub&) = delete;
    GdbStub(GdbStub&&) = delete;
    GdbStub& operator=(const GdbStub&) = delete;
    GdbStub& operator=(GdbStub&&) = delete;
};

#endif  // _GDB_STUB_H_
