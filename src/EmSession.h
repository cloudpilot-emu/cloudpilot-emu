#ifndef _EM_SESSION_H_
#define _EM_SESSION_H_

#include "EmCommon.h"

class EmCPU;

class EmSession {
   public:
    Bool IsNested();

    Bool ExecuteSpecial(Bool checkForResetOnly);

    Bool CheckForBreak(void);

    void ScheduleResetBanks(void);

   private:
    bool bankResetScheduled{false};

    EmCPU* cpuInstance{nullptr};
};

extern EmSession* gSession;

#endif  // _EM_SESSION_H
