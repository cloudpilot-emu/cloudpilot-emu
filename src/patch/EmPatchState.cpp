#include "EmPatchState.h"

EmPatchState gPatchState;

void EmPatchState::SetOSVersion(uint32 version) { osVersion = version; }

uint32 EmPatchState::OSVersion(void) {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return osVersion;
}

uint32 EmPatchState::OSMajorMinorVersion(void) { return OSMajorVersion() * 10 + OSMinorVersion(); }

uint32 EmPatchState::OSMajorVersion(void) {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return sysGetROMVerMajor(osVersion);
}

uint32 EmPatchState::OSMinorVersion(void) {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return sysGetROMVerMinor(osVersion);
}
