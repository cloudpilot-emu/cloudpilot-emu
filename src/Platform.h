#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <cstdlib>

#include "EmCommon.h"

namespace Platform {
    template <class T>
    static void DisposeMemory(T*& p);

    long getMilliseconds();

    void getTime(uint32& hour, uint32& month, uint32& day);
}  // namespace Platform

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T>
static void Platform::DisposeMemory(T*& p) {
    if (p) {
        free((void*)p);

        p = nullptr;
    }
}

#endif  // _PLATFORM_J_
