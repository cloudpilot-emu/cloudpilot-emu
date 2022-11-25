#ifndef _ROM_INFO_H_
#define _ROM_INFO_H_

#include "EmCommon.h"
#include "EmROMReader.h"

class RomInfo {
   public:
    RomInfo(void* buffer, size_t size);

    bool IsValid() const;

    int CardVersion();
    const char* CardName();
    int RomVersion();
    const char* RomVersionString();

    bool Supports(string deviceId);

   private:
    EmROMReader romReader;
    bool valid;

    string cardName;
    string romVersionString;
};

#endif  // _ROM_INFO_H_
