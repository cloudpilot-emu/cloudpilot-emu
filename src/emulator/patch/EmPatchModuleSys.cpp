#include "EmPatchModuleSys.h"

#include "EmCommon.h"
#include "EmLowMem.h"
#include "EmPatchMgr.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Logging.h"
#include "Marshal.h"
#include "Miscellaneous.h"
#include "Platform.h"
#include "ROMStubs.h"

#define LOGGING 1
#ifdef LOGGING
    #define PRINTF log::printf
#else
    #define PRINTF(...) ;
#endif

namespace {
    void PrvSetCurrentDate(void) {
        CEnableFullAccess munge;

        // Get the current date.

        uint32 year, month, day;
        Platform::GetDate(year, month, day);

        // Convert it to days -- and then hourse -- since 1/1/1904

        uint32 rtcHours = ::DateToDays(year, month, day) * 24;

        // Update the "hours adjustment" value to contain the current date.

        emuptr timGlobalsP = EmLowMem_GetGlobal(timGlobalsP);
        EmAliasTimGlobalsType<PAS> timGlobals(timGlobalsP);

        timGlobals.rtcHours = rtcHours;
    }

    CallROMType HeadpatchDmInit(void) {
        gSession->ReleaseBootKeys();

        PRINTF("syscall: DmInit");

        return kExecuteROM;
    }

    CallROMType HeadpatchSysEvGroupWait(void) {
        // Only do this under 2.0 and later.  Under Palm OS 1.0, EvtGetSysEvent
        // called SysSemaphoreWait instead.  See our headpatch of that function
        // for a chunk of pretty similar code.

        if (gSystemState.OSMajorVersion() == 1) {
            return kExecuteROM;
        }

        // Err SysEvGroupWait(UInt32 evID, UInt32 mask, UInt32 value, Int32 matchType,
        //						 Int32 timeout)

        CALLED_SETUP("Err",
                     "UInt32 evID, UInt32 mask, UInt32 value, Int32 matchType,"
                     "Int32 timeout");

        CALLED_GET_PARAM_VAL(Int32, timeout);

        CallROMType result = kExecuteROM;

        EmPatchMgr::PuppetString(result);

        return result;
    }

    void TailpatchFtrInit(void) {
        PRINTF("syscall: FtrInit");

        uint32 value;

        Err err = ::FtrGet(sysFtrCreator, sysFtrNumROMVersion, &value);

        if (err == errNone) {
            gSystemState.SetOSVersion(value);

            PRINTF("PalmOS version: %u.%u", gSystemState.OSMajorVersion(),
                   gSystemState.OSMinorVersion());
        } else {
            // EmSystemState::SetOSVersion(kOSUndeterminedVersion);
            PRINTF("vailed to determine PalmOS version");
        }
    }

    void TailpatchHwrMemReadable(void) {
        CALLED_SETUP("UInt32", "MemPtr address");

        CALLED_GET_PARAM_VAL(emuptr, address);
        GET_RESULT_VAL(uint32);

        if (result == 0) {
            void* addrStart;
            uint32 addrLen;

            Memory::GetMappingInfo(address, &addrStart, &addrLen);

            PUT_RESULT_VAL(uint32, addrLen);
        }

        PRINTF("syscall: HwrMemReadable for 0x%08x", address);
    }

    void TailpatchTimInit(void) {
        // Turn off the RTC bug workaround flag.

        CEnableFullAccess munge;

        emuptr timGlobalsP = EmLowMem_GetGlobal(timGlobalsP);
        EmAliasTimGlobalsType<PAS> timGlobals(timGlobalsP);

        if (timGlobals.rtcBugWorkaround == 0x01) {
            timGlobals.rtcBugWorkaround = 0;
        }

        // Set the current date.

        ::PrvSetCurrentDate();

        PRINTF("syscall: TimInit");
    }

    void TailpatchUIInitialize(void) {
        ::SysSetAutoOffTime(0);

#if ß  // CSTODO
       // Can't call PrefSetPreference on 1.0 devices....
        if (EmLowMem::TrapExists(sysTrapPrefSetPreference)) {
            ::PrefSetPreference(prefAutoOffDuration, 0);
        }
#endif
        ::FtrSet('pose', 0, 0);

#if 0  // CSTODO
        Preference<string> userName(kPrefKeyUserName);
        ::SetHotSyncUserName(userName->c_str());
#endif

        gSystemState.SetUIInitialized();

        PRINTF("syscall: UIInitialize");
    }

    ProtoPatchTableEntry protoPatchTable[] = {
        {sysTrapDmInit, HeadpatchDmInit, NULL},
        {sysTrapSysEvGroupWait, HeadpatchSysEvGroupWait, NULL},
        {sysTrapFtrInit, NULL, TailpatchFtrInit},
        {sysTrapHwrMemReadable, NULL, TailpatchHwrMemReadable},
        {sysTrapTimInit, NULL, TailpatchTimInit},
        {sysTrapUIInitialize, NULL, TailpatchUIInitialize},
        {0, NULL, NULL}};
}  // namespace

EmPatchModuleSys::EmPatchModuleSys() { LoadProtoPatchTable(protoPatchTable); }
