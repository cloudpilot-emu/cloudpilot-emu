#ifndef _EM_SESSION_H_
#define _EM_SESSION_H_

#include "EmCommon.h"

class EmSession {
   public:
    Bool IsNested();

    Bool ExecuteSpecial(Bool checkForResetOnly);

    Bool CheckForBreak(void);
};

#endif  // _EM_SESSION_H
