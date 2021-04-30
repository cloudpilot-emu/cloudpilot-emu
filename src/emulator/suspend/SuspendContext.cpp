#include "SuspendContext.h"

#include "EmCommon.h"
#include "SuspendContextClipboardCopy.h"
#include "SuspendContextClipboardPaste.h"
#include "SuspendManager.h"

SuspendContextClipboardCopy& SuspendContext::AsContextClipboardCopy() {
    EmAssert(GetKind() == Kind::clipboardCopy);

    return static_cast<SuspendContextClipboardCopy&>(*this);
}

SuspendContextClipboardPaste& SuspendContext::AsContextClipboardPaste() {
    EmAssert(GetKind() == Kind::clipboardPaste);

    return static_cast<SuspendContextClipboardPaste&>(*this);
}

void SuspendContext::TriggerResume() { SuspendManager::Resume(); }
