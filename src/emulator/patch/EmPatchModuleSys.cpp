#include "EmPatchModuleSys.h"

#include "EmCommon.h"
#include "EmLowMem.h"
#include "EmPatchMgr.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Logging.h"
#include "Marshal.h"
#include "Miscellaneous.h"
#include "PenEvent.h"
#include "Platform.h"
#include "ROMStubs.h"

#define LOGGING 1
#ifdef LOGGING
    #define PRINTF logging::printf
#else
    #define PRINTF(...) ;
#endif

namespace {
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

    CallROMType HeadpatchSysUIAppSwitch() {
        gSystemState.SetSetupComplete();

        return kExecuteROM;
    }

    CallROMType HeadpatchHwrDockStatus(void) {
        // On non-328 devices, we emulate the hardware that returns this
        // information, so we don't need to patch this function.

        EmAssert(gSession);

        if (gSession->GetDevice().EmulatesDockStatus()) {
            return kExecuteROM;
        }

        // hwrDockStatusState HwrDockStatus(void)
        //
        //	(added in Palm OS 3.1)
        //	(changed later to return UInt16)

        CALLED_SETUP("UInt16", "void");

        // Old enumerated values from Hardware.h:
        //
        //		DockStatusNotDocked = 0,
        //		DockStatusInModem,
        //		DockStatusInCharger,
        //		DockStatusUnknown = 0xFF

        // New defines from HwrDock.h
#define hwrDockStatusUndocked 0x0000            // nothing is attached
#define hwrDockStatusModemAttached 0x0001       // some type of modem is attached
#define hwrDockStatusDockAttached 0x0002        // some type of dock is attached
#define hwrDockStatusUsingExternalPower 0x0004  // using some type of external power source
#define hwrDockStatusCharging 0x0008            // internal power cells are recharging

        PUT_RESULT_VAL(UInt16, hwrDockStatusUsingExternalPower);

        return kSkipROM;
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

        SetCurrentDate();

        PRINTF("syscall: TimInit");
    }

    void TailpatchUIInitialize(void) {
        ::SysSetAutoOffTime(0);
        EmLowMem::fgLowMem.globals.sysRandomSeed = Platform::Random();
        ::FtrSet('pose', 0, 0);

#if 0  // CSTODO
       // Can't call PrefSetPreference on 1.0 devices....
        if (EmLowMem::TrapExists(sysTrapPrefSetPreference)) {
            ::PrefSetPreference(prefAutoOffDuration, 0);
        }
#endif

        SetHotSyncUserName(gSystemState.GetHotsyncUserName().c_str());

        gSystemState.SetUIInitialized();

        PRINTF("syscall: UIInitialize");
    }

    void TailpatchEvtSysEventAvail(void) {
        // Boolean EvtSysEventAvail(Boolean ignorePenUps)

        CALLED_SETUP("Boolean", "Boolean ignorePenUps");

        CALLED_GET_PARAM_VAL(Boolean, ignorePenUps);
        GET_RESULT_VAL(Boolean);

        if (result == 0) {
            EmAssert(gSession);
            if (gSession->HasPenEvent()) {
                PenEvent event = gSession->PeekPenEvent();

                if (event.isPenDown() || !ignorePenUps) {
                    PUT_RESULT_VAL(Boolean, true);
                }
            }
        }
    }

    ProtoPatchTableEntry protoPatchTable[] = {
        {sysTrapDmInit, HeadpatchDmInit, NULL},
        {sysTrapSysUIAppSwitch, HeadpatchSysUIAppSwitch, NULL},
        {sysTrapSysEvGroupWait, HeadpatchSysEvGroupWait, NULL},
        {sysTrapFtrInit, NULL, TailpatchFtrInit},
        {sysTrapHwrMemReadable, NULL, TailpatchHwrMemReadable},
        {sysTrapTimInit, NULL, TailpatchTimInit},
        {sysTrapUIInitialize, NULL, TailpatchUIInitialize},
        {sysTrapEvtSysEventAvail, NULL, TailpatchEvtSysEventAvail},
        {sysTrapHwrDockStatus, HeadpatchHwrDockStatus, NULL},
        {0, NULL, NULL}};
}  // namespace

EmPatchModuleSys::EmPatchModuleSys() { LoadProtoPatchTable(protoPatchTable); }
