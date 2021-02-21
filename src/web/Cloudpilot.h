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

    bool InitializeSession(void* buffer, long size, const char* deviceType);

    long GetCyclesPerSecond();
    long RunEmulation(long cycles);

    Frame& CopyFrame();
    bool IsScreenDirty();
    void MarkScreenClean();

    long MinMemoryForDevice(string id);

    void QueuePenMove(int x, int y);
    void QueuePenUp();

    void QueueButtonDown(int id);
    void QueueButtonUp(int id);

    bool IsPowerOff();

    void Reset();
    void ResetNoExtensions();
    void ResetHard();

   private:
    Frame frame{1024 * 128};
};

#endif  // _CLOUDPILOT_H_
