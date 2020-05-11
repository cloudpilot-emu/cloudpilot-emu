#ifndef _PLATFORM_H_
#define _PLATFORM_H_

namespace Platform {
    template <class T>
    static void DisposeMemory(T*& p);
}

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
