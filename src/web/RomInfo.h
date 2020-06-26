#ifndef _ROM_INFO_H_
#define _ROM_INFO_H_

#include "EmCommon.h"

struct RomInfo {
    int CardVersion() const { return cardVersion; }
    const char* CardName() const { return cardName.c_str(); }
    int RomVersion() const { return romVersion; }
    const char* RomVersionString() const { return romVersionString.c_str(); }

    int cardVersion;
    string cardName;
    int romVersion;
    string romVersionString;
};

#endif  // _ROM_INFO_H_
