#ifndef _CALLBACK_MANAGER_H_
#define _CALLBACK_MANAGER_H_

#include <functional>

#include "EmCommon.h"

class CallbackManager {
   public:
    using CallbackT = function<void()>;

   public:
    static void Reset();

    static emuptr RegisterCallback(CallbackT callback);

    static void ReleaseCallback(emuptr callbackPtr);

    static void HandleBreakpoint();
};

#endif  // _CALLBACK_MANAGER_H_
