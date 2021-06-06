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

    SendAndSuspend(request, bind(&NetworkProxy::SocketOpenSuccess, this, _1, _2),
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
    if (sockAddrP->family != netSocketAddrINET) return SocketBindFail(netErrParamErr);

    NetSocketAddrINType* sockAddrINP = reinterpret_cast<NetSocketAddrINType*>(sockAddrP);

    MsgRequest request = NewRequest(MsgRequest_socketBindRequest_tag);

    request.payload.socketBindRequest.handle = handle;
    request.payload.socketBindRequest.address.port = sockAddrINP->port;
    request.payload.socketBindRequest.address.ip = sockAddrINP->addr;

    SendAndSuspend(request, bind(&NetworkProxy::SocketBindSuccess, this, _1, _2),
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

    SendAndSuspend(request, bind(&NetworkProxy::SocketAddrSuccess, this, _1, _2),
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
        NetSocketAddrINType* locAddrINP =
            reinterpret_cast<NetSocketAddrINType*>((NetSocketAddrType*)locAddrP);

        *locAddrLenP = 8;

        locAddrINP->family = netSocketAddrINET;
        locAddrINP->addr = response.payload.socketAddrResponse.addressLocal.ip;
        locAddrINP->port = response.payload.socketAddrResponse.addressLocal.port;

        CALLED_PUT_PARAM_REF(locAddrP);
        CALLED_PUT_PARAM_REF(locAddrLenP);
    }

    if (remAddrP) {
        NetSocketAddrINType* remAddrINP =
            reinterpret_cast<NetSocketAddrINType*>((NetSocketAddrType*)remAddrP);

        *remAddrLenP = 8;

        remAddrINP->family = netSocketAddrINET;
        remAddrINP->addr = response.payload.socketAddrResponse.addressRemote.ip;
        remAddrINP->port = response.payload.socketAddrResponse.addressRemote.port;

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

bool NetworkProxy::DecodeResponse(uint8* responseData, size_t size, MsgResponse& response,
                                  pb_size_t payloadTag) {
    response = MsgResponse_init_zero;
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

void NetworkProxy::SendAndSuspend(MsgRequest& request,
                                  SuspendContextNetworkRpc::successCallbackT cbSuccess,
                                  SuspendContextNetworkRpc::failCallbackT cbFail) {
    uint8* buffer = new uint8[MsgRequest_size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, MsgRequest_size);

    pb_encode(&stream, MsgRequest_fields, &request);

    SuspendManager::Suspend<SuspendContextNetworkRpc>(buffer, stream.bytes_written, cbSuccess,
                                                      cbFail);
}
