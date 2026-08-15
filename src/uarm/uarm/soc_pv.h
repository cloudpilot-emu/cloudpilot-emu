#ifndef _SOC_PV_H_
#define _SOC_PV_H_

#include "memory_buffer.h"
#include "soc_generic.h"

struct ArmRam;
struct PvIc;
struct PvTimer;
struct PvUart;

class SocPV : public SocGeneric<SocPV> {
    friend SocGeneric<SocPV>;

   public:
    static constexpr int MEMORY_SYSTEM_KIND = ARM_MEMORY_SYSTEM_MPU;

   public:
    SocPV(uint32_t ramSize, void *romData, const uint32_t romSize, int gdbPort);

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
    ArmRam *tinyRam;

    PvTimer *timer;
    PvIc *ic;
    PvUart *uart;
    PvUart *uartDebug;
};

#endif  // _SOC_PV_H_
