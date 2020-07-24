#ifndef _CLOUDPILOT_H_
#define _CLOUDPILOT_H_

#include <memory>
#include <string>

#include "EmDevice.h"
#include "Frame.h"
#include "RomInfo.h"

class Cloudpilot {
   public:
    void* Malloc(long size);
    void Free(void* buffer);

    bool GetRomInfo(void* buffer, long size, RomInfo& romInfo);
    bool InitializeSession(void* buffer, long size, const char* deviceType);

    long GetCyclesPerSecond();
    long RunEmulation(long cycles);

    Frame& CopyFrame();
    bool IsScreenDirty();
    void MarkScreenClean();

   private:
    unique_ptr<EmDevice> device;
    Frame frame{1024 * 128};
};

#endif  // _CLOUDPILOT_H_
