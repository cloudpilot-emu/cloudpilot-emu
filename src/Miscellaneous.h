#ifndef _MISCELLANEOUS_H_
#define _MISCELLANEOUS_H_

#include "EmCommon.h"
#include "EmStructs.h"

uint32 NextPowerOf2(uint32 x);

Bool GetSystemCallContext(emuptr, SystemCallContext&);

void SeparateList(StringList& stringList, string str, char delimiter);

Bool EndsWith(const char* s, const char* pattern);

string GetLibraryName(uint16 refNum);

class StMemoryMapper {
   public:
    StMemoryMapper(const void* memory, long size);
    ~StMemoryMapper(void);

   private:
    const void* fMemory;
};

template <class T>
class EmValueChanger {
   public:
    EmValueChanger(T& variable, T newValue) : fVariable(variable), fOrigValue(variable) {
        fVariable = newValue;
    }

    ~EmValueChanger() { fVariable = fOrigValue; }

   private:
    T& fVariable;
    T fOrigValue;
};

#endif
