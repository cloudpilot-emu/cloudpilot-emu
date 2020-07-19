#ifndef _EM_SYSTEM_STATE_H_
#define _EM_SYSTEM_STATE_H_

#include "EmCommon.h"
#include "EmEvent.h"

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

    void SetHotsyncUserName(string hotsyncUserName);
    string GetHotsyncUserName() const;

    bool IsScreenDirty() const;
    inline void MarkScreenDirty() { screenDirty = true; }
    void MarkScreenClean();

   public:
    EmEvent<> onMarkScreenClean;

   private:
    uint32 osVersion{kOSUndeterminedVersion};

    bool uiInitialized{false};

    string hotsyncUserName;

    bool screenDirty;
};

extern EmSystemState gSystemState;

#endif  // _EM_SYSTEM_STATE_H_
