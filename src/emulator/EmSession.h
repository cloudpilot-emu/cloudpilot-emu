#ifndef _EM_SESSION_H_
#define _EM_SESSION_H_

#include <cstddef>
#include <memory>

#include "ButtonEvent.h"
#include "EmCPU.h"
#include "EmCommon.h"
#include "EmDevice.h"
#include "EmThreadSafeQueue.h"
#include "KeyboardEvent.h"
#include "PenEvent.h"

class SavestateLoader;

class EmSession {
   public:
    enum class ResetType : uint8 { sys = 0x01, soft = 0x02, noext = 0x03, hard = 0x04 };

   public:
    bool Initialize(EmDevice* device, const uint8* romImage, size_t romLength);

    template <typename T>
    void Save(T& savestate);

    void Load(SavestateLoader& loader);

    bool IsNested() const;
    bool IsPowerOn();

    void ReleaseBootKeys();

    bool ExecuteSpecial(bool checkForResetOnly);

    bool CheckForBreak() const;

    void ScheduleResetBanks();

    void ScheduleReset(ResetType resetType);
    void Reset(ResetType);

    EmDevice& GetDevice();

    uint32 RunEmulation(uint32 maxCycles = 10000);

    void ExecuteSubroutine();
    void ScheduleSubroutineReturn();

    bool RunToSyscall();
    bool WaitingForSyscall() const;
    void NotifySyscallDispatched();

    void HandleInstructionBreak();

    void QueuePenEvent(PenEvent evt);
    bool HasPenEvent();
    PenEvent NextPenEvent();

    void QueueKeyboardEvent(KeyboardEvent evt);
    bool HasKeyboardEvent();
    KeyboardEvent NextKeyboardEvent();

    void QueueButtonEvent(ButtonEvent evt);
    bool HasButtonEvent();
    ButtonEvent NextButtonEvent();

    uint32 GetMemorySize() const;
    uint8* GetMemoryPtr() const;
    uint8* GetDirtyPagesPtr() const;

    uint32 GetRandomSeed() const;

    void SetHotsyncUserName(string hotsyncUserName);

    void SetClockDiv(uint32 clockDiv);
    uint32 GetClocksPerSecond() const { return clocksPerSecond; }

    uint64 GetSystemCycles() const { return systemCycles; }

   private:
    template <typename T>
    void DoSaveLoad(T& helper);

    bool Wakeup();

    void PumpEvents();
    bool PromoteKeyboardEvent();
    bool PromotePenEvent();

    void RecalculateClocksPerSecond();

   private:
    bool bankResetScheduled{false};
    bool resetScheduled{false};
    ResetType resetType;

    int nestLevel{0};
    bool subroutineReturn{false};

    bool waitingForSyscall{false};
    bool syscallDispatched{false};

    shared_ptr<EmDevice> device{nullptr};
    unique_ptr<EmCPU> cpu{nullptr};

    EmThreadSafeQueue<PenEvent> penEventQueue{20};
    EmThreadSafeQueue<KeyboardEvent> keyboardEventQueue{20};

    EmThreadSafeQueue<PenEvent> penEventQueueIncoming{20};
    EmThreadSafeQueue<KeyboardEvent> keyboardEventQueueIncoming{20};
    uint64 lastEventPromotedAt{0};

    EmThreadSafeQueue<ButtonEvent> buttonEventQueue{20};
    uint64 lastButtonEventReadAt{0};

    uint64 systemCycles{0};
    uint32 clockDiv{1};

    uint32 clocksPerSecond;

    bool holdingBootKeys;
    ResetType bootKeysType;
};

extern EmSession* gSession;

#endif  // _EM_SESSION_H
