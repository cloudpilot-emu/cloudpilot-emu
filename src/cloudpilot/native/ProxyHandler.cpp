#include "ProxyHandler.h"

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
        gNetworkProxy.onDisconnect.AddHandler(bind(&ProxyHandler::OnDisconnectHandler, this));
}

void ProxyHandler::Teardown() {
    client.Disconnect();
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

    if (client.Connect()) {
        context.AsContextNetworkConnect().Resume();

        logPrintf("network proxy connected");
    } else {
        context.Cancel();

        logPrintf("ERROR: failed to connect to network proxy");
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

void ProxyHandler::OnDisconnectHandler() { client.Disconnect(); }
