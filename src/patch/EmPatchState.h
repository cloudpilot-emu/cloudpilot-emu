#ifndef _EM_PATCH_STATE_H_
#define _EM_PATCH_STATE_H_

#include "EmCommon.h"

const uint32 kOSUndeterminedVersion = ~0;

class EmPatchState {
   public:
    EmPatchState() = default;

    void SetOSVersion(uint32 version);
    uint32 OSVersion(void);
    uint32 OSMajorMinorVersion(void);
    uint32 OSMajorVersion(void);
    uint32 OSMinorVersion(void);

   private:
    uint32 osVersion{kOSUndeterminedVersion};
};

extern EmPatchState gPatchState;

#endif  // _EM_PATCH_STATE_H_
