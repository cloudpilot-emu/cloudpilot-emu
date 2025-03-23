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
#include "LogDomain.h"
#include "Logging.h"

class EInvalidCommand {};

namespace {
    constexpr size_t PKT_BUF_SIZE = 1024;
    constexpr size_t MAX_MEMORY_READ_SIZE = 500;

    const char* GDB_SIG0 = "S00";
    const char* GDB_SIGINT = "S02";
    const char* GDB_SIGTRAP = "S05";

    template <typename F, typename... Ts>
    int withRetry(F fn, Ts... args) {
        int result;
        do {
            result = fn(args...);
        } while (result < 0 && errno == EINTR);

        return result;
    }

    bool setSocketNonblock(int sock) {
        int flags = withRetry(fcntl, sock, F_GETFL, 0);

        return flags >= 0 && withRetry(fcntl, sock, F_SETFL, flags | O_NONBLOCK) >= 0;
    }

    const char* watchpointCode(Debugger::WatchpointType type) {
        switch (type) {
            case Debugger::WatchpointType::read:
                return "r";

            case Debugger::WatchpointType::readwrite:
                return "a";

            default:
                return "";
        }
    }

    Debugger::WatchpointType decodeWatchpointType(char type) {
        switch (type) {
            case '2':
                return Debugger::WatchpointType::write;

            case '3':
                return Debugger::WatchpointType::read;

            default:
                return Debugger::WatchpointType::readwrite;
        }
    }
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

    int newSock = withRetry(socket, PF_INET, SOCK_STREAM, 0);
    if (newSock < 0) {
        std::cerr << "gdb socket creation failed: " << errno << endl << flush;
        return;
    }

    Defer cleanupSocket([&]() {
        if (connectionState != ConnectionState::listening) close(listenSock);
    });

    listenSock = newSock;
    int optVal = 1;
    withRetry(setsockopt, listenSock, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    withRetry(setsockopt, listenSock, SOL_SOCKET, SO_REUSEPORT, &optVal, sizeof(optVal));

    if (withRetry(::bind, listenSock, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        std::cerr << "gdb socket bind failed: " << errno << endl << flush;
        return;
    }

    if (withRetry(listen, listenSock, 1) == 1) {
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
                    while (ReceivePacket(timeout) && HandlePacket()) SendPacket(pktBuf.get(), true);

                    return;
            }
    }
}

bool GdbStub::IsDebuggerConnected() { return connectionState == ConnectionState::connected; }

void GdbStub::SetRelocationOffset(int64 offset) { relocationOffset = offset; }

void GdbStub::ClearRelocationOffset() { relocationOffset = 0; }

void GdbStub::ResetPacketParser() {
    packetFirstChar = true;
    packetInEsc = false;
    packetInProgress = false;
    packetEndLeft = 0;
    pktBufUsed = 0;
}

void GdbStub::AcceptConnection(int timeout) {
    if (connectionState != ConnectionState::listening) return;

    if (PollSocket(listenSock, timeout) != SocketState::data) return;

    struct sockaddr_in sa;
    socklen_t saLen = sizeof(sa);

    int acceptSock = withRetry(accept, listenSock, reinterpret_cast<sockaddr*>(&sa), &saLen);
    if (acceptSock == -1) {
        if (errno != EWOULDBLOCK) std::cerr << "accept failed: " << errno << endl << flush;
        return;
    }

    if (!setSocketNonblock(acceptSock)) {
        std::cerr << "failed to set socket nonblocking: " << errno << endl << flush;
        close(acceptSock);

        return;
    }

    std::cout << "debugger connected" << endl << flush;

    connectionSock = acceptSock;
    connectionState = ConnectionState::connected;

    debugger.Reset();
    debugger.Enable();
    debugger.Interrupt();

    ResetPacketParser();
    runState = RunState::stopped;
}

void GdbStub::CheckForInterrupt(int timeout) {
    if (connectionState != ConnectionState::connected || runState != RunState::running) return;

    if (PollSocket(connectionSock, timeout) != SocketState::data) return;

    uint8 cmd;
    ssize_t bytesRead = withRetry(recv, connectionSock, &cmd, 1, 0);

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

    debugger.UpdateBreakState();

    runState = RunState::stopped;
    SendPacket(stopReason, false);
}

bool GdbStub::ReceivePacket(int timeout) {
    if (connectionState != ConnectionState::connected) return false;

    if (PollSocket(connectionSock, timeout) != SocketState::data) return false;

    if (!packetInProgress) {
        ResetPacketParser();
        packetInProgress = true;
    }

    char c;

    do {
        int recvResult = withRetry(recv, connectionSock, &c, 1, 0);

        if (recvResult < 0) {
            // EAGAIN -> buffer is empty
            if (errno != EAGAIN) {
                std::cerr << "gdb stub: recv failed: " << errno << endl << flush;
                Disconnect();
            }

            return false;
        }

        // socket closed
        if (recvResult == 0) {
            Disconnect();
            return false;
        }

        if (packetFirstChar) {
            if (c == 3)  // spurious interrupt - ignore it - we are already stopped
                continue;

            if (c == '$') packetFirstChar = false;

            continue;
        }
        if (packetInEsc) {
            c ^= 0x20;
            packetInEsc = false;
        } else if (c == 0x7d) {
            packetInEsc = true;
            continue;
        } else if (c == '#') {
            packetEndLeft = 2;
            continue;
        } else if (packetEndLeft) {
            if (--packetEndLeft) continue;
            c = 0;
        }

        if (pktBufUsed == PKT_BUF_SIZE) {
            std::cerr << "gdb stub: packet buffer overlow" << endl << flush;
            Disconnect();
            return false;
        }

        pktBuf.get()[pktBufUsed++] = c;
    } while (c);

    packetInProgress = false;
    return true;
}

bool GdbStub::HandlePacket() {
    logPrintfDom(logging::Domain::domainDebugger, "gdb stub: handling packet %s",
                 (char*)pktBuf.get());

    const char* in = pktBuf.get();
    char* out = pktBuf.get();

    try {
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

        else if (!strcmp(in, "qOffsets")) {
            ostringstream sstream;
            sstream << hex << "Text=" << relocationOffset << ";Data=" << relocationOffset
                    << ";Bss=" << relocationOffset;

            strcpy(out, sstream.str().c_str());
        }

        else if (in[0] == 'H')
            strcpy(out, "OK");

        else if (!strcmp(in, "?"))
            strcpy(out, StopReason());

        else if (!strcmp(in, "D")) {
            Disconnect();
            return false;
        } else if (in[0] == 'Z' || in[0] == 'z') {
            char mode = in[0];
            char type = in[1];

            if (strlen(in) < 3) throw EInvalidCommand();

            in += 2;
            if (*in++ != ',') throw EInvalidCommand();
            uint32 addr = ReadHtoi(&in);

            if (*in++ != ',') throw EInvalidCommand();
            size_t len = ReadHtoi(&in);
            if (*in) throw EInvalidCommand();

            switch (type) {
                case '0':
                case '1':
                    mode == 'Z' ? debugger.SetBreakpoint(addr) : debugger.ClearBreakpoint(addr);
                    strcpy(out, "OK");
                    break;

                case '2':
                case '3':
                case '4':
                    mode == 'Z' ? debugger.SetWatchpoint(addr, decodeWatchpointType(type), len)
                                : debugger.ClearWatchpoint(addr, decodeWatchpointType(type), len);
                    strcpy(out, "OK");
                    break;

                default:
                    strcpy(out, "E05");
                    break;
            }
        } else if (in[0] == 'p') {
            uint32_t regNo;

            in++;
            regNo = ReadHtoi(&in);
            if (*in) throw EInvalidCommand();

            out[0] = 0;
            SerializeRegister(out, regNo);
        } else if (!strcmp(in, "g")) {
            out[0] = 0;
            SerializeRegisters(out);
        } else if (in[0] == 'P') {
            uint32_t regNo, val;

            in++;
            regNo = ReadHtoi(&in);
            if (*in++ != '=') throw EInvalidCommand();
            val = ReadHtoi(&in);

            debugger.SetRegister(regNo, val);

            strcpy(out, "OK");
        } else if (in[0] == 'M') {
            uint32_t addr, len, i;

            in++;
            addr = ReadHtoi(&in);
            if (*in++ != ',') throw EInvalidCommand();

            len = ReadHtoi(&in);
            if (*in++ != ':') throw EInvalidCommand();

            if (strlen(in) != len * 2) throw EInvalidCommand();

            // convert to binary
            for (i = 0; i < len; i++) {
                char c[3] = {in[i * 2 + 0], in[i * 2 + 1], 0};
                const char* p = c;

                out[i] = ReadHtoi(&p);
                if (p != c + 2) throw EInvalidCommand();
            }

            debugger.MemoryWrite(addr, reinterpret_cast<uint8*>(out), len);
            strcpy(out, "OK");
        } else if (in[0] == 'm') {
            uint32_t addr, len;

            in++;
            addr = ReadHtoi(&in);
            if (*in++ != ',') throw EInvalidCommand();
            len = ReadHtoi(&in);
            if (*in) throw EInvalidCommand();

            if (len > MAX_MEMORY_READ_SIZE) {
                std::cerr << "gdb stub: requested read size too large: " << len << endl << flush;
                throw EInvalidCommand();
            }

            out[0] = 0;
            ReadMemory(addr, len, out);
        } else if (!strcmp(in, "s") ||
                   in[0] == 'S') {  // single step [with signal, which we ignore]

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
            throw EInvalidCommand();
        }
    } catch (EInvalidCommand e) {
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
        ssize_t bytesSent = withRetry(send, connectionSock, data, len, 0);

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

void GdbStub::SerializeRegister(char* destination, uint32 index) {
    ostringstream sstream;

    if (index < 18) {
        auto registers = debugger.ReadRegisters();

        sstream << hex << setfill('0') << setw(8) << registers[index];
    } else if (index < 26) {
        sstream << "000000000000000000000000";
    } else if (index < 29) {
        sstream << "00000000";
    }

    strcat(destination, sstream.str().c_str());
}

void GdbStub::ReadMemory(emuptr address, size_t count, char* dest) {
    ostringstream sstream;

    switch (count) {
        case 1:
            sstream << hex << setfill('0') << setw(2)
                    << static_cast<uint32>(debugger.MemoryRead8(address));
            break;

        case 2:
        case 4: {
            uint32 value =
                count == 2 ? debugger.MemoRead16(address) : debugger.MemoryRead32(address);

            for (int i = count - 1; i >= 0; i--)
                sstream << hex << setfill('0') << setw(2)
                        << static_cast<uint32>((value >> (i * 8)) & 0xff);

            break;
        }

        default:
            for (size_t i = 0; i < count; i++)
                sstream << hex << setfill('0') << setw(2)
                        << static_cast<uint32>(debugger.MemoryRead8(address + i));

            break;
    }

    strcpy(dest, sstream.str().c_str());
}

uint32 GdbStub::ReadHtoi(const char** input) {
    const char* in = *input;
    uint32_t i = 0;
    char c;

    while ((c = *in) != 0) {
        if (c >= '0' && c <= '9')
            i = (i * 16) + (c - '0');
        else if (c >= 'a' && c <= 'f')
            i = (i * 16) + (c + 10 - 'a');
        else if (c >= 'A' && c <= 'F')
            i = (i * 16) + (c + 10 - 'A');
        else
            break;
        in++;
    }

    *input = in;

    return i;
}

GdbStub::SocketState GdbStub::PollSocket(int socket, int timeout) {
    struct pollfd fds[] = {{.fd = socket, .events = POLLIN}};
    int pollResult = withRetry(poll, fds, 1, timeout);

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
    static char reason[20];

    switch (debugger.GetBreakState()) {
        case Debugger::BreakState::breakpoint:
        case Debugger::BreakState::trapInternal:
        case Debugger::BreakState::step:
            return GDB_SIGTRAP;

        case Debugger::BreakState::externalInterrupt:
            return GDB_SIGINT;

        case Debugger::BreakState::trapRead:
        case Debugger::BreakState::trapWrite:
            snprintf(reason, 20, "T05%swatch:%08lx;", watchpointCode(debugger.GetWatchpointType()),
                     static_cast<unsigned long>(debugger.GetWatchpointAddress()));

            return reason;

        default:
            EmAssert(false);
            return GDB_SIG0;
    }
}
