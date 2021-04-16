#include "PatchModuleNetlib.h"

#include "EmCommon.h"
#include "Marshal.h"

namespace {
    CallROMType HeadpatchNetLibOpen(void) {
        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 *netIFErrsP");

        // Get the stack-based parameters.
        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_REF(UInt16, netIFErrsP, Marshal::kOutput);

        cout << "netlib: NetLibOpen" << endl << flush;

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibOpenConfig(void) {
        //	Err NetLibOpenConfig( UInt16 libRefNum, UInt16 configIndex, UInt32 openFlags,
        //						UInt16 *netIFErrP)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt16 configIndex, UInt32 openFlags,"
                     "UInt16 *netIFErrP");

        // Get the stack-based parameters.
        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(UInt16, configIndex);
        CALLED_GET_PARAM_VAL(UInt32, openFlags);
        CALLED_GET_PARAM_REF(UInt16, netIFErrP, Marshal::kOutput);

        cout << "netlib: NetLibOpenConfig" << endl << flush;

        return kExecuteROM;
    }

    ProtoPatchTableEntry protoPatchTable[] = {{sysLibTrapOpen, HeadpatchNetLibOpen, NULL},
                                              {netLibOpenConfig, HeadpatchNetLibOpenConfig, NULL},
                                              {0, NULL, NULL}};
}  // namespace

PatchModuleNetlib::PatchModuleNetlib() { LoadProtoPatchTable(protoPatchTable); }
