#ifndef _EM_SESSION_H_
#define _EM_SESSION_H_

#include <memory>

#include "EmCPU.h"
#include "EmCommon.h"
#include "EmDevice.h"
#include "EmThreadSafeQueue.h"
#include "KeyboardEvent.h"
#include "PenEvent.h"

class EmSession {
   public:
    Bool Initialize(EmDevice* device, const uint8* romImage, size_t romLength);

    Bool IsNested();

    void ReleaseBootKeys();

    Bool ExecuteSpecial(Bool checkForResetOnly);

    Bool CheckForBreak();

    void ScheduleResetBanks();

    void ScheduleReset(EmResetType resetType);

    EmDevice& GetDevice();

    uint32 RunEmulation(uint32 maxCycles = 10000);

    void ExecuteSubroutine();
    void ScheduleSubroutineReturn();

    void RunToSyscall();
    bool WaitingForSyscall();
    void NotifySyscallDispatched();

    void HandleInstructionBreak();

    void QueuePenEvent(PenEvent evt);
    bool HasPenEvent();
    PenEvent NextPenEvent();

    void QueueKeyboardEvent(KeyboardEvent evt);
    bool HasKeyboardEvent();
    KeyboardEvent NextKeyboardEvent();

   private:
    void Reset(EmResetType);

   private:
    bool bankResetScheduled{false};
    bool resetScheduled{false};
    EmResetType resetType;

    int nestLevel{0};
    bool suspendCpuSubroutineReturn{false};

    bool waitingForSyscall{false};
    bool syscallDispatched{false};

    shared_ptr<EmDevice> device{nullptr};
    unique_ptr<EmCPU> cpu{nullptr};

    EmThreadSafeQueue<PenEvent> penEventQueue{100};
    EmThreadSafeQueue<KeyboardEvent> keyboardEventQueue{100};

    uint32 additionalCycles{0};
};

extern EmSession* gSession;

#endif  // _EM_SESSION_H
