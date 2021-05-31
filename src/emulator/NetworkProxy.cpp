#include "NetworkProxy.h"

#include <functional>

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
}

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

int NetworkProxy::OpenCount() { return openCount; }

void NetworkProxy::SocketOpen(uint8 domain, uint8 type, uint16 protocol) {
    if (domain != netSocketAddrINET) {
        logging::printf("raw sockets are unsupporrted");
        return SocketOpenFail(netErrParamErr);
    }

    MsgRequest request = NewRequest(MsgRequest_socketOpenRequest_tag);

    request.payload.socketOpenRequest.type = type;
    request.payload.socketOpenRequest.protocol = protocol;

    unique_ptr<uint8[]> buffer = make_unique<uint8[]>(MsgRequest_size);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer.get(), MsgRequest_size);

    pb_encode(&stream, MsgRequest_fields, &request);

    SuspendManager::Suspend<SuspendContextNetworkRpc>(
        buffer.release(), stream.bytes_written,
        bind(&NetworkProxy::SocketOpenSuccess, this, _1, _2),
        bind(&NetworkProxy::SocketOpenFail, this, netErrInternal));
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

void NetworkProxy::SocketOpenSuccess(uint8* response, size_t size) {
    MsgResponse msgResponse;

    if (!DecodeResponse(response, size, msgResponse, MsgResponse_socketOpenResponse_tag)) {
        logging::printf("SocketOpen: bad response");

        return SocketOpenFail();
    }

    if (msgResponse.payload.socketOpenResponse.handle < 0) {
        logging::printf("SocketOpen: failed");

        return SocketOpenFail(msgResponse.payload.socketOpenResponse.err);
    }

    cout << "SocketOpen: handle=" << msgResponse.payload.socketOpenResponse.handle << endl << flush;

    CALLED_SETUP("NetSocketRef",
                 "UInt16 libRefNum, NetSocketAddrEnum domain, "
                 "NetSocketTypeEnum type, Int16 protocol, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 0;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, static_cast<Int16>(msgResponse.payload.socketOpenResponse.handle));
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

MsgRequest NetworkProxy::NewRequest(pb_size_t payloadTag) {
    MsgRequest request = MsgRequest_init_zero;

    request.id = ++currentId;
    request.which_payload = payloadTag;

    return request;
}

bool NetworkProxy::DecodeResponse(uint8* response, size_t size, MsgResponse& msgResponse,
                                  pb_size_t payloadTag) {
    msgResponse = MsgResponse_init_zero;
    unique_ptr<uint8[]> autodelete(response);

    pb_istream_t stream = pb_istream_from_buffer(response, size);
    bool status = pb_decode(&stream, MsgResponse_fields, &msgResponse);

    if (!status) {
        logging::printf("failed to decode response");

        return false;
    }

    if (msgResponse.id != currentId) {
        logging::printf("response out of order");
        return false;
    }

    if (msgResponse.which_payload != MsgResponse_socketOpenResponse_tag) {
        logging::printf("tag mismatch");
        return false;
    }

    return true;
}
