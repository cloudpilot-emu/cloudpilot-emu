#ifndef _DEBUG_SUPPORT_H_
#define _DEBUG_SUPPORT_H_

#include "EmCommon.h"
#include "GdbStub.h"

namespace debug_support {
    void SetApp(const uint8* elfData, size_t elfSize, GdbStub& gdbStub, Debugger& debugger);

    emuptr FindRegion(const uint8* region, size_t regionSize, emuptr start, size_t size);

    void Locate(const uint8* data, size_t size);
}  // namespace debug_support

#endif  // _DEBUG_SUPPORT_H_
