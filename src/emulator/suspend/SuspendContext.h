#ifndef _SUSPEND_CONTEXT_H_
#define _SUSPEND_CONTEXT_H_

class SuspendContextClipboardCopy;
class SuspendContextClipboardPaste;

class SuspendContext {
   public:
    enum Kind : int { clipboardCopy = 1, clipboardPaste = 2 };

   public:
    SuspendContext() = default;

    virtual ~SuspendContext() = default;

    virtual Kind GetKind() const = 0;

    virtual void Cancel() = 0;

    SuspendContextClipboardCopy& AsContextClipboardCopy();
    SuspendContextClipboardPaste& AsContextClipboardPaste();

   protected:
    void TriggerResume();

   private:
    SuspendContext(const SuspendContext&) = delete;
    SuspendContext(SuspendContext&&) = delete;
    SuspendContext& operator=(const SuspendContext&) = delete;
    SuspendContext& operator=(SuspendContext&&) = delete;
};

#endif  // _SUSPEND_CONTEXT_H_
