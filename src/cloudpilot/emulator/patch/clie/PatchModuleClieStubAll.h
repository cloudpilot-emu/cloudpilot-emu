#ifndef _PATCH_MODULE_CLIE_STUB_ALL_
#define _PATCH_MODULE_CLIE_STUB_ALL_

#include "EmCommon.h"
#include "EmPatchModule.h"

class PatchModuleClieStubAll : public EmPatchModule {
   public:
    virtual Err GetHeadpatch(uint16 index, HeadpatchProc &procP);
    virtual Err GetTailpatch(uint16 index, TailpatchProc &procP);

    static bool HandlesLibrary(const string &libname);
};

#endif  // _PATCH_MODULE_CLIE_STUB_ALL_
