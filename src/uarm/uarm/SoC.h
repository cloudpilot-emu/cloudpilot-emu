#ifndef _SOC_H_
#define _SOC_H_

#include <memory.h>

#include <cstdint>

#include "buffer.h"
#include "device_type5.h"
#include "keys.h"
#include "memory_buffer.h"
#include "queue.h"
#include "savestate/ChunkTypeUarm.h"
#include "savestate/Savestate.h"
#include "sdcard.h"

struct VSD;
struct ArmRam;
struct ArmRom;
struct ArmMem;
struct NAND;
struct ArmCpu;
struct SystemState;
struct SyscallDispatch;
struct PacePatch;
struct PatchDispatch;
struct AudioQueue;
struct PatchContext;

class SoC {
   public:
    virtual void Reset() = 0;

    virtual uint64_t Run(uint64_t maxCycles, uint64_t cyclesPerSecond) = 0;
    virtual bool RunToPaceSyscall(uint16_t syscall, uint64_t maxCycles,
                                  uint64_t cyclesPerSecond) = 0;
    virtual bool ExecuteInjected(uint64_t maxCycles, uint64_t cyclesPerSecond) = 0;

    uint64_t GetInjectedTimeNsec() { return injectedTimeNsec; }
    void ResetInjectedTimeNsec() { injectedTimeNsec = 0; }

    void KeyDown(enum KeyId key);
    void KeyUp(enum KeyId key);
    void PenDown(int x, int y);
    void PenUp();

    void SetFramebufferDirty();
    bool SetFramebuffer(uint32_t start, uint32_t size);
    void ClearFramebufferDirty();

    void Sleep();
    void Wakeup(uint8_t wakeupSource);

    void SetAudioQueue(struct AudioQueue *audioQueue);
    void SetPcmOutputEnabled(bool pcmOutputEnabled);
    void SetPcmSuspended(bool pcmSuspended);

    Buffer GetRomData();

    struct Buffer GetNandData();
    struct Buffer GetNandDirtyPages();
    bool IsNandDirty();
    void SetNandDirty(bool isDirty);

    Buffer GetMemoryData();
    Buffer GetMemoryDirtyPages();

    virtual bool Save() = 0;
    virtual bool Load(size_t savestateSize, void *savestateData) = 0;
    struct Buffer GetSavestate();

    void SdInsert();
    bool SdRemount();
    void SdEject();
    bool SdInserted() { return cardInserted; }

    virtual void DumpMMU() = 0;

    struct ArmCpu *GetCpu() { return cpu; }
    struct SyscallDispatch *GetSyscallDispatch() { return syscallDispatch; };
    uint32_t GetRamSize() { return ramSize; };
    struct NAND *GetNand() { return nand; };
    struct SystemState *GetSystemState() { return systemState; };

    virtual void JamKey(enum KeyId key, uint32_t durationMsec) = 0;

    bool IsPacePatched();
    virtual uint64_t GetTime() = 0;

    // Actual SoC needs to implement those, the other virtuals are taken
    // care of in soc_generic.h
    virtual uint32_t *GetPendingFrame() = 0;
    virtual void ResetPendingFrame() = 0;
    virtual enum DeviceType5 GetDeviceType() = 0;
    virtual void SuspendTimerInterrupts(bool suspendInterrupts) = 0;
    virtual bool LcdEnabled() = 0;

   protected:
    // Actual SoC needs to implement those, the other virtuals are taken
    // care of in soc_generic.h
    virtual void OnSetFramebufferDirty() = 0;
    virtual void OnSleep() = 0;
    virtual void OnWakeup() = 0;
    virtual void OnSetAudioQueue(struct AudioQueue *audioQueue) = 0;
    virtual void OnSetPcmOutputEnabled() = 0;
    virtual void OnSetPcmSuspended() = 0;
    virtual void OnSdInsert() = 0;
    virtual void OnSdEject() = 0;

   protected:
    struct PenEvent {
        bool penDown;
        int x, y;

        static PenEvent PenDown(int x, int y);
        static PenEvent PenUp();
    };

    struct KeyEvent {
        bool keyDown;
        enum KeyId key;

        static KeyEvent KeyDown(enum KeyId key);
        static KeyEvent KeyUp(enum KeyId key);
    };

   protected:
    SoC();

   protected:
    ArmRom *rom{nullptr};
    ArmRam *ram{nullptr};
    ArmCpu *cpu{nullptr};
    ArmMem *mem{nullptr};

    NAND *nand{nullptr};
    VSD *vSD{nullptr};

    SystemState *systemState{nullptr};
    SyscallDispatch *syscallDispatch{nullptr};
    PacePatch *pacePatch{nullptr};
    PatchDispatch *patchDispatch{nullptr};
    PatchContext *patchContext{nullptr};

    MemoryBuffer bufferMemory{};

    std::unique_ptr<Savestate<ChunkType>> savestate;
    std::unique_ptr<Savestate<ChunkType>> powerOnState;

    std::unique_ptr<Queue<PenEvent>> penEventQueue;
    std::unique_ptr<Queue<KeyEvent>> keyEventQueue;
    struct AudioQueue *audioQueue;

    bool sleeping{false};
    bool framebufferDirty{true};
    bool pcmEnabled{false};
    bool pcmSuspended{false};
    uint64_t injectedTimeNsec{0};

    uint64_t eventQueueTicks{0};
    uint64_t eventQueueTicksAtPenDown{0};
    KeyId jammedKey{keyInvalid};
    uint64_t releaseJammedKeyAt{0};
    bool penDown{false};

    bool cardInserted{false};
    char cardId[SD_CARD_ID_MAX_LEN + 1]{};

    uint32_t ramBase{0};
    uint32_t ramSize{0};

   private:
    SoC(const SoC &) = delete;
    SoC(SoC &&) = delete;
    SoC &operator=(const SoC &) = delete;
    SoC &operator=(SoC &&) = delete;
};

#endif  // _SOC_H_
