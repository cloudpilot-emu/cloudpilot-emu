#ifndef _SUSPEND_CONTEXT_CLIPBOARD_COPY_H_
#define _SUSPEND_CONTEXT_CLIPBOARD_COPY_H_

#include "EmCommon.h"
#include "SuspendContext.h"

class SuspendContextClipboardCopy : public SuspendContext {
   public:
    SuspendContextClipboardCopy(const string& clipboardContent);

    Kind GetKind() const override;

    void Cancel() override;

    void Resume();

    const char* GetClipboardContent();

    bool RequiresStackAccess() override;

   private:
    const string clipboardContent;

   private:
    SuspendContextClipboardCopy(const SuspendContextClipboardCopy&) = delete;
    SuspendContextClipboardCopy(SuspendContextClipboardCopy&&) = delete;
    SuspendContextClipboardCopy& operator=(const SuspendContextClipboardCopy&) = delete;
    SuspendContextClipboardCopy& operator=(SuspendContextClipboardCopy&&) = delete;
};

#endif  // _SUSPEND_CONTEXT_CLIPBOARD_COPY_H_
