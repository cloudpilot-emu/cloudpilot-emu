#ifndef _PATCH_MODULE_NETLIB_H_
#define _PATCH_MODULE_NETLIB_H_

#include "EmPatchModule.h"

class PatchModuleNetlib : public EmPatchModule {
   public:
    static constexpr const char* LIBNAME = "Net.lib";

   public:
    PatchModuleNetlib();
};

#endif  // _PATCH_MODULE_NETLIB_H_
