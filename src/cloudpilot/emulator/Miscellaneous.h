#ifndef _MISCELLANEOUS_H_
#define _MISCELLANEOUS_H_

#include <functional>

#include "EmCommon.h"
#include "EmStructs.h"

enum GetDatabaseFlags : uint32 {
    kDatabaseFlagsNone = 0x00,
    kApplicationsOnly = 0x01,
    kOnlyRamDatabases = 0x02,
    kGetExtraInfo = 0x04
};

uint32 NextPowerOf2(uint32 x);

Bool GetSystemCallContext(emuptr, SystemCallContext&);

void SeparateList(StringList& stringList, string str, char delimiter);

Bool EndsWith(const char* s, const char* pattern);

string GetLibraryName(uint16 refNum);

uint32 DateToDays(uint32 year, uint32 month, uint32 day);

void SetHotSyncUserName(const char*);

void SetCurrentDate(int32 adjust = 0);

bool GetDatabases(DatabaseInfoList& appList, uint32 flags);

bool IsExecutable(const DatabaseInfo& dbInfo);

string Utf8ToIsolatin1(const string& input, size_t maxLen = 0xffff);
string Isolatin1ToUtf8(const string& input);

void TransformPenCoordinates(int16& x, int16& y);

void InstallCalibrationInfo(void);

bool LaunchAppByName(const string& name);

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
