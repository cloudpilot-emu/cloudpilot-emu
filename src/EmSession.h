#ifndef _EM_SESSION_H_
#define _EM_SESSION_H_

#include <memory>

#include "EmCPU.h"
#include "EmCommon.h"
#include "EmDevice.h"

class EmSession {
   public:
    Bool Initialize(EmDevice* device, const uint8* romImage, size_t romLength);

    Bool IsNested();

    Bool ExecuteSpecial(Bool checkForResetOnly);

    Bool CheckForBreak();

    void ScheduleResetBanks();

    void ScheduleReset(EmResetType resetType);

    void ScheduleSubroutineReturn();

    EmDevice& GetDevice();

    uint32 RunEmulation(uint32 maxCycles = 10000);

    void ExecuteSubroutine();

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
};

extern EmSession* gSession;

#endif  // _EM_SESSION_H
