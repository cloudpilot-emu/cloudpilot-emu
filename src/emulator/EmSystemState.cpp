#include "EmSystemState.h"

EmSystemState gSystemState;

void EmSystemState::Reset() { uiInitialized = false; }

void EmSystemState::SetOSVersion(uint32 version) { osVersion = version; }

uint32 EmSystemState::OSVersion(void) const {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return osVersion;
}

uint32 EmSystemState::OSMajorMinorVersion(void) const {
    return OSMajorVersion() * 10 + OSMinorVersion();
}

uint32 EmSystemState::OSMajorVersion(void) const {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return sysGetROMVerMajor(osVersion);
}

uint32 EmSystemState::OSMinorVersion(void) const {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return sysGetROMVerMinor(osVersion);
}

void EmSystemState::SetUIInitialized() { uiInitialized = true; }

bool EmSystemState::IsUIInitialized() const { return uiInitialized; }

void EmSystemState::SetHotsyncUserName(string hotsyncUserName) {
    this->hotsyncUserName = hotsyncUserName;
}
string EmSystemState::GetHotsyncUserName() const { return hotsyncUserName; };
