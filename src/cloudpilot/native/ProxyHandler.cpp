#include "ProxyHandler.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <functional>
#include <sstream>

#include "EmCommon.h"
#include "Logging.h"
#include "NetworkProxy.h"
#include "SuspendContext.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendContextNetworkRpc.h"
#include "SuspendManager.h"

using namespace std::placeholders;

ProxyHandler::ProxyHandler(ProxyClient& client) : client(client) {}

void ProxyHandler::Initialize() {
    if (onDisconnectHandle) return;

    onDisconnectHandle =
        gNetworkProxy.onDisconnect.AddHandler(bind(&ProxyHandler::OnDisconnectHandler, this, _1));
}

void ProxyHandler::Teardown() {
    client.Disconnect();
    sessionId = "";

    if (onDisconnectHandle) {
        gNetworkProxy.onDisconnect.RemoveHandler(*onDisconnectHandle);
        onDisconnectHandle.reset();
    }
}

void ProxyHandler::HandleSuspend() {
    if (!SuspendManager::IsSuspended()) return;

    SuspendContext& context = SuspendManager::GetContext();

    switch (context.GetKind()) {
        case SuspendContext::Kind::networkConnect:
            HandleConnect(context);
            break;

        case SuspendContext::Kind::networkRpc:
            HandleRpc(context);
            break;

        default:
            break;
    }
}

void ProxyHandler::HandleConnect(SuspendContext& context) {
    client.Disconnect();
    sessionId = "";

    if (client.Connect()) {
        stringstream ss;

        ss << boost::uuids::random_generator()();
        sessionId = ss.str();

        context.AsContextNetworkConnect().Resume(sessionId);

        logging::printf("network proxy connected");
    } else {
        context.Cancel();

        logging::printf("ERROR: failed to connect to network proxy");
    }
}

void ProxyHandler::HandleRpc(SuspendContext& context) {
    auto [request, size] = context.AsContextNetworkRpc().GetRequest();

    if (!client.Send(request, size)) {
        context.Cancel();

        return;
    }

    auto [responseBuffer, responseSize] = client.Receive();

    if (responseBuffer) {
        context.AsContextNetworkRpc().ReceiveResponse(responseBuffer, responseSize);
        delete[] responseBuffer;
    } else
        context.Cancel();
}

void ProxyHandler::OnDisconnectHandler(const string& sessionId) {
    if (sessionId != this->sessionId) return;

    client.Disconnect();
    this->sessionId = "";
}
