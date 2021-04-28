#include "SuspendContextClipboardPaste.h"

#include "ROMStubs.h"

SuspendContext::Kind SuspendContextClipboardPaste::GetKind() const { return Kind::clipboardPaste; }

void SuspendContextClipboardPaste::Cancel() { Resume(""); }

void SuspendContextClipboardPaste::Resume(const string clipboardContent) {
    if (clipboardContent.size() > 0) {
        ClipboardAddItem(clipboardText, clipboardContent.c_str(),
                         min(clipboardContent.size(), static_cast<size_t>(cbdMaxTextLength)));
    } else {
        ClipboardAddItem(clipboardText, NULL, 0);
    }

    TriggerResume();
}
