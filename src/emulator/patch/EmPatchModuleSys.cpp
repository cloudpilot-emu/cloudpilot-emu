#include "EmPatchModuleSys.h"

#include "EmCommon.h"
#include "EmLowMem.h"
#include "EmPalmOS.h"
#include "EmPatchMgr.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Feature.h"
#include "Logging.h"
#include "Marshal.h"
#include "Miscellaneous.h"
#include "NetworkProxy.h"
#include "PenEvent.h"
#include "Platform.h"
#include "ROMStubs.h"
#include "SuspendContextClipboardCopy.h"
#include "SuspendContextClipboardPaste.h"
#include "SuspendManager.h"

// #define LOGGING
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

        EmPalmOS::InjectEvent(result);

        return result;
    }

    CallROMType HeadpatchSysSemaphoreWait(void) {
        // Only do this under 1.0.	Under Palm OS 2.0 and later, EvtGetSysEvent
        // calls SysEvGroupWait instead.  See our headpatch of that function
        // for a chunk of pretty similar code.

        if (gSystemState.OSMajorVersion() != 1) {
            return kExecuteROM;
        }

        // Err SysSemaphoreWait(UInt32 smID, UInt32 priority, Int32 timeout)

        CALLED_SETUP("Err", "UInt32 smID, UInt32 priority, Int32 timeout");

        CALLED_GET_PARAM_VAL(Int32, timeout);

        CallROMType result;

        EmPalmOS::InjectEvent(result);

        return result;
    }

    CallROMType HeadpatchPenScreenToRaw_RawToScreen(void) {
        EmAssert(gSession);
        if (gSession->GetDevice().HasCustomDigitizerTransform()) return kExecuteROM;

        CALLED_SETUP("Err", "PointType* point");

        CALLED_GET_PARAM_REF(PointType, point, Marshal::kInOut);

        TransformPenCoordinates((*point).x, (*point).y);

        CALLED_PUT_PARAM_REF(point);
        PUT_RESULT_VAL(Err, 0);

        return kSkipROM;
    }

    CallROMType HeadpatchPenCalibrate(void) {
        EmAssert(gSession);

        if (gSession->GetDevice().HasCustomDigitizerTransform()) {
            InstallCalibrationInfo();

            return kSkipROM;
        }

        return kExecuteROM;
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

    CallROMType HeadpatchClipboardGetItem() {
        EmAssert(gSession);

        if (Feature::GetClipboardIntegration() && !gSession->IsNested()) {
            SuspendManager::Suspend<SuspendContextClipboardPaste>();
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchDbgMessage() {
        CALLED_SETUP("void", "Char* aStr");

        CALLED_GET_PARAM_STR(Char, aStr);

        cout << "DbgMessage: " << static_cast<char*>(aStr) << endl << flush;

        return kSkipROM;
    }

    CallROMType HeadpatchHwrBatteryLevel(void) {
        EmAssert(gSession);

        if (!gSession->GetDevice().NeedsBatteryPatch()) {
            return kExecuteROM;
        }

        CALLED_SETUP("UInt16", "void");

        PUT_RESULT_VAL(UInt16, 255);  // Hardcode a maximum level

        return kSkipROM;
    }

    CallROMType HeadpatchHwrBattery(void) {
        EmAssert(gSession);

        if (!gSession->GetDevice().NeedsBatteryPatch()) {
            return kExecuteROM;
        }

        CALLED_SETUP("Err", "UInt16 cmd, void * cmdP");

        CALLED_GET_PARAM_VAL(UInt16, cmd);

        if (cmd == 2 /* hwrBatteryCmdMainRead */) {
            CALLED_GET_PARAM_REF(HwrBatCmdReadType, cmdP, Marshal::kInOut);

            if (gSession->GetDevice().HardwareID() == 0x0a /*halModelIDVisorPrism*/) {
                (*cmdP).mVolts = 4000;  // 4.0 volts
            } else {
                (*cmdP).mVolts = 2500;  // 2.5 volts
            }

            (*cmdP).abs = 255;  // Hardcode a maximum level

            cmdP.Put();

            PUT_RESULT_VAL(Err, errNone);
            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchSysLibLoad(void) {
        CALLED_SETUP("Err", "UInt32 libType, UInt32 libCreator, UInt16* refNumP");

        CALLED_GET_PARAM_VAL(UInt32, libType);
        CALLED_GET_PARAM_VAL(UInt32, libCreator);

        if (gSession->GetDevice().IsClie() && libType == sysResTLibrary && libCreator == 'SlMa') {
            // unsupported Audio Library on POSE
            PUT_RESULT_VAL(Err, sysErrLibNotFound);
            return kSkipROM;
        }

        return kExecuteROM;
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
        ::FtrSet('cldp', 0, 0x20150103);

#if 0  // CSTODO
       // Can't call PrefSetPreference on 1.0 devices....
        if (EmLowMem::TrapExists(sysTrapPrefSetPreference)) {
            ::PrefSetPreference(prefAutoOffDuration, 0);
        }
#endif

        SetHotSyncUserName(gSystemState.GetHotsyncUserName().c_str());

        gSystemState.SetUIInitialized();

        gNetworkProxy.Reset();

        PRINTF("syscall: UIInitialize");
    }

    void TailpatchEvtSysEventAvail() {
        // Boolean EvtSysEventAvail(Boolean ignorePenUps)

        CALLED_SETUP("Boolean", "Boolean ignorePenUps");

        CALLED_GET_PARAM_VAL(Boolean, ignorePenUps);
        GET_RESULT_VAL(Boolean);

        if (result == 0) {
            if (EmPalmOS::HasPenEvent()) {
                PenEvent event = EmPalmOS::PeekPenEvent();

                if (event.isPenDown() || !ignorePenUps) {
                    PUT_RESULT_VAL(Boolean, true);
                }
            }
        }
    }

    void DispatchCopyClipboard() {
        UInt16 length;
        emuptr dataHdl = ClipboardGetItem(clipboardText, &length);

        if (length > 0) {
            emuptr dataPtr = MemHandleLock(dataHdl);

            if (dataPtr) {
                char dataCopy[length + 1];
                dataCopy[length] = 0;

                EmMem_memcpy(static_cast<void*>(dataCopy), dataPtr, length);

                MemHandleUnlock(dataHdl);

                SuspendManager::Suspend<SuspendContextClipboardCopy>(Isolatin1ToUtf8(dataCopy));
            }
        } else {
            SuspendManager::Suspend<SuspendContextClipboardCopy>("");
        }
    }

    void TailpatchClipboardAddItem() {
        EmAssert(gSession);

        if (Feature::GetClipboardIntegration() && !gSession->IsNested()) DispatchCopyClipboard();
    }

    void TailpatchClipboardAppendItem() {
        EmAssert(gSession);

        if (Feature::GetClipboardIntegration() && !gSession->IsNested()) DispatchCopyClipboard();
    }

    CallROMType HeadpatchHwrIRQ4Handler() {
        // PalmOS will increment the date by one day if this is a rollover interrupt,
        // so make sure that the day is set in one day in the past. This should
        // make sure that PalmOS calculates programs the alarms correctly.
        SetCurrentDate(-1);

        return kExecuteROM;
    }

    void TailpatchHwrIRQ4Handler() {
        // If this wasn't a rollover interrupt we're gonne be one day in the past by now, so
        // make sure that the date is correct again.
        SetCurrentDate();
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
        {sysTrapPenScreenToRaw, HeadpatchPenScreenToRaw_RawToScreen, NULL},
        {sysTrapPenRawToScreen, HeadpatchPenScreenToRaw_RawToScreen, NULL},
        {sysTrapPenCalibrate, HeadpatchPenCalibrate, NULL},
        {sysTrapClipboardGetItem, HeadpatchClipboardGetItem, NULL},
        {sysTrapClipboardAddItem, NULL, TailpatchClipboardAddItem},
        {sysTrapClipboardAppendItem, NULL, TailpatchClipboardAppendItem},
        {sysTrapSysSemaphoreWait, HeadpatchSysSemaphoreWait, NULL},
        {sysTrapDbgMessage, HeadpatchDbgMessage, NULL},
        {sysTrapHwrBatteryLevel, HeadpatchHwrBatteryLevel, NULL},
        {sysTrapHwrBattery, HeadpatchHwrBattery, NULL},
        {sysTrapSysLibLoad, HeadpatchSysLibLoad, NULL},
        {sysTrapHwrIRQ4Handler, HeadpatchHwrIRQ4Handler, TailpatchHwrIRQ4Handler},
        {0, NULL, NULL}};
}  // namespace

EmPatchModuleSys::EmPatchModuleSys() { LoadProtoPatchTable(protoPatchTable); }
