#ifndef _SUSPEND_CONTEXT_SERIAL_SYNC_H_
#define _SUSPEND_CONTEXT_SERIAL_SYNC_H_

#include "SuspendContext.h"

class SuspendContextSerialSync : public SuspendContext {
   public:
    SuspendContextSerialSync();

    Kind GetKind() const override;

    void Cancel() override;
    void Resume();

    bool RequiresStackAccess() override;
};

#endif  // _SUSPEND_CONTEXT_SERIAL_SYNC_H_
