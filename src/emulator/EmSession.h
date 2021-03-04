#ifndef _EM_SESSION_H_
#define _EM_SESSION_H_

#include <cstddef>
#include <memory>
#include <utility>

#include "ButtonEvent.h"
#include "EmCPU.h"
#include "EmCommon.h"
#include "EmDevice.h"
#include "EmEvent.h"
#include "EmThreadSafeQueue.h"
#include "KeyboardEvent.h"
#include "PenEvent.h"
#include "Savestate.h"

class SavestateLoader;
class SessionImage;

class EmSession {
   public:
    enum class ResetType : uint8 { sys = 0x01, soft = 0x02, noext = 0x03, hard = 0x04 };

   public:
    bool Initialize(EmDevice* device, const uint8* romImage, size_t romLength);

    pair<size_t, unique_ptr<uint8[]>> SaveImage();
    bool LoadImage(SessionImage& image);

    template <typename T>
    void Save(T& savestate);
    void Load(SavestateLoader& loader);

    bool Save();
    bool Load(size_t size, uint8* buffer);

    void Reset(ResetType);

    Savestate& GetSavestate();
    pair<size_t, uint8*> GetRomImage();

    uint32 RunEmulation(uint32 maxCycles = 10000);

    bool IsPowerOn();
    bool IsCpuStopped();

    EmDevice& GetDevice();
    uint32 GetRandomSeed() const;

    uint32 GetMemorySize() const;
    uint8* GetMemoryPtr() const;
    uint8* GetDirtyPagesPtr() const;

    void QueuePenEvent(PenEvent evt);
    void QueueKeyboardEvent(KeyboardEvent evt);
    void QueueButtonEvent(ButtonEvent evt);

    void SetHotsyncUserName(string hotsyncUserName);

    void SetClockDiv(uint32 clockDiv);
    uint32 GetClocksPerSecond() const { return clocksPerSecond; }
    uint64 GetSystemCycles() const { return systemCycles; }

    ///////////////////////////////////////////////////////////////////////////
    // Internal stuff
    ///////////////////////////////////////////////////////////////////////////

    void Deinitialize();

    bool IsNested() const;

    void ReleaseBootKeys();

    bool ExecuteSpecial(bool checkForResetOnly);
    bool CheckForBreak() const;
    void ScheduleResetBanks();
    void ScheduleReset(ResetType resetType);

    void ExecuteSubroutine();
    void ScheduleSubroutineReturn();

    bool RunToSyscall();
    bool WaitingForSyscall() const;
    void NotifySyscallDispatched();

    void YieldMemoryMgr();

    void HandleInstructionBreak();

    bool HasPenEvent();
    PenEvent NextPenEvent();
    PenEvent PeekPenEvent();

    bool HasKeyboardEvent();
    KeyboardEvent NextKeyboardEvent();

    bool HasButtonEvent();
    ButtonEvent NextButtonEvent();

   private:
    template <typename T>
    void DoSaveLoad(T& helper);

    bool Wakeup();

    void PumpEvents();
    bool PromoteKeyboardEvent();
    bool PromotePenEvent();

    void RecalculateClocksPerSecond();

    void CheckDayForRollover();

   private:
    bool bankResetScheduled{false};
    bool resetScheduled{false};
    ResetType resetType;

    int nestLevel{0};
    bool subroutineReturn{false};

    bool waitingForSyscall{false};
    bool syscallDispatched{false};

    bool isInitialized{false};
    shared_ptr<EmDevice> device{nullptr};
    unique_ptr<EmCPU> cpu{nullptr};
    typename EmEvent<>::HandleT onSystemClockChangeHandle;

    EmThreadSafeQueue<PenEvent> penEventQueue{20};
    EmThreadSafeQueue<KeyboardEvent> keyboardEventQueue{20};

    EmThreadSafeQueue<PenEvent> penEventQueueIncoming{20};
    EmThreadSafeQueue<KeyboardEvent> keyboardEventQueueIncoming{20};
    uint64 lastEventPromotedAt{0};

    EmThreadSafeQueue<ButtonEvent> buttonEventQueue{20};
    uint64 lastButtonEventReadAt{0};

    uint64 systemCycles{0};
    uint32 extraCycles{0};
    uint32 clockDiv{2};

    uint64 dayCheckedAt{0};
    uint32 dayAtLastClockSync{0};

    uint32 clocksPerSecond;

    bool holdingBootKeys;
    ResetType bootKeysType;

    unique_ptr<uint8[]> romImage;
    size_t romSize{0};
    Savestate savestate;
};

extern EmSession* gSession;

#endif  // _EM_SESSION_H
