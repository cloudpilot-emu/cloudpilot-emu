#include "NetworkProxy.h"

#include <memory>

#include "EmMemory.h"
#include "EmSubroutine.h"
#include "Logging.h"
#include "Marshal.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendContextNetworkDisconnect.h"
#include "SuspendContextNetworkRpc.h"
#include "SuspendManager.h"
#include "pb_decode.h"
#include "pb_encode.h"

using namespace std::placeholders;

struct BufferDecodeContext {
    unique_ptr<uint8[]> data;
    size_t len;
};

struct BufferEncodeContext {
    uint8* data;
    size_t len;
};

namespace {
    constexpr size_t REQUEST_STATIC_SIZE = 128;

    NetworkProxy networkProxy;

    uint32 getOptionValue(uint32 address, uint16 len) {
        if (address == 0) return 0;

        switch (len) {
            case 1:
                return EmMemGet8(address);

            case 2:
                return EmMemGet16(address);

            case 4:
                return EmMemGet32(address);

            default:
                return 0;
        }
    }

    bool serializeAddress(const NetSocketAddrType* sockAddr, Address& address) {
        if (sockAddr->family != netSocketAddrINET) return false;

        const NetSocketAddrINType* sockAddrIN = (const NetSocketAddrINType*)(sockAddr);

        address.port = sockAddrIN->port;
        address.ip = ntohl(sockAddrIN->addr);

        return true;
    }

    void deserializeAddress(NetSocketAddrType* sockAddr, const Address& address) {
        NetSocketAddrINType* sockAddrIN = reinterpret_cast<NetSocketAddrINType*>(sockAddr);

        sockAddrIN->family = netSocketAddrINET;
        sockAddrIN->port = address.port;
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
}  // namespace

NetworkProxy& gNetworkProxy{networkProxy};

void NetworkProxy::Open() {
    if (openCount > 0) return ConnectSuccess();

    SuspendManager::Suspend<SuspendContextNetworkConnect>(bind(&NetworkProxy::ConnectSuccess, this),
                                                          bind(&NetworkProxy::ConnectAbort, this));
}

void NetworkProxy::Close() {
    if (openCount == 0) return CloseDone(netErrNotOpen);

    if (--openCount == 0)
        SuspendManager::Suspend<SuspendContextNetworkDisconnect>(
            bind(&NetworkProxy::CloseDone, this, 0));
    else
        CloseDone(netErrStillOpen);
}

void NetworkProxy::CloseDone(Err err) {
    CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 immediate");

    PUT_RESULT_VAL(Err, err);
}

void NetworkProxy::ConnectSuccess() {
    CALLED_SETUP("Err", "void");
    PUT_RESULT_VAL(Err, 0);

    openCount++;
}

void NetworkProxy::ConnectAbort() {
    CALLED_SETUP("Err", "void");
    PUT_RESULT_VAL(Err, netErrInternal);
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

void NetworkProxy::SocketOpenSuccess(uint8* responseData, size_t size) {
    MsgResponse response;

    if (!DecodeResponse(responseData, size, response, MsgResponse_socketOpenResponse_tag)) {
        logging::printf("SocketOpen: bad response");

        return SocketOpenFail();
    }

    if (response.payload.socketOpenResponse.handle < 0) {
        logging::printf("SocketOpen: failed");

        return SocketOpenFail(response.payload.socketOpenResponse.err);
    }

    CALLED_SETUP("NetSocketRef",
                 "UInt16 libRefNum, NetSocketAddrEnum domain, "
                 "NetSocketTypeEnum type, Int16 protocol, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, static_cast<Int16>(response.payload.socketOpenResponse.handle));
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

void NetworkProxy::SocketBind(int16 handle, NetSocketAddrType* sockAddrP) {
    MsgRequest request = NewRequest(MsgRequest_socketBindRequest_tag);

    request.payload.socketBindRequest.handle = handle;

    if (!serializeAddress(sockAddrP, request.payload.socketBindRequest.address))
        return SocketBindFail(netErrParamErr);

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketBindSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketBindFail, this, _1));
}

void NetworkProxy::SocketBindSuccess(uint8* responseData, size_t size) {
    MsgResponse response;

    if (!DecodeResponse(responseData, size, response, MsgResponse_socketBindResponse_tag)) {
        logging::printf("SocketBind: bad response");

        return SocketBindFail();
    }

    if (response.payload.socketBindResponse.err != 0) {
        logging::printf("SocketBind: failed");

        return SocketBindFail(response.payload.socketBindResponse.err);
    }

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
                              NetSocketAddrType* remAddrP, Int16* remAddrLenP) {
    MsgRequest request = NewRequest(MsgRequest_socketAddrRequest_tag);
    request.payload.socketAddrRequest.handle = handle;

    if ((locAddrP && *locAddrLenP < 8) || (remAddrP && *remAddrLenP < 8)) {
        return SocketBindFail(netErrParamErr);
    }

    request.payload.socketAddrRequest.requestAddressLocal = locAddrP;
    request.payload.socketAddrRequest.requestAddressRemote = remAddrP;

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketAddrSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketAddrFail, this, _1));
}

void NetworkProxy::SocketAddrSuccess(uint8* responseData, size_t size) {
    MsgResponse response;

    if (!DecodeResponse(responseData, size, response, MsgResponse_socketAddrResponse_tag)) {
        logging::printf("SocketAddr: bad response");

        return SocketBindFail();
    }

    if (response.payload.socketAddrResponse.err != 0) {
        logging::printf("SocketAddr: failed");

        return SocketBindFail(response.payload.socketAddrResponse.err);
    }

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
        deserializeAddress(locAddrP, response.payload.socketAddrResponse.addressLocal);
        *locAddrLenP = 8;

        CALLED_PUT_PARAM_REF(locAddrP);
        CALLED_PUT_PARAM_REF(locAddrLenP);
    }

    if (remAddrP) {
        deserializeAddress(remAddrP, response.payload.socketAddrResponse.addressRemote);

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

void NetworkProxy::SocketOptionSet(int16 handle, uint16 level, uint16 option, uint32 valueP,
                                   uint16 valueLen) {
    if (!valueP || (valueLen != 1 && valueLen != 2 && valueLen != 4))
        return SocketOptionSetFail(netErrParamErr);

    uint32 value = getOptionValue(valueP, valueLen);

    logging::printf(
        "WARNING: unsupported NetLibSocketOptionSet, level=0x%04x option=0x%04x value=%u", level,
        option, value);

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "UInt16 level, UInt16 option, "
                 "void *optValueP, UInt16 optValueLen,"
                 "Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, 0);
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

    sendRequest.timeout =
        timeout > 0 ? timeout * 10 : timeout;  // milliseconds, assume 100 ticks per second

    BufferEncodeContext bufferEncodeCtx{data, count};

    sendRequest.data.arg = &bufferEncodeCtx;
    sendRequest.data.funcs.encode = bufferEncodeCb;

    SendAndSuspend(request, REQUEST_STATIC_SIZE + count,
                   bind(&NetworkProxy::SocketSendSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketSendFail, this, _1));
}

void NetworkProxy::SocketSendSuccess(uint8* responseData, size_t size) {
    MsgResponse response;

    if (!DecodeResponse(responseData, size, response, MsgResponse_socketSendResponse_tag)) {
        logging::printf("SocketSend: bad response");

        return SocketSendFail();
    }

    if (response.payload.socketSendResponse.err != 0) {
        logging::printf("SocketSend: failed");

        return SocketSendFail(response.payload.socketSendResponse.err);
    }

    CALLED_SETUP("Int16",
                 "UInt16 libRefNum, NetSocketRef socket,"
                 "void *bufP, UInt16 bufLen, UInt16 flags,"
                 "void *toAddrP, UInt16 toLen, Int32 timeout, Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0, CALLED_PUT_PARAM_REF(errP);
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, response.payload.socketSendResponse.bytesSent);
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

void NetworkProxy::SocketReceive(int16 handle, uint32 flags, uint16 bufLen, int32 timeout) {
    MsgRequest request = NewRequest(MsgRequest_socketReceiveRequest_tag);

    if (flags) {
        logging::printf("ERROR: SocketReceive: unsupported flags 0x%08x", flags);

        return SocketReceiveFail();
    }

    request.payload.socketReceiveRequest.handle = handle;
    request.payload.socketReceiveRequest.flags = flags;
    request.payload.socketReceiveRequest.timeout = timeout;
    request.payload.socketReceiveRequest.maxLen = bufLen;

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketReceiveSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketReceiveFail, this, _1));
}

void NetworkProxy::SocketReceiveSuccess(uint8* responseData, size_t size) {
    MsgResponse response;
    BufferDecodeContext bufferDecodeContext;

    if (!DecodeResponse(responseData, size, response, MsgResponse_socketReceiveResponse_tag,
                        &bufferDecodeContext)) {
        logging::printf("SocketReceive: bad response");

        return SocketReceiveFail();
    }

    if (response.payload.socketReceiveResponse.err != 0) {
        logging::printf("SocketReceive: failed");

        return SocketReceiveFail(response.payload.socketReceiveResponse.err);
    }

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

    if (fromAddrP) {
        deserializeAddress(fromAddrP, response.payload.socketReceiveResponse.address);
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

void NetworkProxy::SocketClose(int16 handle) {
    MsgRequest request = NewRequest(MsgRequest_socketCloseRequest_tag);

    request.payload.socketCloseRequest.handle = handle;

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketCloseSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketCloseFail, this, _1));
}

void NetworkProxy::SocketCloseSuccess(uint8* responseData, size_t size) {
    MsgResponse response;
    BufferDecodeContext bufferDecodeContext;

    if (!DecodeResponse(responseData, size, response, MsgResponse_socketCloseResponse_tag,
                        &bufferDecodeContext)) {
        logging::printf("SocketClose: bad response");

        return SocketCloseFail();
    }

    if (response.payload.socketCloseResponse.err != 0) {
        logging::printf("SocketClose: failed");

        return SocketReceiveFail(response.payload.socketReceiveResponse.err);
    }

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

void NetworkProxy::GetHostByNameSuccess(uint8* responseData, size_t size) {
    MsgResponse msgResponse;

    if (!DecodeResponse(responseData, size, msgResponse, MsgResponse_getHostByNameResponse_tag)) {
        logging::printf("GetHostByName: bad response");

        return GetHostByNameFail();
    }

    const MsgGetHostByNameResponse& response(msgResponse.payload.getHostByNameResponse);

    if (response.err != 0) {
        logging::printf("GetHostByName: failed");

        return GetHostByNameFail(response.err);
    }

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

void NetworkProxy::GetServByNameSuccess(uint8* responseData, size_t size) {
    MsgResponse msgResponse;

    if (!DecodeResponse(responseData, size, msgResponse, MsgResponse_getServByNameResponse_tag)) {
        logging::printf("GetServByName: bad response");

        return GetServByNameFail();
    }

    const MsgGetServByNameResponse& response(msgResponse.payload.getServByNameResponse);

    if (response.err != 0) {
        logging::printf("GetServByName: failed");

        return GetServByNameFail(response.err);
    }

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
    (*bufP).servInfo.port = response.port;

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

bool NetworkProxy::DecodeResponse(uint8* responseData, size_t size, MsgResponse& response,
                                  pb_size_t payloadTag, BufferDecodeContext* bufferrDecodeContext) {
    response = MsgResponse_init_zero;

    if (bufferrDecodeContext) {
        response.cb_payload.arg = bufferrDecodeContext;
        response.cb_payload.funcs.decode = setupPayloadDecodeCb;
    }

    pb_istream_t stream = pb_istream_from_buffer(responseData, size);
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
