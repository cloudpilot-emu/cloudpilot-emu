#ifndef _EM_SESSION_H_
#define _EM_SESSION_H_

#include <memory>

#include "EmCPU.h"
#include "EmCommon.h"
#include "EmDevice.h"
#include "EmThreadSafeQueue.h"
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

    void HandleInstructionBreak();

    void QueuePenEvent(PenEvent evt);
    bool HasPenEvent();
    PenEvent NextPenEvent();

   private:
    void Reset(EmResetType);

   private:
    bool bankResetScheduled{false};
    bool resetScheduled{false};
    EmResetType resetType;

    int nestLevel{0};
    bool suspendCpuSubroutineReturn{false};

    shared_ptr<EmDevice> device{nullptr};
    unique_ptr<EmCPU> cpu{nullptr};

    EmThreadSafeQueue<PenEvent> penEventQueue{100};
};

extern EmSession* gSession;

#endif  // _EM_SESSION_H
