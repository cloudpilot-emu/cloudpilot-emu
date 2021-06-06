#include "NetworkProxy.h"

#include <functional>

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
        address.ip = sockAddrIN->addr;

        return true;
    }

    void deserializeAddress(NetSocketAddrType* sockAddr, const Address& address) {
        NetSocketAddrINType* sockAddrIN = reinterpret_cast<NetSocketAddrINType*>(sockAddr);

        sockAddrIN->family = netSocketAddrINET;
        sockAddrIN->port = address.port;
        sockAddrIN->addr = address.ip;
    }

    struct BufferEncodeContext {
        uint8* data;
        size_t len;
    };

    bool bufferEncodeCb(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg) {
        if (!arg) return false;

        const BufferEncodeContext* ctx = (const BufferEncodeContext*)(*arg);

        pb_encode_tag_for_field(stream, field);

        return pb_encode_string(stream, ctx->data, ctx->len);
    }
}  // namespace

NetworkProxy& gNetworkProxy{networkProxy};

void NetworkProxy::Open() {
    if (openCount > 0) return ConnectSuccess();

    SuspendManager::Suspend<SuspendContextNetworkConnect>(bind(&NetworkProxy::ConnectSuccess, this),
                                                          bind(&NetworkProxy::ConnectAbort, this));
}

Err NetworkProxy::Close() {
    if (openCount == 0) return 1;

    openCount--;

    return 0;
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
                   bind(&NetworkProxy::SocketOpenFail, this, netErrInternal));
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

    cout << "SocketOpen: handle=" << response.payload.socketOpenResponse.handle << endl << flush;

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
                   bind(&NetworkProxy::SocketBindFail, this, netErrInternal));
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

void NetworkProxy::SocketAddr(int16 handle) {
    MsgRequest request = NewRequest(MsgRequest_socketAddrRequest_tag);
    request.payload.socketAddrRequest.handle = handle;

    SendAndSuspend(request, REQUEST_STATIC_SIZE,
                   bind(&NetworkProxy::SocketAddrSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketAddrFail, this, netErrInternal));
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

void NetworkProxy::SocketSend(int16 handle, uint8* data, size_t count, int32 flags,
                              NetSocketAddrType* toAddrP, int32 toLen, int32 timeout) {
    MsgRequest request = NewRequest(MsgRequest_socketSendRequest_tag);
    MsgSocketSendRequest& sendRequest(request.payload.socketSendRequest);

    sendRequest.handle = handle;

    if (flags) {
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

    sendRequest.timeout = timeout;

    BufferEncodeContext bufferEncodeCtx{data, count};

    sendRequest.data.arg = &bufferEncodeCtx;
    sendRequest.data.funcs.encode = bufferEncodeCb;

    SendAndSuspend(request, REQUEST_STATIC_SIZE + count,
                   bind(&NetworkProxy::SocketSendSuccess, this, _1, _2),
                   bind(&NetworkProxy::SocketSendFail, this, netErrInternal));
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

bool NetworkProxy::DecodeResponse(uint8* responseData, size_t size, MsgResponse& response,
                                  pb_size_t payloadTag) {
    response = MsgResponse_init_default;
    unique_ptr<uint8[]> autodelete(responseData);

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

void NetworkProxy::SendAndSuspend(MsgRequest& request, size_t staticSize,
                                  SuspendContextNetworkRpc::successCallbackT cbSuccess,
                                  SuspendContextNetworkRpc::failCallbackT cbFail) {
    uint8* buffer = new uint8[staticSize];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, staticSize);

    pb_encode(&stream, MsgRequest_fields, &request);

    SuspendManager::Suspend<SuspendContextNetworkRpc>(buffer, stream.bytes_written, cbSuccess,
                                                      cbFail);
}
