#include "SuspendContextClipboardPaste.h"

#include "Miscellaneous.h"
#include "ROMStubs.h"

SuspendContext::Kind SuspendContextClipboardPaste::GetKind() const { return Kind::clipboardPaste; }

void SuspendContextClipboardPaste::Cancel() { Resume(""); }

void SuspendContextClipboardPaste::Resume(const string clipboardContent) {
    string contentIsolatin1 = Utf8ToIsolatin1(clipboardContent, cbdMaxTextLength);

    if (contentIsolatin1.size() > 0) {
        ClipboardAddItem(clipboardText, contentIsolatin1.c_str(),
                         min(contentIsolatin1.size(), static_cast<size_t>(cbdMaxTextLength)));
    } else {
        ClipboardAddItem(clipboardText, NULL, 0);
    }

    TriggerResume();
}
