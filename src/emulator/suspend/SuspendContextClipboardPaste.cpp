#include "SuspendContextClipboardPaste.h"

SuspendContext::Kind SuspendContextClipboardPaste::GetKind() const { return Kind::clipboardPaste; }

void SuspendContextClipboardPaste::Cancel() { Resume(""); }

void SuspendContextClipboardPaste::Resume(const string clipboardContent) {
    // TODO

    TriggerResume();
}
