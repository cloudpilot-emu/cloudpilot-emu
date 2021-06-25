#include "ProxyHandler.h"

#include "EmCommon.h"
#include "SuspendContext.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendContextNetworkDisconnect.h"
#include "SuspendContextNetworkRpc.h"
#include "SuspendManager.h"

ProxyHandler::ProxyHandler(ProxyClient& client) : client(client) {}

void ProxyHandler::Initialize() {}

void ProxyHandler::Teardown() { client.Disconnect(); }

void ProxyHandler::HandleSuspend() {
    if (!SuspendManager::IsSuspended()) return;

    SuspendContext& context = SuspendManager::GetContext();

    switch (context.GetKind()) {
        case SuspendContext::Kind::networkConnect:
            HandleConnect(context);
            break;

        case SuspendContext::Kind::networkDisconnect:
            HandleDisconnect(context);
            break;

        case SuspendContext::Kind::networkRpc:
            HandleRpc(context);
            break;

        default:
            break;
    }
}

void ProxyHandler::HandleConnect(SuspendContext& context) {
    if (client.Connect()) {
        context.AsContextNetworkConnect().Resume();

        cout << "network proxy connected" << endl << flush;
    } else {
        context.Cancel();

        cout << "failed to connect to network proxy" << endl << flush;
    }
}

void ProxyHandler::HandleDisconnect(SuspendContext& context) {
    client.Disconnect();
    context.AsContextNetworkDisconnect().Resume();

    cout << "network proxy disconnected" << endl << flush;
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
