#ifndef _SUSPEND_CONTEXT_H_
#define _SUSPEND_CONTEXT_H_

class SuspendContextClipboardCopy;
class SuspendContextClipboardPaste;
class SuspendContextNetworkRpc;
class SuspendContextNetworkConnect;
class SuspendContextSerialSync;

class SuspendContext {
   public:
    enum Kind : int {
        clipboardCopy = 1,
        clipboardPaste = 2,
        networkRpc = 3,
        networkConnect = 4,
        serialSync = 5
    };

   public:
    SuspendContext() = default;

    virtual ~SuspendContext() = default;

    virtual Kind GetKind() const = 0;

    virtual void Cancel() = 0;

    virtual bool RequiresStackAccess();

    SuspendContextClipboardCopy& AsContextClipboardCopy();
    SuspendContextClipboardPaste& AsContextClipboardPaste();
    SuspendContextNetworkRpc& AsContextNetworkRpc();
    SuspendContextNetworkConnect& AsContextNetworkConnect();
    SuspendContextSerialSync& AsContextSerialSync();

   protected:
    void ResumeExecution();

   private:
    SuspendContext(const SuspendContext&) = delete;
    SuspendContext(SuspendContext&&) = delete;
    SuspendContext& operator=(const SuspendContext&) = delete;
    SuspendContext& operator=(SuspendContext&&) = delete;
};

#endif  // _SUSPEND_CONTEXT_H_
