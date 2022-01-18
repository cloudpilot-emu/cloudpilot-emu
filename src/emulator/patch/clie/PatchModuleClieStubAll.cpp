#include "PatchModuleClieStubAll.h"

namespace {
    CallROMType HeadpatchStub(void) { return kSkipROM; }
}  // namespace

Err PatchModuleClieStubAll::GetHeadpatch(uint16 index, HeadpatchProc &procP) {
    procP = HeadpatchStub;
    return kPatchErrNone;
}

Err PatchModuleClieStubAll::GetTailpatch(uint16 index, TailpatchProc &procP) {
    procP = nullptr;
    return kPatchErrNone;
}

bool PatchModuleClieStubAll::HandlesLibrary(const string &libname) {
    if (libname == "Sony Sound Library") return true;
    if (libname == "Sony Irc Library") return true;

    return false;
}
