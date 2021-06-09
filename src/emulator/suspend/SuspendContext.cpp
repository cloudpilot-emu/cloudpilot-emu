#include "SuspendContext.h"

#include "EmCommon.h"
#include "SuspendContextClipboardCopy.h"
#include "SuspendContextClipboardPaste.h"
#include "SuspendContextNetworkConnect.h"
#include "SuspendContextNetworkDisconnect.h"
#include "SuspendContextNetworkRpc.h"
#include "SuspendManager.h"

SuspendContextClipboardCopy& SuspendContext::AsContextClipboardCopy() {
    EmAssert(GetKind() == Kind::clipboardCopy);

    return static_cast<SuspendContextClipboardCopy&>(*this);
}

SuspendContextClipboardPaste& SuspendContext::AsContextClipboardPaste() {
    EmAssert(GetKind() == Kind::clipboardPaste);

    return static_cast<SuspendContextClipboardPaste&>(*this);
}

SuspendContextNetworkRpc& SuspendContext::AsContextNetworkRpc() {
    EmAssert(GetKind() == Kind::networkRpc);

    return static_cast<SuspendContextNetworkRpc&>(*this);
}

SuspendContextNetworkConnect& SuspendContext::AsContextNetworkConnect() {
    EmAssert(GetKind() == Kind::networkConnect);

    return static_cast<SuspendContextNetworkConnect&>(*this);
}

SuspendContextNetworkDisconnect& SuspendContext::AsContextNetworkDisconnect() {
    EmAssert(GetKind() == Kind::networkDisconnect);

    return static_cast<SuspendContextNetworkDisconnect&>(*this);
}

void SuspendContext::ResumeExecution() { SuspendManager::Resume(); }
