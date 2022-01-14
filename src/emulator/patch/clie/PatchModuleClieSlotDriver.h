#ifndef _PATCH_MODULE_SONY_SLOT_DRIVER_H_
#define _PATCH_MODULE_SONY_SLOT_DRIVER_H_

#include "EmPatchModule.h"

class PatchModuleClieSlotDriver : public EmPatchModule {
   public:
    static constexpr const char* LIBNAME = "Slot Driver Library";

   public:
    PatchModuleClieSlotDriver();
};

#endif  // _PATCH_MODULE_SONY_SLOT_DRIVER_H_
