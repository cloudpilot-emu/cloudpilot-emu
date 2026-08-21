#ifndef _SOC_PV_H_
#define _SOC_PV_H_

#include <cstddef>
#include <memory>

#include "memory_buffer.h"
#include "pv_display.h"
#include "soc_generic.h"

struct ArmRam;
struct PvIc;
struct PvTimer;
struct PvUart;
struct PvHypercallInterface;
struct PvDisplay;
struct PvRtc;
struct PvKeys;
struct PvAudio;
struct PvTouch;
struct PvSysctl;
struct PvStorage;

class SocPV : public SocGeneric<SocPV> {
    friend SocGeneric<SocPV>;

   public:
    static constexpr int MEMORY_SYSTEM_KIND = ARM_MEMORY_SYSTEM_MPU;

   public:
    SocPV(uint32_t ramSize, void *romData, const uint32_t romSize, uint32_t displayWidth,
          uint32_t displayHeight, uint32_t displayDensity, int gdbPort);

    uint32_t *GetPendingFrame() override;
    void ResetPendingFrame() override;
    enum DeviceType5 GetDeviceType() override;
    void SuspendTimerInterrupts(bool suspendInterrupts) override;
    bool LcdEnabled() override;

    uint32_t DispatchTicks(uint32_t clientType, uint32_t batchedTicks);

   protected:
    void OnSetFramebufferDirty() override;
    bool OnSleep() override;
    void OnWakeup() override;
    void OnSetAudioQueue(struct AudioQueue *audioQueue) override;
    void OnSetPcmOutputEnabled() override;
    void OnSetPcmSuspended() override;
    void OnSdInsert() override;
    void OnSdEject() override;

    void OnTouch(int x, int y);
    void OnEngageKey(KeyId key, bool down);
    void OnReset();

    void OnLoad(SavestateLoader<ChunkType> &loader);

    template <typename T>
    void OnSave(T &savestate);

   private:
    void AllocateBuffers();
    void SetupScheduler();

   private:
    MemoryBuffer bufferTinyRam;
    MemoryBuffer bufferClut;
    ArmRam *tinyRam{nullptr};

    uint32_t displayWidth{0};
    uint32_t displayHeight{0};
    uint32_t displayDensity{0};

    PvTimer *timer{nullptr};
    PvIc *ic{nullptr};
    PvUart *uart{nullptr};
    PvUart *uartDebug{nullptr};
    PvHypercallInterface *hypercallIface{nullptr};
    PvDisplay *display{nullptr};
    PvKeys *keys{nullptr};
    PvRtc *rtc{nullptr};
    PvAudio *audio{nullptr};
    PvTouch *touch{nullptr};
    PvSysctl *sysctl{nullptr};
    PvStorage *storage{nullptr};

    std::unique_ptr<uint32_t[]> framebuffer;
};

#endif  // _SOC_PV_H_
