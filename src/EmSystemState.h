#ifndef _EM_SYSTEM_STATE_H_
#define _EM_SYSTEM_STATE_H_

#include "EmCommon.h"

const uint32 kOSUndeterminedVersion = ~0;

class EmSystemState {
   public:
    EmSystemState() = default;

    void Reset();

    void SetOSVersion(uint32 version);
    uint32 OSVersion(void) const;
    uint32 OSMajorMinorVersion(void) const;
    uint32 OSMajorVersion(void) const;
    uint32 OSMinorVersion(void) const;

    void SetUIInitialized();
    bool IsUIInitialized() const;

   private:
    uint32 osVersion{kOSUndeterminedVersion};
    bool uiInitialized{false};
};

extern EmSystemState gSystemState;

#endif  // _EM_SYSTEM_STATE_H_
