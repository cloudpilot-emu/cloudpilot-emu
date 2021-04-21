#ifndef _DB_INSTALLER_H_
#define _DB_INSTALLER_H_

#include "EmCommon.h"

class DbInstaller {
   public:
    enum class Result : int { failure = -1, success = 1, needsReboot = 2 };

   public:
    static Result Install(size_t bufferSize, uint8* buffer);
};

#endif  // _DB_INSTALLER_H_
