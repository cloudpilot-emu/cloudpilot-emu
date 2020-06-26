#ifndef _CLOUDPILOT_H_
#define _CLOUDPILOT_H_

#include <string>

#include "RomInfo.h"

class Cloudpilot {
   public:
    void* Malloc(long size);
    void Free(void* buffer);

    bool GetRomInfo(void* buffer, long size, RomInfo& romInfo);
};

#endif  // _CLOUDPILOT_H_
