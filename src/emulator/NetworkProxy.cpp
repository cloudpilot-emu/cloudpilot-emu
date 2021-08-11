#include "NetworkProxy.h"

#include <memory>

#include "EmMemory.h"
#include "EmSubroutine.h"
#include "Logging.h"
#include "Marshal.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendContextNetworkRpc.h"
#include "SuspendManager.h"
#include "pb_decode.h"
#include "pb_encode.h"

using namespace std::placeholders;

#define PREPARE_RESPONSE(call, submessage)                                                  \
    MsgResponse msgResponse;                                                                \
                                                                                            \
    if (!DecodeResponse(responseData, size, msgResponse, MsgResponse_##submessage##_tag)) { \
        logging::printf(#call ": bad response");                                            \
                                                                                            \
        return call##Fail();                                                                \
    }                                                                                       \
                                                                                            \
    const auto& response(msgResponse.payload.submessage);                                   \
                                                                                            \
    if (response.err != 0) {                                                                \
        logging::printf(#call ": failed, err = %u", response.err);                          \
                                                                                            \
        return call##Fail(response.err);                                                    \
    }

#define PREPARE_RESPONSE_WITH_BUFFER(call, submessage)                                   \
    MsgResponse msgResponse;                                                             \
    BufferDecodeContext bufferDecodeContext;                                             \
                                                                                         \
    if (!DecodeResponse(responseData, size, msgResponse, MsgResponse_##submessage##_tag, \
                        &bufferDecodeContext)) {                                         \
        logging::printf(#call ": bad response");                                         \
                                                                                         \
        return call##Fail();                                                             \
    }                                                                                    \
                                                                                         \
    const auto& response(msgResponse.payload.submessage);                                \
                                                                                         \
    if (response.err != 0) {                                                             \
        logging::printf(#call ": failed, err = %u", response.err);                       \
                                                                                         \
        return call##Fail(response.err);                                                 \
    }

struct BufferDecodeContext {
    unique_ptr<uint8[]> data;
    size_t len;
};

struct BufferEncodeContext {
    uint8* data;
    size_t len;
};

namespace {
    enum class SocketOptionType { flagval, intval, bufval };

    constexpr size_t REQUEST_STATIC_SIZE = 128;

    NetworkProxy networkProxy;

    bool serializeAddress(const NetSocketAddrType* sockAddr, Address& address) {
        if (sockAddr->family != netSocketAddrINET) return false;

        const NetSocketAddrINType* sockAddrIN = (const NetSocketAddrINType*)(sockAddr);

        address.port = ntohs(sockAddrIN->port);
        address.ip = ntohl(sockAddrIN->addr);

        return true;
    }

    void deserializeAddress(NetSocketAddrType* sockAddr, const Address& address) {
        NetSocketAddrINType* sockAddrIN = reinterpret_cast<NetSocketAddrINType*>(sockAddr);

        sockAddrIN->family = netSocketAddrINET;
        sockAddrIN->port = htons(address.port);
        sockAddrIN->addr = htonl(address.ip);
    }

    bool bufferEncodeCb(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg) {
        if (!arg) return false;

        const BufferEncodeContext* ctx = (const BufferEncodeContext*)(*arg);

        pb_encode_tag_for_field(stream, field);

        return pb_encode_string(stream, ctx->data, ctx->len);
    }

    bool bufferDecodeCb(pb_istream_t* stream, const pb_field_iter_t* field, void** arg) {
        if (!arg) return false;

        BufferDecodeContext* ctx = (BufferDecodeContext*)(*arg);

        ctx->len = stream->bytes_left;
        ctx->data = make_unique<uint8[]>(stream->bytes_left);

        return pb_read(stream, ctx->data.get(), stream->bytes_left);

        return true;
    }

    bool setupPayloadDecodeCb(pb_istream_t* stream, const pb_field_iter_t* field, void** arg) {
        if (!arg) return false;

        if (field->tag == MsgResponse_socketReceiveResponse_tag) {
            MsgSocketReceiveResponse* receiveResponse = (MsgSocketReceiveResponse*)field->pData;

            receiveResponse->data.arg = *arg;
            receiveResponse->data.funcs.decode = bufferDecodeCb;
        }

        return true;
    }

    int32 convertTimeout(int32 timeout) {
        return timeout > 0 ? timeout * 10 : timeout;  // milliseconds, assume 100 ticks per second
    }

    int32 countFDs(int32 fdSet) {
        int32 result = 0;

        for (int i = 0; i < 32; i++)
            if (fdSet & (1 << i)) result++;

        return result;
    }

    SocketOptionType determineSocketOptionType(uint16 level, uint16 option) {
        if (level == netSocketOptLevelIP) {
            return SocketOptionType::bufval;
        }

        if (level == netSocketOptLevelTCP) {
            return option == netSocketOptTCPNoDelay ? SocketOptionType::flagval
                                                    : SocketOptionType::intval;
        }

        switch (option) {
            case netSocketOptSockLinger:
            case netSocketOptSockSndBufSize:
            case netSocketOptSockRcvBufSize:
            case netSocketOptSockSndLowWater:
            case netSocketOptSockRcvLowWater:
            case netSocketOptSockSndTimeout:
            case netSocketOptSockRcvTimeout:
            case netSocketOptSockErrorStatus:
            case netSocketOptSockSocketType:
                return SocketOptionType::intval;

            default:
                return SocketOptionType::flagval;
        }
    }
}  // namespace

NetworkProxy& gNetworkProxy{networkProxy};

void NetworkProxy::Reset() {
    if (this->openCount > 0) onDisconnect.Dispatch(sessionId.c_str());

    openCount = 0;
}

void NetworkProxy::Open() {
    if (openCount > 0) {
        CALLED_SETUP("Err", "void");
        PUT_RESULT_VAL(Err, 0);

        return;
    }

    SuspendManager::Suspend<SuspendContextNetworkConnect>(
        bind(&NetworkProxy::ConnectSuccess, this, _1), bind(&NetworkProxy::ConnectAbort, this));
}

void NetworkProxy::ConnectSuccess(const string& sessionId) {
    this->sessionId = sessionId;
    openCount++;

    CALLED_SETUP("Err", "void");
    PUT_RESULT_VAL(Err, 0);
}

void NetworkProxy::ConnectAbort() {
    CALLED_SETUP("Err", "void");
    PUT_RESULT_VAL(Err, netErrInternal);
}

void NetworkProxy::Close() {
    if (openCount == 0) return CloseDone(netErrNotOpen);

    if (--openCount == 0) this->onDisconnect.Dispatch(sessionId.c_str());
    return CloseDone(0);

    CloseDone(netErrStillOpen);
}

void NetworkProxy::CloseDone(Err err) {
    CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 immediate");

    PUT_RESULT_VAL(Err, err);
}

int NetworkProxy::OpenCount() { return openCount; }

void NetworkProxy::SocketOpen(uint8 domain, uint8 type, uint16 protocol) {
    if (domain != netSocketAddrINET) {
        logging::printf("raw sockets are unsupporrted");
        return SocketOpenFail(netErrParamErr);
    }

    MsgRequest request = NewRequest(MsgRequest_socketOpenRequest_tag);

    request.payload.socketOpenRequest.type = type;
    request.payload.socketOpenRequest.protocol = protocol;

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketOpenSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketOpenFail, this, _1));
}

void NetworkProxy::SocketOpenSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(SocketOpen, socketOpenResponse)
    CALLED_SETUP("NetSocketRef",
                 "UInt16 libRefNum, NetSocketAddrEnum domain, "
                 "NetSocketTypeEnum type, Int16 protocol, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, static_cast<Int16>(response.handle));
}

void NetworkProxy::SocketOpenFail(Err err) {
    CALLED_SETUP("NetSocketRef",
                 "UInt16 libRefNum, NetSocketAddrEnum domain, "
                 "NetSocketTypeEnum type, Int16 protocol, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

void NetworkProxy::SocketBind(int16 handle, NetSocketAddrType* sockAddrP, Int32 timeout) {
    MsgRequest request = NewRequest(MsgRequest_socketBindRequest_tag);

    request.payload.socketBindRequest.handle = handle;
    request.payload.socketBindRequest.timeout = timeout;

    if (!serializeAddress(sockAddrP, request.payload.socketBindRequest.address))
        return SocketBindFail(netErrParamErr);

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketBindSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketBindFail, this, _1));
}

void NetworkProxy::SocketBindSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(SocketBind, socketBindResponse)

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "NetSocketAddrType *sockAddrP, Int16 addrLen, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, 0);
}

void NetworkProxy::SocketBindFail(Err err) {
    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "NetSocketAddrType *sockAddrP, Int16 addrLen, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

void NetworkProxy::SocketAddr(int16 handle, NetSocketAddrType* locAddrP, Int16* locAddrLenP,
                              NetSocketAddrType* remAddrP, Int16* remAddrLenP, int32 timeout) {
    MsgRequest request = NewRequest(MsgRequest_socketAddrRequest_tag);
    request.payload.socketAddrRequest.handle = handle;

    if ((locAddrP && *locAddrLenP < 8) || (remAddrP && *remAddrLenP < 8)) {
        return SocketBindFail(netErrParamErr);
    }

    request.payload.socketAddrRequest.requestAddressLocal = locAddrP;
    request.payload.socketAddrRequest.requestAddressRemote = remAddrP;
    request.payload.socketAddrRequest.timeout = convertTimeout(timeout);

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketAddrSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketAddrFail, this, _1));
}

void NetworkProxy::SocketAddrSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(SocketAddr, socketAddrResponse)

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "NetSocketAddrType *locAddrP, Int16 *locAddrLenP, "
                 "NetSocketAddrType *remAddrP, Int16 *remAddrLenP, "
                 "Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(NetSocketAddrType, locAddrP, Marshal::kOutput);
    CALLED_GET_PARAM_REF(Int16, locAddrLenP, Marshal::kInOut);
    CALLED_GET_PARAM_REF(NetSocketAddrType, remAddrP, Marshal::kOutput);
    CALLED_GET_PARAM_REF(Int16, remAddrLenP, Marshal::kInOut);
    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    if ((locAddrP && *locAddrLenP < 8) || (remAddrP && *remAddrLenP < 8)) {
        return SocketBindFail(netErrParamErr);
    }

    if (locAddrP) {
        deserializeAddress(locAddrP, response.addressLocal);
        *locAddrLenP = 8;

        CALLED_PUT_PARAM_REF(locAddrP);
        CALLED_PUT_PARAM_REF(locAddrLenP);
    }

    if (remAddrP) {
        deserializeAddress(remAddrP, response.addressRemote);

        *remAddrLenP = 8;
        CALLED_PUT_PARAM_REF(remAddrP);
        CALLED_PUT_PARAM_REF(remAddrLenP);
    }

    *errP = 0;

    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, 0);
}

void NetworkProxy::SocketAddrFail(Err err) {
    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "NetSocketAddrType *locAddrP, Int16 *locAddrLenP, "
                 "NetSocketAddrType *remAddrP, Int16 *remAddrLenP, "
                 "Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

void NetworkProxy::SocketSend(int16 handle, uint8* data, size_t count, uint32 flags,
                              NetSocketAddrType* toAddrP, int32 toLen, int32 timeout) {
    MsgRequest request = NewRequest(MsgRequest_socketSendRequest_tag);
    MsgSocketSendRequest& sendRequest(request.payload.socketSendRequest);

    sendRequest.handle = handle;

    if (flags & ~(netIOFlagOutOfBand | netIOFlagPeek | netIOFlagDontRoute)) {
        logging::printf("ERROR: SocketSend: unsupported flags 0x%08x", flags);

        return SocketSendFail();
    }

    sendRequest.flags = flags;

    if (toAddrP) {
        if (toLen < 8 || !serializeAddress(toAddrP, request.payload.socketSendRequest.address))
            return SocketSendFail(netErrParamErr);

        sendRequest.has_address = true;
    } else
        sendRequest.has_address = false;

    sendRequest.timeout = convertTimeout(timeout);

    BufferEncodeContext bufferEncodeCtx{data, count};

    sendRequest.data.arg = &bufferEncodeCtx;
    sendRequest.data.funcs.encode = bufferEncodeCb;

    SendAndSuspend(request, REQUEST_STATIC_SIZE + count,
                   bind(&NetworkProxy::SocketSendSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketSendFail, this, _1));
}

void NetworkProxy::SocketSendSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(SocketSend, socketSendResponse)

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "void *bufP, UInt16 bufLen, UInt16 flags,"
                 "void *toAddrP, UInt16 toLen, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0, CALLED_PUT_PARAM_REF(errP);
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, response.bytesSent);
}

void NetworkProxy::SocketSendFail(Err err) {
    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "void *bufP, UInt16 bufLen, UInt16 flags,"
                 "void *toAddrP, UInt16 toLen, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

void NetworkProxy::SocketReceive(int16 handle, uint32 flags, uint16 bufLen, int32 timeout,
                                 NetSocketAddrType* fromAddrP) {
    MsgRequest request = NewRequest(MsgRequest_socketReceiveRequest_tag);

    if (flags & ~(netIOFlagOutOfBand | netIOFlagPeek | netIOFlagDontRoute)) {
        logging::printf("ERROR: SocketReceive: unsupported flags 0x%08x", flags);

        return SocketReceiveFail();
    }

    request.payload.socketReceiveRequest.handle = handle;
    request.payload.socketReceiveRequest.flags = flags;
    request.payload.socketReceiveRequest.timeout = convertTimeout(timeout);
    request.payload.socketReceiveRequest.maxLen = bufLen;
    request.payload.socketReceiveRequest.addressRequested = fromAddrP;

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketReceiveSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketReceiveFail, this, _1));
}

void NetworkProxy::SocketReceiveSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE_WITH_BUFFER(SocketReceive, socketReceiveResponse)

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "void *bufP, UInt16 bufLen, UInt16 flags, "
                 "void *fromAddrP, UInt16 *fromLenP, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_VAL(UInt16, bufLen);
    CALLED_GET_PARAM_VAL(UInt32, bufP);
    CALLED_GET_PARAM_REF(NetSocketAddrType, fromAddrP, Marshal::kOutput);
    CALLED_GET_PARAM_REF(UInt16, fromLenP, Marshal::kInOut);
    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    if (bufferDecodeContext.len > bufLen) {
        logging::printf("SocketReceive: message too long: %u vs. %u", bufferDecodeContext.len,
                        bufLen);

        return SocketReceiveFail();
    }

    if (fromAddrP && fromLenP < 8) return SocketReceiveFail(netErrParamErr);

    EmMem_memcpy((emuptr)bufP, static_cast<void*>(bufferDecodeContext.data.get()),
                 bufferDecodeContext.len);

    if (fromAddrP && response.has_address) {
        deserializeAddress(fromAddrP, response.address);
        *fromLenP = 8;

        CALLED_PUT_PARAM_REF(fromAddrP);
        CALLED_PUT_PARAM_REF(fromLenP);
    }

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, bufferDecodeContext.len);
}

void NetworkProxy::SocketReceiveFail(Err err) {
    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "void *bufP, UInt16 bufLen, UInt16 flags, "
                 "void *fromAddrP, UInt16 *fromLenP, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

void NetworkProxy::SocketDmReceive(int16 handle, uint32 flags, uint16 rcvlen, int32 timeout,
                                   NetSocketAddrType* fromAddrP) {
    MsgRequest request = NewRequest(MsgRequest_socketReceiveRequest_tag);

    if (flags & ~(netIOFlagOutOfBand | netIOFlagPeek | netIOFlagDontRoute)) {
        logging::printf("ERROR: SocketDmReceive: unsupported flags 0x%08x", flags);

        return SocketDmReceiveFail();
    }

    request.payload.socketReceiveRequest.handle = handle;
    request.payload.socketReceiveRequest.flags = flags;
    request.payload.socketReceiveRequest.timeout = convertTimeout(timeout);
    request.payload.socketReceiveRequest.maxLen = rcvlen;
    request.payload.socketReceiveRequest.addressRequested = fromAddrP;

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketDmReceiveSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketDmReceiveFail, this, _1));
}

void NetworkProxy::SocketDmReceiveSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE_WITH_BUFFER(SocketDmReceive, socketReceiveResponse)

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "void *recordP, UInt32 recordOffset, UInt16 rcvLen, UInt16 flags, "
                 "void *fromAddrP, UInt16 *fromLenP, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_VAL(UInt32, recordP);
    CALLED_GET_PARAM_VAL(UInt32, recordOffset);
    CALLED_GET_PARAM_VAL(UInt16, rcvLen);
    CALLED_GET_PARAM_REF(NetSocketAddrType, fromAddrP, Marshal::kOutput);
    CALLED_GET_PARAM_REF(UInt16, fromLenP, Marshal::kOutput);
    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    if (bufferDecodeContext.len > rcvLen) {
        logging::printf("SocketDmReceive: message too long: %u vs. %u", bufferDecodeContext.len,
                        rcvLen);

        return SocketDmReceiveFail();
    }

    if (fromAddrP && fromLenP < 8) return SocketDmReceiveFail(netErrParamErr);

    CEnableFullAccess munge;

    EmMem_memcpy((emuptr)(recordP + recordOffset),
                 static_cast<void*>(bufferDecodeContext.data.get()), bufferDecodeContext.len);

    if (fromAddrP && response.has_address) {
        deserializeAddress(fromAddrP, response.address);
        *fromLenP = 8;

        CALLED_PUT_PARAM_REF(fromAddrP);
        CALLED_PUT_PARAM_REF(fromLenP);
    }

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, bufferDecodeContext.len);
}

void NetworkProxy::SocketDmReceiveFail(Err err) {
    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "void *recordP, UInt32 recordOffset, UInt16 rcvLen, UInt16 flags, "
                 "void *fromAddrP, UInt16 *fromLenP, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

void NetworkProxy::SocketClose(int16 handle, int32 timeout) {
    MsgRequest request = NewRequest(MsgRequest_socketCloseRequest_tag);

    request.payload.socketCloseRequest.handle = handle;
    request.payload.socketCloseRequest.timeout = convertTimeout(timeout);

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketCloseSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketCloseFail, this, _1));
}

void NetworkProxy::SocketCloseSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(SocketClose, socketCloseResponse)

    CALLED_SETUP("Int16", "UInt16 libRefNum, NetSocketRef socket, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, 0);
}

void NetworkProxy::SocketCloseFail(Err err) {
    CALLED_SETUP("Int16", "UInt16 libRefNum, NetSocketRef socket, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

void NetworkProxy::GetHostByName(const string name) {
    MsgRequest requestMsg = NewRequest(MsgRequest_getHostByNameRequest_tag);
    MsgGetHostByNameRequest& request(requestMsg.payload.getHostByNameRequest);

    if (name.length() >= sizeof(request.name)) return GetHostByNameFail(netErrParamErr);

    strcpy(request.name, name.c_str());

    SendAndSuspend(requestMsg, REQUEST_STATIC_SIZE + sizeof(request.name),
                   bind(&NetworkProxy::GetHostByNameSuccess, this, _1, _2),
                   bind(&NetworkProxy::GetHostByNameFail, this, _1));
}

void NetworkProxy::GetHostByNameSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(GetHostByName, getHostByNameResponse)

    if (response.addresses_count > 3) {
        return GetHostByNameFail();
    }

    CALLED_SETUP("NetHostInfoPtr",
                 "UInt16 libRefNum, Char *nameP, "
                 "NetHostInfoBufPtr bufP, Int32	timeout, Err *errP");

    CALLED_GET_PARAM_REF(NetHostInfoBufType, bufP, Marshal::kOutput);
    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    strncpy((*bufP).name, response.name, 255);
    (*bufP).name[255] = 0;

    if (response.has_alias) {
        strncpy((*bufP).aliases[0], response.alias, 255);
        (*bufP).aliases[0][255] = 0;

        (*bufP).aliasList[0] = (*bufP).aliases[0];
        (*bufP).aliasList[1] = nullptr;
    } else
        (*bufP).aliasList[0] = nullptr;

    for (int i = 0; i < response.addresses_count; i++) {
        (*bufP).address[i] = ntohl(response.addresses[i]);
        (*bufP).addressList[i] = &(*bufP).address[i];
    }

    (*bufP).addressList[response.addresses_count] = nullptr;

    (*bufP).hostInfo.nameP = (*bufP).name;
    (*bufP).hostInfo.nameAliasesP = (*bufP).aliasList;
    (*bufP).hostInfo.addrListP = reinterpret_cast<uint8**>((*bufP).addressList);
    (*bufP).hostInfo.addrType = netSocketAddrINET;
    (*bufP).hostInfo.addrLen = 4;

    *errP = 0;

    CALLED_PUT_PARAM_REF(errP);
    CALLED_PUT_PARAM_REF(bufP);
    PUT_RESULT_VAL(emuptr, (emuptr)bufP);
}

void NetworkProxy::GetHostByNameFail(Err err) {
    CALLED_SETUP("NetHostInfoPtr",
                 "UInt16 libRefNum, Char *nameP, "
                 "NetHostInfoBufPtr bufP, Int32	timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(emuptr, 0);
}

void NetworkProxy::GetServByName(const string name, const string proto) {
    MsgRequest requestMsg = NewRequest(MsgRequest_getServByNameRequest_tag);
    MsgGetServByNameRequest& request(requestMsg.payload.getServByNameRequest);

    if (name.length() >= sizeof(request.name) - 1 || proto.length() >= sizeof(request.protocol))
        return GetHostByNameFail(netErrParamErr);

    strcpy(request.name, name.c_str());
    strcpy(request.protocol, proto.c_str());

    SendAndSuspend(requestMsg,
                   REQUEST_STATIC_SIZE + sizeof(request.name) + sizeof(request.protocol),
                   bind(&NetworkProxy::GetServByNameSuccess, this, _1, _2),
                   bind(&NetworkProxy::GetServByNameFail, this, _1));
}

void NetworkProxy::GetServByNameSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(GetServByName, getServByNameResponse)

    CALLED_SETUP("NetServInfoPtr",
                 "UInt16 libRefNum, const Char *servNameP, "
                 "const Char *protoNameP,  NetServInfoBufPtr bufP, "
                 "Int32	timeout, Err *errP");

    CALLED_GET_PARAM_STR(Char, servNameP);
    CALLED_GET_PARAM_STR(Char, protoNameP);
    CALLED_GET_PARAM_REF(NetServInfoBufType, bufP, Marshal::kOutput);
    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    if (strlen(servNameP) > netServMaxName || strlen(protoNameP) > netProtoMaxName)
        return GetServByNameFail(netErrParamErr);

    strcpy((*bufP).name, servNameP);
    strcpy((*bufP).protoName, protoNameP);

    (*bufP).aliasList[0] = nullptr;

    (*bufP).servInfo.nameP = (*bufP).name;
    (*bufP).servInfo.protoP = (*bufP).protoName;
    (*bufP).servInfo.nameAliasesP = (*bufP).aliasList;
    (*bufP).servInfo.port = htons(response.port);

    CALLED_PUT_PARAM_REF(bufP);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(emuptr, (emuptr)bufP);
}

void NetworkProxy::GetServByNameFail(Err err) {
    CALLED_SETUP("NetServInfoPtr",
                 "UInt16 libRefNum, const Char *servNameP, "
                 "const Char *protoNameP,  NetServInfoBufPtr bufP, "
                 "Int32	timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;

    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(emuptr, 0);
}

void NetworkProxy::SocketConnect(int16 handle, NetSocketAddrType* address, int16 addrLen,
                                 int32 timeout) {
    if (addrLen < 8) return SocketConnectFail(netErrParamErr);

    MsgRequest msgRequest = NewRequest(MsgRequest_socketConnectRequest_tag);
    MsgSocketConnectRequest& request(msgRequest.payload.socketConnectRequest);

    if (!serializeAddress(address, request.address)) return SocketConnectFail(netErrParamErr);

    request.handle = handle;
    request.timeout = timeout;

    SendAndSuspend(msgRequest, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketConnectSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketConnectFail, this, _1));
}

void NetworkProxy::SocketConnectSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(SocketConnect, socketConnectResponse)

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "NetSocketAddrType *sockAddrP, Int16 addrLen, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, 0);
}

void NetworkProxy::SocketConnectFail(Err err) {
    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "NetSocketAddrType *sockAddrP, Int16 addrLen, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

void NetworkProxy::Select(UInt16 width, NetFDSetType readFDs, NetFDSetType writeFDs,
                          NetFDSetType exceptFDs, int32 timeout) {
    if (width > 32) width = 32;

    MsgRequest msgRequest = NewRequest(MsgRequest_selectRequest_tag);
    MsgSelectRequest& request(msgRequest.payload.selectRequest);

    request.width = width;
    request.readFDs = readFDs;
    request.writeFDs = writeFDs;
    request.exceptFDs = exceptFDs;
    request.timeout = convertTimeout(timeout);

    SendAndSuspend(msgRequest, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SelectSuccess, this, _1, _2),
                   bind(&NetworkProxy::SelectFail, this, _1));
}

void NetworkProxy::SelectSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(Select, selectResponse)

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, UInt16 width, NetFDSetType *readFDs, "
                 "NetFDSetType *writeFDs, NetFDSetType *exceptFDs,"
                 "Int32	timeout, Err *errP");

    CALLED_GET_PARAM_REF(NetFDSetType, readFDs, Marshal::kOutput);
    CALLED_GET_PARAM_REF(NetFDSetType, writeFDs, Marshal::kOutput);
    CALLED_GET_PARAM_REF(NetFDSetType, exceptFDs, Marshal::kOutput);
    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *readFDs = response.readFDs;
    *writeFDs = response.writeFDs;
    *exceptFDs = response.exceptFDs;
    *errP = 0;

    CALLED_PUT_PARAM_REF(readFDs);
    CALLED_PUT_PARAM_REF(writeFDs);
    CALLED_PUT_PARAM_REF(exceptFDs);
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16,
                   static_cast<Int16>(countFDs(response.readFDs) + countFDs(response.writeFDs) +
                                      countFDs(response.exceptFDs)));
}

void NetworkProxy::SelectFail(Err err) {
    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, UInt16 width, NetFDSetType *readFDs, "
                 "NetFDSetType *writeFDs, NetFDSetType *exceptFDs,"
                 "Int32	timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

bool NetworkProxy::SettingGet(UInt16 setting) {
    switch (setting) {
        case netSettingHostName:
        case netSettingPrimaryDNS:
        case netSettingSecondaryDNS:
        case netSettingRTPrimaryDNS:
        case netSettingRTSecondaryDNS:
            break;

        default:
            return false;
    }

    MsgRequest msgRequest = NewRequest(MsgRequest_settingGetRequest_tag);
    MsgSettingGetRequest& request(msgRequest.payload.settingGetRequest);

    request.setting = setting;

    SendAndSuspend(msgRequest, REQUEST_STATIC_SIZE + 256,
                   bind(&NetworkProxy::SettingGetSuccess, this, _1, _2),
                   bind(&NetworkProxy::SettingGetFail, this, _1));

    return true;
}

void NetworkProxy::SettingGetSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(SettingGet, settingGetResponse);

    CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 setting, void *valueP, UInt16 *valueLenP");

    CALLED_GET_PARAM_REF(UInt16, valueLenP, Marshal::kInOut);
    CALLED_GET_PARAM_VAL(emuptr, valueP);

    switch (response.which_value) {
        case MsgSettingGetResponse_strval_tag: {
            size_t len = strnlen(response.value.strval, 256);

            if (len == 256) return SettingGetFail(netErrInternal);
            if (len >= *valueLenP) return SettingGetFail(netErrBufTooSmall);

            EmMem_strcpy(static_cast<emuptr>(valueP), response.value.strval);

            *valueLenP = len;

            break;
        }

        case MsgSettingGetResponse_uint32val_tag:
            if (*valueLenP < 4) return SettingGetFail(netErrBufTooSmall);

            EmMemPut32(valueP, response.value.uint32val);
            *valueLenP = 4;

            break;

        case MsgSettingGetResponse_uint8val_tag:
            if (*valueLenP == 0) return SettingGetFail(netErrBufTooSmall);

            EmMemPut8(valueP, response.value.uint8val);
            *valueLenP = 1;

            break;

        default:
            return SettingGetFail(netErrInternal);
    }

    CALLED_PUT_PARAM_REF(valueLenP);

    PUT_RESULT_VAL(Err, 0);
}

void NetworkProxy::SettingGetFail(Err err) {
    CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 setting, void *valueP, UInt16 *valueLenP");

    PUT_RESULT_VAL(Err, err);
}

void NetworkProxy::SocketOptionSet(int16 handle, uint16 level, uint16 option, emuptr valueP,
                                   size_t len) {
    if (level == netSocketOptLevelSocket &&
        (option == netSocketOptSockRequireErrClear || option == netSocketOptSockMultiPktAddr))
        return SocketOptionSetFail(netErrUnimplemented);

    MsgRequest msgRequest = NewRequest(MsgRequest_socketOptionSetRequest_tag);
    MsgSocketOptionSetRequest& request(msgRequest.payload.socketOptionSetRequest);

    request.handle = handle;
    request.level = level;
    request.option = option;

    if (level == netSocketOptLevelSocket && option == netSocketOptSockLinger) {
        request.which_value = MsgSocketOptionSetRequest_intval_tag;

        if (len != 4) return SocketOptionSetFail(netErrParamErr);
        uint16 onOff = EmMemGet16(valueP);
        uint16 time = EmMemGet16(valueP + 2);

        request.value.intval = onOff | (time << 16);
    } else {
        switch (determineSocketOptionType(level, option)) {
            case SocketOptionType::bufval:
                if (len > 40) return SocketOptionSetFail(netErrParamErr);

                request.which_value = MsgSocketOptionSetRequest_bufval_tag;
                EmMem_memcpy(static_cast<void*>(request.value.bufval.bytes), valueP, len);
                request.value.bufval.size = len;

                break;

            case SocketOptionType::flagval:
                if (len != 1 && len != 2 && len != 4) return SocketOptionSetFail(netErrParamErr);

                request.which_value = MsgSocketOptionSetRequest_boolval_tag;
                request.value.boolval = EmMemGet8(valueP);

                break;

            case SocketOptionType::intval:
                switch (len) {
                    case 2:
                        request.value.intval = EmMemGet16(valueP);
                        break;

                    case 4:
                        request.value.intval = EmMemGet32(valueP);
                        break;

                    default:
                        return SocketOptionSetFail(netErrParamErr);
                }

                request.which_value = MsgSocketOptionSetRequest_intval_tag;

                break;
        }
    }

    SendAndSuspend(msgRequest, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketOptionSetSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketOptionSetFail, this, _1));
}

void NetworkProxy::SocketOptionSetSuccess(void* responseData, size_t size) {
    PREPARE_RESPONSE(SocketOptionSet, socketOptionSetResponse);

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "UInt16 level, UInt16 option, "
                 "void *optValueP, UInt16 optValueLen,"
                 "Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(int16, 0);
}

void NetworkProxy::SocketOptionSetFail(Err err) {
    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "UInt16 level, UInt16 option, "
                 "void *optValueP, UInt16 optValueLen,"
                 "Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = err;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}

bool NetworkProxy::DecodeResponse(void* responseData, size_t size, MsgResponse& response,
                                  pb_size_t payloadTag, BufferDecodeContext* bufferrDecodeContext) {
    response = MsgResponse_init_zero;

    if (bufferrDecodeContext) {
        response.cb_payload.arg = bufferrDecodeContext;
        response.cb_payload.funcs.decode = setupPayloadDecodeCb;
    }

    pb_istream_t stream = pb_istream_from_buffer(static_cast<uint8*>(responseData), size);
    bool status = pb_decode(&stream, MsgResponse_fields, &response);

    if (!status) {
        logging::printf("failed to decode response");

        return false;
    }

    if (response.id != currentId) {
        logging::printf("response out of order");
        return false;
    }

    if (response.which_payload != payloadTag) {
        logging::printf("tag mismatch");
        return false;
    }

    return true;
}

MsgRequest NetworkProxy::NewRequest(pb_size_t payloadTag) {
    MsgRequest request = MsgRequest_init_zero;

    request.id = ++currentId;
    request.which_payload = payloadTag;

    return request;
}

void NetworkProxy::SendAndSuspend(MsgRequest& request, size_t size,
                                  SuspendContextNetworkRpc::successCallbackT cbSuccess,
                                  function<void(Err)> cbFail) {
    if (openCount == 0) return cbFail(netErrNotOpen);

    uint8* buffer = new uint8[size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, size);

    pb_encode(&stream, MsgRequest_fields, &request);

    SuspendManager::Suspend<SuspendContextNetworkRpc>(buffer, stream.bytes_written, cbSuccess,
                                                      bind(cbFail, netErrInternal));
}
