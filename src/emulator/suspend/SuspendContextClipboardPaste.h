#ifndef _SUSPEND_CONTEXT_CLIPBOARD_PASTE_H_
#define _SUSPEND_CONTEXT_CLIPBOARD_PASTE_H_

#include "EmCommon.h"
#include "SuspendContext.h"

class SuspendContextClipboardPaste : public SuspendContext {
   public:
    SuspendContextClipboardPaste() = default;

    Kind GetKind() const override;

    void Cancel() override;

    void Resume(const string clipboardContent);

    bool RequiresStackAccess() override;

   private:
    SuspendContextClipboardPaste(const SuspendContextClipboardPaste&) = delete;
    SuspendContextClipboardPaste(SuspendContextClipboardPaste&&) = delete;
    SuspendContextClipboardPaste& operator=(const SuspendContextClipboardPaste&) = delete;
    SuspendContextClipboardPaste& operator=(SuspendContextClipboardPaste&&) = delete;
};

#endif  // _SUSPEND_CONTEXT_CLIPBOARD_PASTE_H_
