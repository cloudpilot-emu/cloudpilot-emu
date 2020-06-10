#include "EmPatchState.h"

EmPatchState gPatchState;

void EmPatchState::SetOSVersion(UInt32 version) { osVersion = version; }

UInt32 EmPatchState::OSVersion(void) {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return osVersion;
}

UInt32 EmPatchState::OSMajorMinorVersion(void) { return OSMajorVersion() * 10 + OSMinorVersion(); }

UInt32 EmPatchState::OSMajorVersion(void) {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return sysGetROMVerMajor(osVersion);
}

UInt32 EmPatchState::OSMinorVersion(void) {
    EmAssert(osVersion != kOSUndeterminedVersion);

    return sysGetROMVerMinor(osVersion);
}
