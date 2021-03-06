#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <cstdlib>

#include "EmCommon.h"

namespace Platform {
    template <class T>
    void DisposeMemory(T*& p);

    void* AllocateMemory(size_t count);

    void* AllocateMemoryClear(size_t count);

    long GetMilliseconds();

    void GetTime(uint32& hour, uint32& min, uint32& sec);

    void GetDate(uint32& year, uint32& month, uint32& day);

    uint32 Random();
}  // namespace Platform

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T>
void Platform::DisposeMemory(T*& p) {
    if (p) {
        free(p);

        p = nullptr;
    }
}

#endif  // _PLATFORM_J_
