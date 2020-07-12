#ifndef _EM_PATCH_MODULE_HTAL_
#define _EM_PATCH_MODULE_HTAL_

#include "EmCommon.h"
#include "EmPatchModule.h"

class EmPatchModuleHtal : public EmPatchModule {
   public:
    static constexpr uint16 kMagicRefNum = 0x666;

    virtual Err GetHeadpatch(uint16 index, HeadpatchProc& proc);
};

#endif  // _EM_PATCH_MODULE_HTAL_
