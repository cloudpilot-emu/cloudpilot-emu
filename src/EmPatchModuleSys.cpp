#include "EmPatchModuleSys.h"

#include "EmCommon.h"
#include "EmSession.h"

namespace {
    CallROMType HeadpatchDmInit(void) {
        gSession->ReleaseBootKeys();

        return kExecuteROM;
    }

    ProtoPatchTableEntry protoPatchTable[] = {{sysTrapDmInit, HeadpatchDmInit, NULL},
                                              {0, NULL, NULL}};
}  // namespace

EmPatchModuleSys::EmPatchModuleSys() { LoadProtoPatchTable(protoPatchTable); }
