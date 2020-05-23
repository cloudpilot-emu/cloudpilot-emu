#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <cstdlib>

#include "EmCommon.h"

namespace Platform {
    template <class T>
    void DisposeMemory(T*& p);

    void* AllocateMemory(size_t count);

    void* AllocateMemoryClear(size_t count);

    long getMilliseconds();

    void getTime(uint32& hour, uint32& month, uint32& day);
}  // namespace Platform

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T>
void Platform::DisposeMemory(T*& p) {
    if (p) {
        delete[] p;

        p = nullptr;
    }
}

#endif  // _PLATFORM_J_
