#ifndef _CLOUDPILOT_H_
#define _CLOUDPILOT_H_

#include <memory>
#include <string>

#include "EmDevice.h"
#include "RomInfo.h"

class Cloudpilot {
   public:
    void* Malloc(long size);
    void Free(void* buffer);

    bool GetRomInfo(void* buffer, long size, RomInfo& romInfo);

    bool InitializeSession(void* buffer, long size, const char* deviceType);

   private:
    unique_ptr<EmDevice> device;
};

#endif  // _CLOUDPILOT_H_
