#include "NetworkProxy.h"

#include <functional>

#include "EmSubroutine.h"
#include "Marshal.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendContextNetworkRpc.h"
#include "SuspendManager.h"
#include "networking.pb.h"
#include "pb_encode.h"

using namespace std::placeholders;

namespace {
    NetworkProxy networkProxy;
}

NetworkProxy& gNetworkProxy{networkProxy};

void NetworkProxy::Open() {
    if (openCount > 0) {
        ConnectSuccess();

        return;
    }

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
    MsgRequest request = MsgRequest_init_zero;
    request.which_payload = MsgRequest_socketOpenRequest_tag;

    cout << (int)domain << " " << (int)type << " " << (int)protocol << endl << flush;

    request.payload.socketOpenRequest.domain = domain;
    request.payload.socketOpenRequest.type = type;
    request.payload.socketOpenRequest.protocol = protocol;

    unique_ptr<uint8[]> buffer = make_unique<uint8[]>(MsgRequest_size);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer.get(), MsgRequest_size);

    pb_encode(&stream, MsgRequest_fields, &request);

    SuspendManager::Suspend<SuspendContextNetworkRpc>(
        buffer.release(), stream.bytes_written,
        bind(&NetworkProxy::SocketOpenSuccess, this, _1, _2),
        bind(&NetworkProxy::SocketOpenFail, this));
}

void NetworkProxy::ConnectSuccess() {
    CALLED_SETUP("Err", "void");
    PUT_RESULT_VAL(Err, 0);

    openCount++;
}

void NetworkProxy::ConnectAbort() {
    CALLED_SETUP("Err", "void");
    PUT_RESULT_VAL(Err, 1);
}

void NetworkProxy::SocketOpenSuccess(uint8* response, size_t size) {
    // TODO
}

void NetworkProxy::SocketOpenFail() {
    CALLED_SETUP("NetSocketRef",
                 "UInt16 libRefNum, NetSocketAddrEnum domain, "
                 "NetSocketTypeEnum type, Int16 protocol, Int32 timeout, "
                 "Err *errP");

    CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

    *errP = 1;
    CALLED_PUT_PARAM_REF(errP);

    PUT_RESULT_VAL(Int16, -1);
}
