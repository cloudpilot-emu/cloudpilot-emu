#include "EmPatchModuleSys.h"

#include "EmCommon.h"
#include "EmSession.h"
#include "Logging.h"

#define LOGGING 1
#ifdef LOGGING
    #define PRINTF log::printf
#else
    #define PRINTF(...) ;
#endif

namespace {
    CallROMType HeadpatchDmInit(void) {
        gSession->ReleaseBootKeys();

        PRINTF("syscall: DmInit");

        return kExecuteROM;
    }

    void TailpatchFtrInit(void) { PRINTF("syscall: FtrInit"); }

    ProtoPatchTableEntry protoPatchTable[] = {{sysTrapDmInit, HeadpatchDmInit, NULL},
                                              {sysTrapFtrInit, NULL, TailpatchFtrInit},
                                              {0, NULL, NULL}};
}  // namespace

EmPatchModuleSys::EmPatchModuleSys() { LoadProtoPatchTable(protoPatchTable); }
