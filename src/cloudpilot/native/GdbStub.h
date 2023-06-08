#ifndef _GDB_STUB_H_
#define _GDB_STUB_H_

#include <memory>
#include <string>

#include "Debugger.h"
#include "EmCommon.h"

class GdbStub {
   public:
    enum class ConnectionState { socketClosed, listening, connected };

    enum class RunState { running, stopped };

    enum class SocketState { none, data, hangup };

   public:
    GdbStub(Debugger& debugger, uint32 listenPort);

    ConnectionState GetConnectionState() const;
    RunState GetRunState() const;

    void Listen();
    void Stop();

    void Cycle(int timeout);

   private:
    void AcceptConnection(int timeout);

    void CheckForInterrupt(int timeout);
    void CheckForBreak();
    bool ReceivePacket(int timeout);
    bool HandlePacket();

    void SendPacket(const char* packet, bool includeAck);
    void SendBytes(const char* data, size_t len);
    void SendAck();

    void SerializeRegisters(char* destination);

    SocketState PollSocket(int timeout);

    void Disconnect();

    const char* StopReason() const;

   private:
    ConnectionState connectionState{ConnectionState::socketClosed};
    RunState runState{RunState::running};

    uint32 listenPort{0};
    int sock{0};

    unique_ptr<char[]> pktBuf;
    size_t pktBufUsed{0};

    Debugger& debugger;

   private:
    GdbStub(const GdbStub&) = delete;
    GdbStub(GdbStub&&) = delete;
    GdbStub& operator=(const GdbStub&) = delete;
    GdbStub& operator=(GdbStub&&) = delete;
};

#endif  // _GDB_STUB_H_
