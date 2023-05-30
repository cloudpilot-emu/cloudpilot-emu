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

#include <iostream>

#include "Defer.h"

namespace {
    constexpr size_t INITAL_PKT_BUF_SIZE = 4096;
    // constexpr size_t MAX_PKT_BUF_SIZE = 1024 * 1024;
}  // namespace

GdbStub::GdbStub() {
    pktBuf = reinterpret_cast<uint8*>(malloc(INITAL_PKT_BUF_SIZE));
    pktBufSz = INITAL_PKT_BUF_SIZE;
}

GdbStub::~GdbStub() { free(pktBuf); }

GdbStub::ConnectionState GdbStub::GetConnectionState() const { return connectionState; }

GdbStub::RunState GdbStub::GetRunState() const { return runState; }

void GdbStub::Listen(uint32 port) {
    if (connectionState != ConnectionState::socketClosed) return;

    struct sockaddr_in sa = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
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
    std::cout << "debugger listening on port " << port << endl << flush;
}

void GdbStub::Stop() {
    if (connectionState == ConnectionState::socketClosed) return;
    close(sock);

    connectionState = ConnectionState::socketClosed;
}

void GdbStub::Cycle(int timeout) {
    switch (connectionState) {
        case ConnectionState::socketClosed:
            return;

        case ConnectionState::listening:
            return AcceptConnection(timeout);

        case ConnectionState::connected:
            break;
    }
}

void GdbStub::AcceptConnection(int timeout) {
    if (connectionState != ConnectionState::listening) return;

    struct pollfd fds[] = {{.fd = sock, .events = POLL_IN}};
    int pollResult = poll(fds, 1, timeout);

    if (pollResult < 0) {
        std::cerr << "poll failed: " << errno << endl << flush;
        return;
    }

    if (pollResult == 0 || (fds[0].revents | POLL_IN) != POLL_IN) return;

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
}
