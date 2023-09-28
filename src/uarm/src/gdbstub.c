//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#pragma GCC diagnostic ignored "-Wunused-variable"

#ifdef GDB_STUB_ENABLED
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>

#endif
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdbstub.h"
#include "util.h"

#define MAX_BREAKPOINTS 16
#define MAX_WATCHPOINTS 16

struct bp {
    uint32_t addr;
};

struct wp {
    uint32_t addr;
    uint8_t size : 6;
    uint8_t read : 1;
    uint8_t write : 1;
};

enum RunState {
    RunStateStopped = 0,
    RunStateSingleStep,
    RunstateRunning,
};

struct stub {
    struct ArmCpu *cpu;
    int sock;

#ifdef GDB_STUB_ENABLED
    unsigned numBp;
    struct bp bp[MAX_BREAKPOINTS];

    unsigned numWp;
    struct wp wp[MAX_WATCHPOINTS];

    enum RunState runState;

    char *pktBuf;
    unsigned pktBufSz, pktBufUsed;

    char stopReason[128];
#endif
};

#ifdef GDB_STUB_ENABLED
static void gdbStubPrvSendBytes(struct stub *stub, const char *data, unsigned len) {
    ssize_t ret;

    if (!len) return;

    do {
        ret = send(stub->sock, data, len, 0);
        if (!ret)
            ERR("send returned zero\n");
        else if (ret > 0) {
            data += ret;
            len -= ret;
        }
    } while (len && (ret > 0 || errno == EINTR));
}

static void gdbStubPrvSendAck(struct stub *stub) { gdbStubPrvSendBytes(stub, "+", 1); }

static void gdbStubPrvSendPacket(struct stub *stub, const char *packet, bool includeAck) {
    unsigned i, L = strlen(packet);
    uint8_t sum = 0;
    char end[4];

    for (i = 0; i < L; i++) sum += packet[i];
    sprintf(end, "#%02x", sum);

    fprintf(stderr, "TX: <<%s$%s%s>>\n", includeAck ? "+" : "", packet, end);

    if (includeAck) gdbStubPrvSendAck(stub);

    gdbStubPrvSendBytes(stub, "$", 1);
    gdbStubPrvSendBytes(stub, packet, L);
    gdbStubPrvSendBytes(stub, end, 3);
}

static void gdbStubPrvGetPacket(struct stub *stub) {
    bool inEsc = false, first = true;
    int ret, endLeft = 0;
    char c;

    stub->pktBufUsed = 0;
    do {
        ret = recv(stub->sock, &c, 1, 0);
        if (ret == 0) ERR("debugger connection was closed\n");
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            else
                ERR("debugger RX error %d\n", errno);
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

        if (stub->pktBufUsed == stub->pktBufSz) {
            stub->pktBuf =
                (char *)realloc(stub->pktBuf, stub->pktBufSz = stub->pktBufSz * 3 / 2 + 1);

            if (!stub->pktBuf) ERR("cannot realloc stub rx buffer\n");
        }

        stub->pktBuf[stub->pktBufUsed++] = c;
    } while (c);

    fprintf(stderr, "RX: <<%s>>\n", stub->pktBuf);
}

static uint32_t gdbStubPrvHtoi(const char **cP) {
    const char *in = *cP;
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

    *cP = in;

    return i;
}

static bool gdbStubPrvBpOp(struct stub *stub, bool set, uint32_t addr) {
    unsigned i;

    if (set) {
        if (stub->numBp == MAX_BREAKPOINTS) return false;
        stub->bp[stub->numBp++].addr = addr;
        return true;
    } else {
        for (i = 0; i < stub->numBp; i++) {
            if (stub->bp[i].addr == addr) {
                stub->bp[i] = stub->bp[--stub->numBp];
                return true;
            }
        }

        return false;
    }
}

static bool gdbStubPrvWpOp(struct stub *stub, bool set, uint32_t addr, uint_fast8_t size,
                           bool forWrite, bool forRead) {
    unsigned i;

    if (set) {
        struct wp wp = {
            .addr = addr,
            .size = size,
            .read = (uint8_t)(forRead ? 1 : 0),
            .write = (uint8_t)(forWrite ? 1 : 0),
        };

        if (size != wp.size)  // verify size fits
            return false;

        if (stub->numWp == MAX_WATCHPOINTS) return false;
        stub->wp[stub->numWp++] = wp;
        return true;
    } else {
        for (i = 0; i < stub->numWp; i++) {
            if (stub->wp[i].addr == addr && stub->wp[i].size == size &&
                !stub->wp[i].write == !forWrite && !stub->wp[i].read == !forRead) {
                stub->wp[i] = stub->wp[--stub->numWp];
                return true;
            }
        }

        return false;
    }
}

static bool gdbStubPrvBpWpOp(struct stub *stub, bool set, char type, uint32_t addr, uint32_t len) {
    // breakpoint?
    if (type == '0' || type == '1') return gdbStubPrvBpOp(stub, set, addr);

    // not watchpoint?
    if (type < '2' || type > '4') return false;

    type -= '1';
    return gdbStubPrvWpOp(stub, set, addr, len, !!(type & 1), !!(type & 2));
}

static bool gdbStubPrvAddRegToStr(struct stub *stub, char *out, uint32_t regNo) {
    if (regNo < 15)  // normal regs
        sprintf(out + strlen(out), "%08lx",
                (unsigned long)__builtin_bswap32(cpuGetRegExternal(stub->cpu, regNo)));
    else if (regNo == 15)  // PC
        sprintf(out + strlen(out), "%08lx",
                (unsigned long)__builtin_bswap32(cpuGetRegExternal(stub->cpu, regNo) & ~1));
    else if (regNo == 0x19)  // CPSR
        sprintf(out + strlen(out), "%08lx",
                (unsigned long)__builtin_bswap32(cpuGetRegExternal(stub->cpu, ARM_REG_NUM_CPSR)));
    else if (regNo < 0x18)  // FP regs
        strcat(out, "000000000000000000000000");
    else if (regNo == 0x18)  // FP status
        strcat(out, "00000000");
    else
        return false;

    return true;
}

static uint32_t gdbStubPrvMemWrite(struct stub *stub, char *data, uint32_t addr, uint32_t len) {
    char *orig = data;

    // use largest possible aligned size to do the accesses
    if (addr & 1) {
        if (!cpuMemOpExternal(stub->cpu, data, addr, 1, true)) return data - orig;
        data++;
        len--;
        addr++;
    }

    if ((addr & 2) && len >= 2) {
        if (!cpuMemOpExternal(stub->cpu, data, addr, 2, true)) return data - orig;
        data += 2;
        len -= 2;
        addr += 2;
    }

    while (len >= 4) {
        if (!cpuMemOpExternal(stub->cpu, data, addr, 4, true)) return data - orig;
        data += 4;
        len -= 4;
        addr += 4;
    }

    if (len & 2) {
        if (!cpuMemOpExternal(stub->cpu, data, addr, 2, true)) return data - orig;
        data += 2;
        len -= 2;
        addr += 2;
    }

    if (len) {
        if (!cpuMemOpExternal(stub->cpu, data, addr, 1, true)) return data - orig;
        data++;
        len--;
        addr++;
    }

    return data - orig;
}

static bool gdbStubPrvMemRead(struct stub *stub, char *out, uint32_t addr, uint32_t len) {
    char *orig = out;
    uint8_t vals[4];

    // limit size to what we can produce
    if (len > (stub->pktBufSz - 1) / 2) len = (stub->pktBufSz - 1) / 2;

    // use largest possible aligned size to do the accesses
    if (addr & 1) {
        if (!cpuMemOpExternal(stub->cpu, vals, addr, 1, false)) return out - orig;
        sprintf(out, "%02x", vals[0]);
        out += 2;
        len--;
        addr++;
    }

    if ((addr & 2) && len >= 2) {
        if (!cpuMemOpExternal(stub->cpu, vals, addr, 2, false)) return out - orig;
        sprintf(out, "%02x%02x", vals[0], vals[1]);
        out += 4;
        len -= 2;
        addr += 2;
    }

    while (len >= 4) {
        if (!cpuMemOpExternal(stub->cpu, vals, addr, 4, false)) return out - orig;
        sprintf(out, "%02x%02x%02x%02x", vals[0], vals[1], vals[2], vals[3]);
        out += 8;
        len -= 4;
        addr += 4;
    }

    if (len & 2) {
        if (!cpuMemOpExternal(stub->cpu, vals, addr, 2, false)) return out - orig;
        sprintf(out, "%02x%02x", vals[0], vals[1]);
        out += 4;
        len -= 2;
        addr += 2;
    }

    if (len) {
        if (!cpuMemOpExternal(stub->cpu, vals, addr, 1, false)) return out - orig;
        sprintf(out, "%02x", vals[0]);
        out += 2;
        len--;
        addr++;
    }

    return !!(out - orig);
}

static bool gdbStubPrvInterpPacket(struct stub *stub)  // return true if we prepared a reply
{
    const char *in = stub->pktBuf;
    char *out = stub->pktBuf;

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
        strcpy(out, stub->stopReason);

    else if (!strcmp(in, "D")) {
        fprintf(stderr, "debugger detached\n");
        exit(0);
    }

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

    else if (in[0] == 'p') {
        uint32_t regNo;

        in++;
        regNo = gdbStubPrvHtoi(&in);
        if (*in) goto cmderr;

        out[0] = 0;
        if (!gdbStubPrvAddRegToStr(stub, out, regNo)) goto cmderr;
    }

    else if (!strcmp(in, "g")) {
        uint32_t regNo;

        out[0] = 0;
        for (regNo = 0; regNo < 0x1a; regNo++) {
            if (!gdbStubPrvAddRegToStr(stub, out, regNo)) goto cmderr;
        }
    }

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
            const char *p = c;

            out[i] = gdbStubPrvHtoi(&p);
            if (p != c + 2) goto cmderr;
        }

        if (len == gdbStubPrvMemWrite(stub, out, addr, len))
            strcpy(out, "OK");
        else
            strcpy(out, "E0e");
    }

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

    else if (!strcmp(in, "s") || in[0] == 'S') {  // single step [with signal, which we ignore]

        gdbStubPrvSendAck(stub);
        stub->runState = RunStateSingleStep;
        return false;
    }

    else if (!strcmp(in, "c") || in[0] == 'C') {  // continue [with signal, which we ignore]

        gdbStubPrvSendAck(stub);
        stub->runState = RunstateRunning;
        return false;
    }

    else {
    cmderr:
        fprintf(stderr, "how do i respond to packet <<%s>>\n", in);
        out[0] = 0;
    }

    return true;
}

static void gdbStubPrvGetAndHandleCommands(struct stub *stub) {
    while (stub->runState == RunStateStopped) {
        gdbStubPrvGetPacket(stub);

        if (gdbStubPrvInterpPacket(stub)) gdbStubPrvSendPacket(stub, stub->pktBuf, true);
    }
}

static int gdbStubPrvCheckBreakpoints(struct stub *stub, uint32_t addr, bool thumb) {
    struct bp *bp = stub->bp;
    unsigned i;

    for (i = 0; i < stub->numBp; i++, bp++) {
        if (bp->addr == addr) return i;
    }
    return -1;
}

static int gdbStubPrvCheckWatchpoints(struct stub *stub, uint32_t addr, uint_fast8_t sz,
                                      bool write) {
    struct wp *wp = stub->wp;
    unsigned i;

    for (i = 0; i < stub->numWp; i++, wp++) {
        // overlap iff max(start) < min(end)
        if (((wp->addr > addr) ? wp->addr : addr) >=
            ((wp->addr + wp->size < addr + sz) ? wp->addr + wp->size : addr + sz))
            continue;

        if (write && !wp->write)
            continue;
        else if (!write && !wp->read)
            continue;

        // match
        return i;
    }
    return -1;
}

static bool gdbStubPrvCheckInterrupt(struct stub *stub) {
    struct timeval tv = {};
    fd_set set;
    int ret;
    char c;

    FD_ZERO(&set);
    FD_SET(stub->sock, &set);
    do {
        ret = select(stub->sock + 1, &set, NULL, NULL, &tv);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) ERR("select fails: %d", errno);

    if (ret == 0) return false;

    ret = recv(stub->sock, &c, 1, 0);
    if (ret != 1) ERR("failed to receive one byte: %d %d\n", ret, errno);

    if (c != 3) ERR("RXed byte ('%c' 0x%02x) is not interrupt when only that is possible\n", c, c);

    return true;
}

#endif

void gdbStubDebugBreakRequested(struct stub *stub) {
    if (stub->sock < 0) return;

#ifdef GDB_STUB_ENABLED
    strcpy(stub->stopReason, "S05");  // debug break sends "TRAP" which is 5
    stub->runState = RunStateStopped;
#endif
}

void gdbStubReportPc(struct stub *stub, uint32_t pc, bool thumb) {
    if (stub->sock < 0) return;

#ifdef GDB_STUB_ENABLED
    if (stub->runState == RunStateSingleStep) {
        strcpy(stub->stopReason, "S05");  // single step sends "TRAP" which is 5
        stub->runState = RunStateStopped;
    } else if (stub->runState == RunstateRunning && gdbStubPrvCheckInterrupt(stub)) {
        strcpy(stub->stopReason, "S02");  // Ctrl+C sends "INT" which is 2 (we also seem to be able
                                          // to send STOP which is 0x11)
        stub->runState = RunStateStopped;
    } else if (stub->runState == RunstateRunning &&
               gdbStubPrvCheckBreakpoints(stub, pc, thumb) >= 0) {
        strcpy(stub->stopReason, "S05");  // breakpoint send "TRAP" which is 5
        stub->runState = RunStateStopped;
    }

    if (stub->runState == RunStateStopped) gdbStubPrvSendPacket(stub, stub->stopReason, false);

    gdbStubPrvGetAndHandleCommands(stub);
#endif
}

void gdbStubReportMemAccess(struct stub *stub, uint32_t addr, uint_fast8_t sz, bool write) {
    int idx;

    if (stub->sock < 0) return;

#ifdef GDB_STUB_ENABLED
    if (stub->runState != RunstateRunning) return;

    idx = gdbStubPrvCheckWatchpoints(stub, addr, sz, write);

    if (idx < 0) return;

    // gdb expects the insruction to be aborted. it will then expect to remove the watchpoint, and
    // step over it. this means that we need to:
    //  1. reply to commands right here and now, before the access is executed
    //  2. once we get a step, continue to finish this instr and then stop
    // it makes no sence to stop here for debugger commands. we are mid-emulating-an-instruction
    // luckily, we know that so gdb will only do a mem read of the watched address (which
    // we have not yet accesed) and remove the watchpoint, and then step. This will cause this
    // command handling loop to end, and us to "Step" to next instr start, thus acting
    // like GDB on other platfrms - stoppping n inst just after the access

    sprintf(stub->stopReason, "T05%swatch:%08lx;",
            stub->wp[idx].read ? (stub->wp[idx].write ? "a" : "r") : "",
            (unsigned long)stub->wp[idx].addr);
    stub->runState = RunStateStopped;
    gdbStubPrvSendPacket(stub, stub->stopReason, false);
    gdbStubPrvGetAndHandleCommands(stub);
#endif
}

struct stub *gdbStubInit(struct ArmCpu *cpu, int port) {
    struct stub *stub = (struct stub *)malloc(sizeof(*stub));
    int sock, ret;

    if (!stub) ERR("cannot alloc GDBSTUB");

    memset(stub, 0, sizeof(*stub));
    stub->cpu = cpu;

    if (port < 0)
        stub->sock = -1;
    else {
#ifdef GDB_STUB_ENABLED
        struct sockaddr_in sa = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
        };
        socklen_t sl = sizeof(sa);

        inet_aton("0.0.0.0", (struct in_addr *)&sa.sin_addr.s_addr);

        sock = socket(PF_INET, SOCK_STREAM, 0);
        if (sock == -1) ERR("gdb socket creation fails: %d", errno);

        do {
            ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
        } while (ret == -1 && errno == EINTR);

        if (ret) ERR("gdb socket bind fails: %d", errno);

        do {
            ret = listen(sock, 1);
        } while (ret == -1 && errno == EINTR);

        if (ret) ERR("gdb socket listen fails: %d", errno);

        do {
            ret = accept(sock, (struct sockaddr *)&sa, &sl);
        } while (ret == -1 && errno == EINTR);
        if (ret == -1) ERR("gdb socket accept fails: %d", errno);
        close(sock);
        stub->sock = ret;

        stub->pktBufSz = 4096;
        stub->pktBuf = (char *)malloc(stub->pktBufSz);
        if (!stub->pktBuf) ERR("Command buffer alloc error");

        // start with a sanem stop reason
        strcpy(stub->stopReason, "S05");
#endif
    }
    return stub;
}
