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

    bool IsDebuggerConnected();

    void SetRelocationOffset(int64 offset);
    void ClearRelocationOffset();

   private:
    void ResetPacketParser();

    void AcceptConnection(int timeout);
    SocketState PollSocket(int socket, int timeout);

    void CheckForInterrupt(int timeout);
    void CheckForBreak();
    bool ReceivePacket(int timeout);
    bool HandlePacket();

    void SendPacket(const char* packet, bool includeAck);
    void SendBytes(const char* data, size_t len);
    void SendAck();

    void SerializeRegisters(char* destination);
    void SerializeRegister(char* destination, uint32 index);
    void ReadMemory(emuptr address, size_t count, char* dest);
    uint32 ReadHtoi(const char** input);

    void Disconnect();

    const char* StopReason() const;

   private:
    ConnectionState connectionState{ConnectionState::socketClosed};
    RunState runState{RunState::running};

    uint32 listenPort{0};
    int listenSock{0};
    int connectionSock{0};

    unique_ptr<char[]> pktBuf;
    size_t pktBufUsed{0};

    bool packetFirstChar{true};
    bool packetInEsc{false};
    bool packetInProgress{false};
    uint32 packetEndLeft{2};

    int64 relocationOffset{0};

    Debugger& debugger;

   private:
    GdbStub(const GdbStub&) = delete;
    GdbStub(GdbStub&&) = delete;
    GdbStub& operator=(const GdbStub&) = delete;
    GdbStub& operator=(GdbStub&&) = delete;
};

#endif  // _GDB_STUB_H_
