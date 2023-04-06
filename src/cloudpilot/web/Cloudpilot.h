#ifndef _CLOUDPILOT_H_
#define _CLOUDPILOT_H_

#include <memory>
#include <string>

#include "DbBackup.h"
#include "EmDevice.h"
#include "Frame.h"
#include "SuspendContext.h"

enum class CardSupportLevel : int { unsupported = 0, sdOnly = 1, sdAndMs = 2 };

class Cloudpilot {
   public:
    void* Malloc(int size);
    void Free(void* buffer);
    void* Nullptr();

    bool InitializeSession(void* buffer, int size, const char* deviceType);

    int GetCyclesPerSecond();
    int RunEmulation(int cycles);
    void SetClockFactor(double clockFactor);

    Frame& CopyFrame();
    bool IsScreenDirty();
    void MarkScreenClean();

    int MinMemoryForDevice(string id);
    int FramebufferSizeForDevice(string id);
    int TotalMemoryForDevice(string id);

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
    bool ImportMemoryImage(void* buffer, size_t len);

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
    bool GetClipboardIntegration();

    bool IsSuspended();
    SuspendContext& GetSuspendContext();

    void SetNetworkRedirection(bool toggle);
    bool GetNetworkRedirection();
    void RegisterProxyDisconnectHandler(uint32 handlerPtr);

    void SetHotsyncNameManagement(bool toggle);

    bool LaunchAppByName(const char* name);
    bool LaunchAppByDbHeader(void* header, int len);

    bool DeviceSupportsCardSize(uint32 size);
    void ClearExternalStorage();
    bool AllocateCard(const char* key, uint32 blockCount);
    bool AdoptCard(const char* key, void* data, uint32 blockCount);
    bool MountCard(const char* key);
    bool RemoveCard(const char* key);
    void* GetCardData(const char* key);
    void* GetCardDirtyPages(const char* key);
    int GetCardSize(const char* key);
    void RemountCards();
    int GetSupportLevel(uint32 size);
    const char* GetMountedKey();

   private:
    Frame frame{320 * 480 * 4};
};

#endif  // _CLOUDPILOT_H_
