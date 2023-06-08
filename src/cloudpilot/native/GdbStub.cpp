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
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Defer.h"

namespace {
    constexpr size_t PKT_BUF_SIZE = 1024;

    const char* GDB_SIG0 = "S00";
    const char* GDB_SIGINT = "S02";
    const char* GDB_SIGTRAP = "S05";
}  // namespace

GdbStub::GdbStub(Debugger& debugger, uint32 listenPort)
    : listenPort(listenPort), pktBuf(make_unique<char[]>(PKT_BUF_SIZE)), debugger(debugger) {}

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
        if (connectionState != ConnectionState::listening) close(listenSock);
    });

    listenSock = newSock;
    int optVal = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEPORT, &optVal, sizeof(optVal));

    if (::bind(listenSock, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        std::cerr << "gdb socket bind failed: " << errno << endl << flush;
        return;
    }

    if (listen(listenSock, 1) == 1) {
        std::cerr << "gdb socket listen failed: " << errno << endl << flush;
        return;
    }

    connectionState = ConnectionState::listening;
    std::cout << "debugger listening on port " << listenPort << endl << flush;
}

void GdbStub::Stop() {
    if (connectionState == ConnectionState::socketClosed) return;
    close(listenSock);
    close(connectionSock);

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
                    if (ReceivePacket(timeout) && HandlePacket()) SendPacket(pktBuf.get(), true);

                    return;
            }
    }
}

void GdbStub::AcceptConnection(int timeout) {
    if (connectionState != ConnectionState::listening) return;

    if (PollSocket(listenSock, timeout) != SocketState::data) return;

    struct sockaddr_in sa;
    socklen_t saLen = sizeof(sa);

    int acceptSock = accept(listenSock, reinterpret_cast<sockaddr*>(&sa), &saLen);
    if (acceptSock == -1) {
        if (errno != EWOULDBLOCK) std::cerr << "accept failed: " << errno << endl << flush;

        return;
    }

    std::cout << "debugger connected" << endl << flush;

    connectionSock = acceptSock;
    connectionState = ConnectionState::connected;

    debugger.Reset();
    debugger.Interrupt();
    runState = RunState::stopped;
}

void GdbStub::CheckForInterrupt(int timeout) {
    if (connectionState != ConnectionState::connected || runState != RunState::running) return;

    if (PollSocket(connectionSock, timeout) != SocketState::data) return;

    uint8 cmd;
    ssize_t bytesRead = recv(connectionSock, &cmd, 1, 0);

    if (bytesRead != 1) {
        std::cerr << "gdb stub: check for interrupt: failed to receive one byte: " << errno << endl
                  << flush;
        Disconnect();
        return;
    }

    if (cmd != 3) {
        std::cerr << "gdb stub: check for interrup: invalid command byte " << hex << (int)cmd << dec
                  << endl
                  << flush;
        Disconnect();
        return;
    }

    debugger.Interrupt();
}

void GdbStub::CheckForBreak() {
    if (connectionState != ConnectionState::connected || runState != RunState::running ||
        debugger.GetBreakState() == Debugger::BreakState::none)
        return;

    const char* stopReason = StopReason();
    if (stopReason == GDB_SIG0) return;

    runState = RunState::stopped;
    SendPacket(stopReason, false);
}

bool GdbStub::ReceivePacket(int timeout) {
    if (connectionState != ConnectionState::connected || runState != RunState::stopped)
        return false;

    if (PollSocket(connectionSock, timeout) != SocketState::data) return false;

    bool inEsc = false, first = true;
    int ret, endLeft = 0;
    char c;

    pktBufUsed = 0;
    do {
        ret = recv(connectionSock, &c, 1, 0);

        if (ret <= 0) {
            std::cerr << "gdb stub: failed to receive" << endl << flush;
            Disconnect();
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

bool GdbStub::HandlePacket() {
    std::cout << "handling packet " << (char*)pktBuf.get() << endl << flush;

    const char* in = pktBuf.get();
    char* out = pktBuf.get();

    if (in == strstr(in, "qSupported"))
        strcpy(out, "PacketSize=1024");

    else if (in == strstr(in, "qAttached"))
        strcpy(out, "1");

    else if (!strcmp(in, "qSymbol::"))
        strcpy(out, "OK");

    else if (in == strstr(in, "qL"))
        out[0] = 0;

    else if (!strcmp(in, "qTStatus"))
        out[0] = 0;

    else if (!strcmp(in, "qfThreadInfo"))
        out[0] = 0;

    else if (in[0] == 'v')
        out[0] = 0;

    else if (!strcmp(in, "qC"))
        out[0] = 0;

    else if (!strcmp(in, "qOffsets"))
        strcpy(out, "Text=0;Data=0;Bss=0");

    else if (in[0] == 'H')
        strcpy(out, "OK");

    else if (!strcmp(in, "?"))
        strcpy(out, StopReason());

    else if (!strcmp(in, "D")) {
        Disconnect();
        return false;
    }
#if 0
    else if (in[0] == 'Z' || in[0] == 'z') {
        bool set = in[0] == 'Z';
        char type = in[1];
        uint32_t addr, len;

        if (strlen(in) < 3) goto cmderr;

        in += 2;
        if (*in++ != ',') goto cmderr;
        addr = gdbStubPrvHtoi(&in);
        if (*in++ != ',') goto cmderr;
        len = gdbStubPrvHtoi(&in);
        if (*in) goto cmderr;

        if (gdbStubPrvBpWpOp(stub, set, type, addr, len))
            strcpy(out, "OK");
        else
            strcpy(out, "E05");
    }
#endif
#if 0
    else if (in[0] == 'p') {
        uint32_t regNo;

        in++;
        regNo = gdbStubPrvHtoi(&in);
        if (*in) goto cmderr;

        out[0] = 0;
        if (!gdbStubPrvAddRegToStr(stub, out, regNo)) goto cmderr;
    }
#endif
    else if (!strcmp(in, "g")) {
        out[0] = 0;
        SerializeRegisters(out);
    }
#if 0
    else if (in[0] == 'P') {
        uint32_t regNo, val;

        in++;
        regNo = gdbStubPrvHtoi(&in);
        if (*in++ != '=') goto cmderr;
        val = __builtin_bswap32(gdbStubPrvHtoi(&in));

        if (regNo < 15) {
            cpuSetReg(stub->cpu, regNo, val);
            sprintf(out, "OK");
        } else if (regNo == 15) {  // cpuSetReg(PC, ...) does interworking. we need to be careful

            cpuSetReg(
                stub->cpu, 15,
                (val & ~1) | ((cpuGetRegExternal(stub->cpu, ARM_REG_NUM_CPSR) & ARM_SR_T) ? 1 : 0));
        } else if (regNo == 0x19) {
            cpuSetReg(stub->cpu, ARM_REG_NUM_CPSR, val);
            sprintf(out, "OK");
        } else
            strcpy(out, "E05");
    }
#endif
#if 0
    else if (in[0] == 'M') {
        uint32_t addr, len, i;

        in++;
        addr = gdbStubPrvHtoi(&in);
        if (*in++ != ',') goto cmderr;

        len = gdbStubPrvHtoi(&in);
        if (*in++ != ':') goto cmderr;

        if (strlen(in) != len * 2) goto cmderr;

        // convert to binary
        for (i = 0; i < len; i++) {
            char c[3] = {in[i * 2 + 0], in[i * 2 + 1], 0};
            const char* p = c;

            out[i] = gdbStubPrvHtoi(&p);
            if (p != c + 2) goto cmderr;
        }

        if (len == gdbStubPrvMemWrite(stub, out, addr, len))
            strcpy(out, "OK");
        else
            strcpy(out, "E0e");
    }
#endif
#if 0
    else if (in[0] == 'm') {
        uint32_t addr, len;

        in++;
        addr = gdbStubPrvHtoi(&in);
        if (*in++ != ',') goto cmderr;
        len = gdbStubPrvHtoi(&in);
        if (*in) goto cmderr;
        out[0] = 0;

        if (!gdbStubPrvMemRead(stub, out, addr, len))
            strcpy(out, "00");  // le sigh...gdb does not handle actual read failures, so we report
                                // all unreadable memory as zeroes...
    }
#endif
    else if (!strcmp(in, "s") || in[0] == 'S') {  // single step [with signal, which we ignore]

        SendAck();
        debugger.Step();
        runState = RunState::running;
        return false;
    }

    else if (!strcmp(in, "c") || in[0] == 'C') {  // continue [with signal, which we ignore]

        SendAck();
        debugger.Continue();
        runState = RunState::running;
        return false;
    }

    else {
        // cmderr:
        fprintf(stderr, "unhandled packet <<%s>>\n", in);
        out[0] = 0;
    }

    return true;
}

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
        ssize_t bytesSent = send(connectionSock, data, len, 0);

        if (bytesSent <= 0) {
            std::cerr << "gdb stub: failed to send" << endl << flush;
            return;
        }

        data += bytesSent;
        len -= bytesSent;
    } while (len > 0);
}

void GdbStub::SendAck() { SendBytes("+", 1); }

void GdbStub::SerializeRegisters(char* destination) {
    // straight from the horse's mouth:
    //
    // Return the GDB type object for the "standard" data type of data in
    // register N.  This should be int for D0-D7, SR, FPCONTROL and
    // FPSTATUS, long double for FP0-FP7, and void pointer for all others
    // (A0-A7, PC, FPIADDR).  Note, for registers which contain
    // addresses return pointer to void, not pointer to char, because we
    // don't want to attempt to print the string after printing the
    // address.

    ostringstream sstream;

    auto registers = debugger.ReadRegisters();
    for (uint32 reg : registers) sstream << hex << setfill('0') << setw(8) << reg;

    // fp[0] .. fp[7] are 96bit long doubles
    for (int i = 0; i < 12 * 8; i++) sstream << "00";

    // fudge the three int32 FPU registers
    for (int i = 0; i < 3; i++) sstream << "00000000";

    strcat(destination, sstream.str().c_str());
}

GdbStub::SocketState GdbStub::PollSocket(int socket, int timeout) {
    struct pollfd fds[] = {{.fd = socket, .events = POLLIN}};
    int pollResult = poll(fds, 1, timeout);

    if (pollResult < 0) {
        std::cerr << "gdb stub: poll failed: " << errno << endl << flush;
        return SocketState::none;
    }

    if ((pollResult & POLLIN) == POLLIN) return SocketState::data;

    if ((pollResult & POLLHUP) == POLLHUP) {
        Disconnect();
    }

    return SocketState::none;
}

void GdbStub::Disconnect() {
    std::cout << "debugger disconnected" << endl << flush;

    close(connectionSock);

    connectionState = ConnectionState::listening;
    runState = RunState::running;
    debugger.Reset();
    pktBufUsed = 0;
    connectionSock = 0;
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
