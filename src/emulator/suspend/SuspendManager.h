#ifndef _SUSPEND_MANAGER_H_
#define _SUSPEND_MANAGER_H_

#include "EmCommon.h"

class SuspendContext;

class SuspendManager {
    friend SuspendContext;

   public:
    static inline bool IsSuspended();

    static SuspendContext& GetContext();

    template <class T, class... Us>
    static void Suspend(Us... args);

    static void Reset();

   private:
    static void Resume();

   private:
    static SuspendContext* context;
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T, class... Us>
void SuspendManager::Suspend(Us... args) {
    EmAssert(context == nullptr);

    context = new T(args...);
}

bool SuspendManager::IsSuspended() { return context != nullptr; }

#endif  // _SUSPEND_MANAGER_H_
