#ifndef _SOC_PXA_H_
#define _SOC_PXA_H_

#include <cstdint>

#include "CPU.h"
#include "device_type5.h"
#include "memory_buffer.h"
#include "soc_generic.h"

struct SocUart;
struct SocSsp;
struct SocGpio;
struct SocAC97;
struct SocDma;
struct SocI2s;
struct SocI2c;
struct SocIc;
struct PxaMemCtrlr;
struct PxaPwrClk;
struct SocI2c;
struct PxaPwm;
struct PxaTimr;
struct PxaMmc;
struct PxaRtc;
struct PxaLcd;
struct Pxa270wmmx;
struct Pxa270Udc;
struct PxaImc;
struct PxaKpc;
struct Pxa255dsp;
struct Pxa255Udc;
struct Device;
struct Keypad;

class SocPXA : public SocGeneric<SocPXA> {
    friend SocGeneric<SocPXA>;

   public:
    static constexpr int MEMORY_SYSTEM_KIND = ARM_MEMORY_SYSTEM_MMU;

   public:
    SocPXA(enum DeviceType5 deviceType, uint32_t ramSize, void *romData, const uint32_t romSize,
           uint8_t *nandContent, size_t nandSize, int gdbPort, uint_fast8_t socRev);

    uint32_t *GetPendingFrame() override;
    void ResetPendingFrame() override;
    enum DeviceType5 GetDeviceType() override;
    void SuspendTimerInterrupts(bool suspendInterrupts) override;
    bool LcdEnabled() override;

    uint32_t DispatchTicks(uint32_t clientType, uint32_t batchedTicks);

   protected:
    void OnSetFramebufferDirty() override;
    void OnSleep() override;
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
    void SchedulePcmTask();

    void CycleBatch0();
    bool Batch0Required();

    static void RescheduleCB(void *ctx, uint32_t task);
    static uint64_t GetTimeCB(void *ctx);

   private:
    uint8_t socRev{0};

    SocUart *ffUart{nullptr}, *hwUart{nullptr}, *stUart{nullptr}, *btUart{nullptr};
    SocSsp *ssp[3]{nullptr, nullptr, nullptr};
    SocGpio *gpio{nullptr};
    SocAC97 *ac97{nullptr};
    SocDma *dma{nullptr};
    SocI2s *i2s{nullptr};
    SocI2c *i2c{nullptr};
    SocIc *ic{nullptr};

    PxaMemCtrlr *memCtrl{nullptr};
    PxaPwrClk *pwrClk{nullptr};
    SocI2c *pwrI2c{nullptr};
    PxaPwm *pwm[4]{nullptr, nullptr, nullptr, nullptr};
    PxaTimr *tmr{nullptr};
    PxaMmc *mmc{nullptr};
    PxaRtc *rtc{nullptr};
    PxaLcd *lcd{nullptr};
    ArmRam *sram{nullptr};
    ArmRam *ramMirror{nullptr};  // mirror for ram termination
    Device *dev{nullptr};
    Keypad *kp{nullptr};

    MemoryBuffer bufferNand{};
    MemoryBuffer bufferLcd{};
    MemoryBuffer bufferSram{};

    union {
        // 25x/26x
        struct {
            Pxa255dsp *dsp;
            Pxa255Udc *udc1;
        };
        // PXA27x
        struct {
            Pxa270wmmx *wmmx;
            Pxa270Udc *udc2;
            PxaImc *imc;
            PxaKpc *kpc;
        };
        void *__zeroInit[4]{nullptr};
    };
};

#endif  // _SOC_PXA_H_
