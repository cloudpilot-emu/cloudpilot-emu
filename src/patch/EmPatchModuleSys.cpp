#include "EmPatchModuleSys.h"

#include "EmCommon.h"
#include "EmSession.h"
#include "Logging.h"
#include "ROMStubs.h"

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

    void TailpatchFtrInit(void) {
        PRINTF("syscall: FtrInit");

        /*
                UInt32 value;

                Err err = ::FtrGet(sysFtrCreator, sysFtrNumROMVersion, &value);

                if (err == errNone) {
                    // EmPatchState::SetOSVersion(value);
                    PRINTF("ROM version: %u", value);
                } else {
                    // EmPatchState::SetOSVersion(kOSUndeterminedVersion);
                    PRINTF("vailed to determine ROM version");
                }
                */
    }

    ProtoPatchTableEntry protoPatchTable[] = {{sysTrapDmInit, HeadpatchDmInit, NULL},
                                              {sysTrapFtrInit, NULL, TailpatchFtrInit},
                                              {0, NULL, NULL}};
}  // namespace

EmPatchModuleSys::EmPatchModuleSys() { LoadProtoPatchTable(protoPatchTable); }
