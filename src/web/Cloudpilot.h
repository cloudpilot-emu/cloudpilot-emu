#ifndef _CLOUDPILOT_H_
#define _CLOUDPILOT_H_

#include <memory>
#include <string>

#include "DbBackup.h"
#include "EmDevice.h"
#include "Frame.h"
#include "SuspendContext.h"

class Cloudpilot {
   public:
    void* Malloc(long size);
    void Free(void* buffer);

    bool InitializeSession(void* buffer, long size, const char* deviceType);

    long GetCyclesPerSecond();
    long RunEmulation(long cycles);
    void SetClockFactor(double clockFactor);

    Frame& CopyFrame();
    bool IsScreenDirty();
    void MarkScreenClean();

    long MinMemoryForDevice(string id);

    void QueuePenMove(int x, int y);
    void QueuePenUp();

    void QueueButtonDown(int id);
    void QueueButtonUp(int id);

    void QueueKeyboardEvent(uint16 c, bool ctrl);

    bool IsPowerOff();
    bool IsUIInitialized();
    bool IsSetupComplete();
    int GetOSVersion();

    void Reset();
    void ResetNoExtensions();
    void ResetHard();

    int InstallDb(void* buffer, size_t len);

    int GetPalette2bitMapping();

    void* GetMemoryPtr();
    void* GetDirtyPagesPtr();
    int GetMemorySize();

    void* GetRomPtr();
    int GetRomSize();

    void* GetSavestatePtr();
    int GetSavestateSize();

    bool SaveState();
    bool LoadState(void* buffer, int len);

    const char* GetHotsyncName();
    void SetHotsyncName(const char* name);

    void RegisterPwmHandler(uint32 handlerPtr);

    DbBackup* StartBackup();

    void SetClipboardIntegration(bool toggle);

    bool IsSuspended();
    SuspendContext& GetSuspendContext();

    void RegisterProxyDisconnectHandler(uint32 handlerPtr);

   private:
    Frame frame{320 * 320 * 4};
};

#endif  // _CLOUDPILOT_H_
