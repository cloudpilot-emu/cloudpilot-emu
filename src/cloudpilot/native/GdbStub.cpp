#include "GdbStub.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Defer.h"

namespace {
    constexpr size_t PKT_BUF_SIZE = 1024;

    const char* GDB_SIG0 = "S00";
    const char* GDB_SIGINT = "S02";
    const char* GDB_SIGTRAP = "S05";
}  // namespace

GdbStub::GdbStub(Debugger& debugger, uint32 listenPort)
    : listenPort(listenPort), pktBuf(make_unique<uint8[]>(PKT_BUF_SIZE)), debugger(debugger) {}

GdbStub::ConnectionState GdbStub::GetConnectionState() const { return connectionState; }

GdbStub::RunState GdbStub::GetRunState() const { return runState; }

void GdbStub::Listen() {
    if (connectionState != ConnectionState::socketClosed) return;

    struct sockaddr_in sa = {
        .sin_family = AF_INET,
        .sin_port = htons(listenPort),
    };

    inet_aton("0.0.0.0", (struct in_addr*)&sa.sin_addr.s_addr);

    int newSock = socket(PF_INET, SOCK_STREAM, 0);
    if (newSock < 0) {
        std::cerr << "gdb socket creation failed: " << errno << endl << flush;
        return;
    }

    Defer cleanupSocket([&]() {
        if (connectionState != ConnectionState::listening) close(sock);
    });

    sock = newSock;

    if (::bind(sock, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        std::cerr << "gdb socket bind failed: " << errno << endl << flush;
        return;
    }

    if (listen(sock, 1) == 1) {
        std::cerr << "gdb socket listen failed: " << errno << endl << flush;
        return;
    }

    connectionState = ConnectionState::listening;
    std::cout << "debugger listening on port " << listenPort << endl << flush;
}

void GdbStub::Stop() {
    if (connectionState == ConnectionState::socketClosed) return;
    close(sock);

    connectionState = ConnectionState::socketClosed;

    debugger.Continue();
}

void GdbStub::Cycle(int timeout) {
    switch (connectionState) {
        case ConnectionState::socketClosed:
            return;

        case ConnectionState::listening:
            return AcceptConnection(timeout);

        case ConnectionState::connected:
            switch (runState) {
                case RunState::running:
                    CheckForInterrupt(timeout);
                    CheckForBreak();

                    return;

                case RunState::stopped:
                    if (ReceivePacket(timeout)) HandlePacket();
                    return;
            }
    }
}

void GdbStub::AcceptConnection(int timeout) {
    if (connectionState != ConnectionState::listening) return;

    if (PollSocket(timeout) != SocketState::data) return;

    struct sockaddr_in sa;
    socklen_t saLen = sizeof(sa);

    int acceptSock = accept(sock, reinterpret_cast<sockaddr*>(&sa), &saLen);
    if (acceptSock == -1) {
        if (errno != EWOULDBLOCK) std::cerr << "accept failed: " << errno << endl << flush;

        return;
    }

    std::cout << "debugger connected" << endl << flush;

    close(sock);
    sock = acceptSock;
    connectionState = ConnectionState::connected;

    debugger.Interrupt();
}

void GdbStub::CheckForInterrupt(int timeout) {
    if (connectionState != ConnectionState::connected || runState != RunState::running) return;

    if (PollSocket(timeout) != SocketState::data) return;

    uint8 cmd;
    ssize_t bytesRead = recv(sock, &cmd, 1, 0);

    if (bytesRead != 1) {
        std::cerr << "gdb stub: check for interrupt: failed to receive one byte: " << errno << endl
                  << flush;
        return;
    }

    if (cmd != 3) {
        std::cerr << "gdb stub: check for interrup: invalid command byte " << hex << (int)cmd << dec
                  << endl
                  << flush;
        return;
    }

    debugger.Interrupt();
}

void GdbStub::CheckForBreak() {
    if (connectionState != ConnectionState::connected || runState != RunState::running) return;

    const char* stopReason = StopReason();
    if (stopReason == GDB_SIG0) return;

    runState = RunState::stopped;
    SendPacket(stopReason, false);
}

bool GdbStub::ReceivePacket(int timeout) {
    if (connectionState != ConnectionState::connected || runState != RunState::stopped)
        return false;

    if (PollSocket(timeout) != SocketState::data) return false;

    bool inEsc = false, first = true;
    int ret, endLeft = 0;
    char c;

    pktBufUsed = 0;
    do {
        ret = recv(sock, &c, 1, 0);

        if (ret <= 0) {
            std::cerr << "gdb stub: failed to receive" << endl << flush;
            return false;
        }

        if (first) {
            if (c == 3)  // spurious interrupt - ignore it - we are already stopped
                continue;

            if (c == '$') first = false;

            continue;
        }
        if (inEsc) {
            c ^= 0x20;
            inEsc = false;
        } else if (c == 0x7d) {
            inEsc = true;
            continue;
        } else if (c == '#') {
            endLeft = 2;
            continue;
        } else if (endLeft) {
            if (--endLeft) continue;
            c = 0;
        }

        if (pktBufUsed == PKT_BUF_SIZE) {
            std::cerr << "gdb stub: packet buffer overlow" << endl << flush;
            return false;
        }

        pktBuf.get()[pktBufUsed++] = c;
    } while (c);

    return true;
}

void GdbStub::HandlePacket() { cout << "handling packet " << (char*)pktBuf.get() << endl << flush; }

void GdbStub::SendPacket(const char* packet, bool includeAck) {
    size_t len = strlen(packet);
    uint8 sum = 0;

    for (size_t i = 0; i < len; i++) sum += packet[i];

    char end[4];
    snprintf(end, sizeof(end), "#%02x", sum);

    if (includeAck) SendAck();

    SendBytes("$", 1);
    SendBytes(packet, len);
    SendBytes(end, 3);
}

void GdbStub::SendBytes(const char* data, size_t len) {
    if (len == 0) return;

    do {
        ssize_t bytesSent = send(sock, data, len, 0);

        if (bytesSent <= 0) {
            std::cerr << "gdb stub: failed to send" << endl << flush;
            return;
        }

        data += bytesSent;
        len -= bytesSent;
    } while (len > 0);
}

void GdbStub::SendAck() { SendBytes("+", 1); }

GdbStub::SocketState GdbStub::PollSocket(int timeout) {
    struct pollfd fds[] = {{.fd = sock, .events = POLLIN}};
    int pollResult = poll(fds, 1, timeout);

    if (pollResult < 0) {
        std::cerr << "gdb stub: poll failed: " << errno << endl << flush;
        return SocketState::none;
    }

    if ((pollResult & POLLIN) == POLLIN) return SocketState::data;

    if ((pollResult & POLLHUP) == POLLHUP) {
        std::cout << "debugger disconnected" << endl << flush;
        connectionState = ConnectionState::socketClosed;

        runState = RunState::running;
        debugger.Reset();
        pktBufUsed = 0;
        sock = 0;

        Listen();
    }

    return SocketState::none;
}

const char* GdbStub::StopReason() const {
    switch (debugger.GetBreakState()) {
        case Debugger::BreakState::breakpoint:
            return GDB_SIGINT;

        case Debugger::BreakState::externalInterrupt:
        case Debugger::BreakState::trapInternal:
        case Debugger::BreakState::step:
            return GDB_SIGTRAP;

        case Debugger::BreakState::trapRead:
        case Debugger::BreakState::trapWrite:
            // CSTODO
            return GDB_SIG0;

        case Debugger::BreakState::none:
            EmAssert(false);
            return GDB_SIG0;
    }
}
