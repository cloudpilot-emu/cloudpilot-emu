#include "NetworkProxy.h"

namespace {
    NetworkProxy networkProxy;
}

NetworkProxy& gNetworkProxy{networkProxy};

Err NetworkProxy::Open() {
    openCount++;

    return 0;
}

Err NetworkProxy::Close() {
    if (openCount == 0) return 1;

    openCount--;

    return 0;
}

int NetworkProxy::OpenCount() { return openCount; }
