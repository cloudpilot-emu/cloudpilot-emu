#ifndef _META_MEMORY_H_
#define _META_MEMORY_H_

#include "EmCommon.h"

class MetaMemory {
   public:
    static Bool InRAMOSComponent(emuptr pc);

    static void Initialize();

    static void Reset();

    static void Dispose();
};

#endif  // _META_MEMORY_H_
