#ifndef _EM_SESSION_H_
#define _EM_SESSION_H_

#include <memory>

#include "EmCPU.h"
#include "EmCommon.h"
#include "EmDevice.h"

class EmSession {
   public:
    Bool IsNested();

    Bool ExecuteSpecial(Bool checkForResetOnly);

    Bool CheckForBreak(void);

    void ScheduleResetBanks(void);

    EmDevice& GetDevice();

   private:
    bool bankResetScheduled{false};

    unique_ptr<EmDevice> device{nullptr};

    unique_ptr<EmCPU> cpuInstance{nullptr};
};

extern EmSession* gSession;

#endif  // _EM_SESSION_H
