#include "NetworkProxy.h"

#include <functional>

#include "EmSubroutine.h"
#include "Marshal.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendManager.h"

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

void NetworkProxy::ConnectSuccess() {
    CALLED_SETUP("Err", "void");
    PUT_RESULT_VAL(Err, 0);

    openCount++;
}

void NetworkProxy::ConnectAbort() {
    CALLED_SETUP("Err", "void");
    PUT_RESULT_VAL(Err, 1);
}
