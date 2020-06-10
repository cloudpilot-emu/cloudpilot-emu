#ifndef _EM_PATCH_STATE_H_
#define _EM_PATCH_STATE_H_

#include "EmCommon.h"

const UInt32 kOSUndeterminedVersion = ~0;

class EmPatchState {
   public:
    EmPatchState() = default;

    void SetOSVersion(UInt32 version);
    UInt32 OSVersion(void);
    UInt32 OSMajorMinorVersion(void);
    UInt32 OSMajorVersion(void);
    UInt32 OSMinorVersion(void);

   private:
    UInt32 osVersion{kOSUndeterminedVersion};
};

extern EmPatchState gPatchState;

#endif  // _EM_PATCH_STATE_H_
