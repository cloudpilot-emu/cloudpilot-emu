#ifndef _CALLBACK_MANAGER_H_
#define _CALLBACK_MANAGER_H_

#include <functional>

#include "EmCommon.h"

class CallbackManager {
   public:
    using CallbackT = function<void()>;

   public:
    static void Clear();

    static emuptr RegisterCallback(CallbackT callback);

    static void ReleaseCallback(emuptr callbackPtr);

    static void HandleBreakpoint();
};

class CallbackWrapper {
   public:
    CallbackWrapper(CallbackManager::CallbackT callback);

    ~CallbackWrapper();

    operator emuptr() const;

   private:
    emuptr callbackPtr{0};
};

#endif  // _CALLBACK_MANAGER_H_
