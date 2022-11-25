#include "EmPatchModuleHtal.h"

#include "Marshal.h"

namespace {
    CallROMType HeadpatchSendReply(void) {
        CALLED_SETUP("Err", "void");

        PUT_RESULT_VAL(Err, errNone);

        return kSkipROM;
    }
}  // namespace

Err EmPatchModuleHtal::GetHeadpatch(uint16 /* index */, HeadpatchProc& procP) {
    procP = HeadpatchSendReply;

    return kPatchErrNone;
}
