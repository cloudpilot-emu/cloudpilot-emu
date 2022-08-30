#include "ROMStubs.h"

#include "EmBankMapped.h"  // EmBankMapped::GetEmulatedAddress
#include "EmCommon.h"
#include "EmSubroutine.h"  // EmSubroutine
#include "Marshal.h"       // CALLER_PUT_PARAM_VAL

Err FtrGet(UInt32 creator, UInt16 featureNum, UInt32* valueP) {
    // Prepare the stack.
    CALLER_SETUP("Err", "UInt32 creator, UInt16 featureNum, UInt32* valueP");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt32, creator);
    CALLER_PUT_PARAM_VAL(UInt16, featureNum);
    CALLER_PUT_PARAM_REF(UInt32, valueP, Marshal::kInOut);

    // Call the function.
    sub.Call(sysTrapFtrGet);

    // Write back any "by ref" parameters.
    CALLER_GET_PARAM_REF(valueP);

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err FtrSet(UInt32 creator, UInt16 featureNum, UInt32 newValue) {
    // Prepare the stack.
    CALLER_SETUP("Err", "UInt32 creator, UInt16 featureNum, UInt32 newValue");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt32, creator);
    CALLER_PUT_PARAM_VAL(UInt16, featureNum);
    CALLER_PUT_PARAM_VAL(UInt32, newValue);

    // Call the function.
    sub.Call(sysTrapFtrSet);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

UInt16 SysSetAutoOffTime(UInt16 seconds) {
    // Prepare the stack.
    CALLER_SETUP("UInt16", "UInt16 seconds");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, seconds);

    // Call the function.
    sub.Call(sysTrapSysSetAutoOffTime);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(UInt16);
}

Err PenScreenToRaw(PointType* penP) {
    // Prepare the stack.
    CALLER_SETUP("Err", "PointType* penP");

    // Set the parameters.
    CALLER_PUT_PARAM_REF(PointType, penP, Marshal::kInOut);

    // Call the function.
    sub.Call(sysTrapPenScreenToRaw);

    // Write back any "by ref" parameters.
    CALLER_GET_PARAM_REF(penP);

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err EvtEnqueuePenPoint(PointType* ptP) {
    // Prepare the stack.
    CALLER_SETUP("Err", "PointType* ptP");

    // Set the parameters.
    CALLER_PUT_PARAM_REF(PointType, ptP, Marshal::kInput);

    // Call the function.
    sub.Call(sysTrapEvtEnqueuePenPoint);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err EvtWakeup(void) {
    // Prepare the stack.
    CALLER_SETUP("Err", "void");

    // Set the parameters.

    // Call the function.
    sub.Call(sysTrapEvtWakeup);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err EvtWakeupWithoutNilEvent(void) {
    // Prepare the stack.
    CALLER_SETUP("Err", "void");

    // Set the parameters.

    // Call the function.
    sub.Call(sysTrapEvtWakeupWithoutNilEvent);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err EvtEnqueueKey(UInt16 ascii, UInt16 keycode, UInt16 modifiers) {
    // Prepare the stack.
    CALLER_SETUP("Err", "UInt16 ascii, UInt16 keycode, UInt16 modifiers");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, ascii);
    CALLER_PUT_PARAM_VAL(UInt16, keycode);
    CALLER_PUT_PARAM_VAL(UInt16, modifiers);

    // Call the function.
    sub.Call(sysTrapEvtEnqueueKey);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err SysKernelInfo(MemPtr p) {
    // Prepare the stack.
    CALLER_SETUP("Err", "SysKernelInfoPtr p");

    // Set the parameters.
    CALLER_PUT_PARAM_REF(SysKernelInfoType, p, Marshal::kInOut);

    // Call the function.
    sub.Call(sysTrapSysKernelInfo);

    // Write back any "by ref" parameters.
    CALLER_GET_PARAM_REF(p);

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

LocalID DmFindDatabase(UInt16 cardNo, const Char* nameP) {
    // Prepare the stack.
    CALLER_SETUP("LocalID", "UInt16 cardNo, const Char* nameP");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_STR(Char, nameP);

    // Call the function.
    sub.Call(sysTrapDmFindDatabase);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(LocalID);
}

Err DmDeleteDatabase(UInt16 cardNo, LocalID dbID) {
    // Prepare the stack.
    CALLER_SETUP("Err", "UInt16 cardNo, LocalID dbID");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_VAL(LocalID, dbID);

    // Call the function.
    sub.Call(sysTrapDmDeleteDatabase);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err DmCreateDatabase(UInt16 cardNo, const Char* const nameP, UInt32 creator, UInt32 type,
                     Boolean resDB) {
    // Prepare the stack.
    CALLER_SETUP("Err",
                 "UInt16 cardNo, const Char * const nameP, "
                 "UInt32 creator, UInt32 type, Boolean resDB");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_STR(Char, nameP);
    CALLER_PUT_PARAM_VAL(UInt32, creator);
    CALLER_PUT_PARAM_VAL(UInt32, type);
    CALLER_PUT_PARAM_VAL(Boolean, resDB);

    // Call the function.
    sub.Call(sysTrapDmCreateDatabase);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err DmGetLastErr(void) {
    // Prepare the stack.
    CALLER_SETUP("Err", "void");

    // Set the parameters.

    // Call the function.
    sub.Call(sysTrapDmGetLastErr);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err DmSetDatabaseInfo(UInt16 cardNo, LocalID dbID, const Char* nameP, UInt16* attributesP,
                      UInt16* versionP, UInt32* crDateP, UInt32* modDateP, UInt32* bckUpDateP,
                      UInt32* modNumP, LocalID* appInfoIDP, LocalID* sortInfoIDP, UInt32* typeP,
                      UInt32* creatorP) {
    // Prepare the stack.
    CALLER_SETUP("Err",
                 "UInt16 cardNo, LocalID dbID, const Char* nameP,"
                 "UInt16* attributesP, UInt16* versionP, UInt32* crDateP,"
                 "UInt32* modDateP, UInt32* bckUpDateP,"
                 "UInt32* modNumP, LocalID* appInfoIDP,"
                 "LocalID* sortInfoIDP, UInt32* typeP,"
                 "UInt32* creatorP");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_VAL(LocalID, dbID);
    CALLER_PUT_PARAM_STR(Char, nameP);
    CALLER_PUT_PARAM_REF(UInt16, attributesP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(UInt16, versionP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(UInt32, crDateP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(UInt32, modDateP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(UInt32, bckUpDateP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(UInt32, modNumP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(LocalID, appInfoIDP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(LocalID, sortInfoIDP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(UInt32, typeP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(UInt32, creatorP, Marshal::kInput);

    // Call the function.
    sub.Call(sysTrapDmSetDatabaseInfo);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

emuptr DmOpenDatabase(UInt16 cardNo, LocalID dbID, UInt16 mode) {
    // Prepare the stack.
    CALLER_SETUP("DmOpenRef", "UInt16 cardNo, LocalID dbID, UInt16 mode");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_VAL(LocalID, dbID);
    CALLER_PUT_PARAM_VAL(UInt16, mode);

    // Call the function.
    sub.Call(sysTrapDmOpenDatabase);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_PTR(emuptr);
}

emuptr DmNewHandle(emuptr dbR, UInt32 size) {
    // Prepare the stack.
    CALLER_SETUP("MemHandle", "DmOpenRef dbR, UInt32 size");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, dbR);
    CALLER_PUT_PARAM_VAL(UInt32, size);

    // Call the function.
    sub.Call(sysTrapDmNewHandle);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_PTR(emuptr);
}

emuptr MemHandleLock(emuptr h) {
    // Prepare the stack.
    CALLER_SETUP("MemPtr", "MemHandle h");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, h);

    // Call the function.
    sub.Call(sysTrapMemHandleLock);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_PTR(emuptr);
}

Err DmWrite(emuptr recordP, UInt32 offset, const void* const srcP, UInt32 bytes) {
    // Prepare the stack.
    CALLER_SETUP("Err", "MemPtr recordP, UInt32 offset, const void * const srcP, UInt32 bytes");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, recordP);
    CALLER_PUT_PARAM_VAL(UInt32, offset);
    CALLER_PUT_PARAM_PTR(void, srcP, bytes, Marshal::kInput);
    CALLER_PUT_PARAM_VAL(UInt32, bytes);

    // Call the function.
    sub.Call(sysTrapDmWrite);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err MemHandleUnlock(emuptr h) {
    // Prepare the stack.
    CALLER_SETUP("Err", "MemHandle h");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, h);

    // Call the function.
    sub.Call(sysTrapMemHandleUnlock);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

LocalID MemHandleToLocalID(emuptr h) {
    // Prepare the stack.
    CALLER_SETUP("LocalID", "MemHandle h");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, h);

    // Call the function.
    sub.Call(sysTrapMemHandleToLocalID);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(LocalID);
}

emuptr DmNewResource(emuptr dbR, DmResType resType, DmResID resID, UInt32 size) {
    // Prepare the stack.
    CALLER_SETUP("MemHandle", "DmOpenRef dbR, DmResType resType, DmResID resID, UInt32 size");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, dbR);
    CALLER_PUT_PARAM_VAL(DmResType, resType);
    CALLER_PUT_PARAM_VAL(DmResID, resID);
    CALLER_PUT_PARAM_VAL(UInt32, size);

    // Call the function.
    sub.Call(sysTrapDmNewResource);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_PTR(emuptr);
}

Err DmReleaseResource(emuptr resourceH) {
    // Prepare the stack.
    CALLER_SETUP("Err", "MemHandle resourceH");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, resourceH);

    // Call the function.
    sub.Call(sysTrapDmReleaseResource);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

emuptr DmNewRecord(emuptr dbR, UInt16* atP, UInt32 size) {
    // Prepare the stack.
    CALLER_SETUP("MemHandle", "DmOpenRef dbR, UInt16* atP, UInt32 size");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, dbR);
    CALLER_PUT_PARAM_REF(UInt16, atP, Marshal::kInOut);
    CALLER_PUT_PARAM_VAL(UInt32, size);

    // Call the function.
    sub.Call(sysTrapDmNewRecord);

    // Write back any "by ref" parameters.
    CALLER_GET_PARAM_REF(atP);

    // Return the result.
    RETURN_RESULT_PTR(emuptr);
}

Err DmSetRecordInfo(emuptr dbR, UInt16 index, UInt16* attrP, UInt32* uniqueIDP) {
    // Prepare the stack.
    CALLER_SETUP("Err", "DmOpenRef dbR, UInt16 index, UInt16* attrP, UInt32* uniqueIDP");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, dbR);
    CALLER_PUT_PARAM_VAL(UInt16, index);
    CALLER_PUT_PARAM_REF(UInt16, attrP, Marshal::kInput);
    CALLER_PUT_PARAM_REF(UInt32, uniqueIDP, Marshal::kInput);

    // Call the function.
    sub.Call(sysTrapDmSetRecordInfo);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err DmReleaseRecord(emuptr dbR, UInt16 index, Boolean dirty) {
    // Prepare the stack.
    CALLER_SETUP("Err", "DmOpenRef dbR, UInt16 index, Boolean dirty");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, dbR);
    CALLER_PUT_PARAM_VAL(UInt16, index);
    CALLER_PUT_PARAM_VAL(Boolean, dirty);

    // Call the function.
    sub.Call(sysTrapDmReleaseRecord);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err DmCloseDatabase(emuptr dbR) {
    // Prepare the stack.
    CALLER_SETUP("Err", "DmOpenRef dbR");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, dbR);

    // Call the function.
    sub.Call(sysTrapDmCloseDatabase);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err DlkDispatchRequest(DlkServerSessionPtr sessP) {
    // Prepare the stack.
    CALLER_SETUP("Err", "DlkServerSessionType* sessP");

    // Set the parameters.
    CALLER_PUT_PARAM_REF(DlkServerSessionType, sessP, Marshal::kInput);

    // Call the function.
    sub.Call(sysTrapDlkDispatchRequest);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

UInt16 MemNumCards(void) {
    // Prepare the stack.
    CALLER_SETUP("UInt16", "void");

    // Set the parameters.

    // Call the function.
    sub.Call(sysTrapMemNumCards);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(UInt16);
}

UInt16 DmNumDatabases(UInt16 cardNo) {
    // Prepare the stack.
    CALLER_SETUP("UInt16", "UInt16 cardNo");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);

    // Call the function.
    sub.Call(sysTrapDmNumDatabases);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(UInt16);
}

LocalID DmGetDatabase(UInt16 cardNo, UInt16 index) {
    // Prepare the stack.
    CALLER_SETUP("LocalID", "UInt16 cardNo, UInt16 index");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_VAL(UInt16, index);

    // Call the function.
    sub.Call(sysTrapDmGetDatabase);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(LocalID);
}

emuptr DmGet1Resource(DmResType type, DmResID id) {
    // Prepare the stack.
    CALLER_SETUP("MemHandle", "DmResType type, DmResID id");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(DmResType, type);
    CALLER_PUT_PARAM_VAL(DmResID, id);

    // Call the function.
    sub.Call(sysTrapDmGet1Resource);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_PTR(emuptr);
}

LocalIDKind MemLocalIDKind(LocalID local) {
    // Prepare the stack.
    CALLER_SETUP("LocalIDKind", "LocalID local");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(LocalID, local);

    // Call the function.
    sub.Call(sysTrapMemLocalIDKind);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(LocalIDKind);
}

emuptr MemLocalIDToGlobal(LocalID local, UInt16 cardNo) {
    // Prepare the stack.
    CALLER_SETUP("MemPtr", "LocalID local, UInt16 cardNo");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(LocalID, local);
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);

    // Call the function.
    sub.Call(sysTrapMemLocalIDToGlobal);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_PTR(emuptr);
}

emuptr MemPtrSize(emuptr p) {
    // Prepare the stack.
    CALLER_SETUP("UInt32", "MemPtr p");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(emuptr, p);

    // Call the function.
    sub.Call(sysTrapMemPtrSize);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(emuptr);
}

Err DmDatabaseInfo(UInt16 cardNo, LocalID dbID, Char* nameP, UInt16* attributesP, UInt16* versionP,
                   UInt32* crDateP, UInt32* modDateP, UInt32* bckUpDateP, UInt32* modNumP,
                   LocalID* appInfoIDP, LocalID* sortInfoIDP, UInt32* typeP, UInt32* creatorP) {
    // Prepare the stack.
    CALLER_SETUP("Err",
                 "UInt16 cardNo, LocalID	dbID, Char* nameP,"
                 "UInt16* attributesP, UInt16* versionP, UInt32* crDateP,"
                 "UInt32* modDateP, UInt32* bckUpDateP,"
                 "UInt32* modNumP, LocalID* appInfoIDP,"
                 "LocalID* sortInfoIDP, UInt32* typeP,"
                 "UInt32* creatorP");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_VAL(LocalID, dbID);
    CALLER_PUT_PARAM_PTR(Char, nameP, dmDBNameLength, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(UInt16, attributesP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(UInt16, versionP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(UInt32, crDateP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(UInt32, modDateP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(UInt32, bckUpDateP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(UInt32, modNumP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(LocalID, appInfoIDP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(LocalID, sortInfoIDP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(UInt32, typeP, Marshal::kInOut);
    CALLER_PUT_PARAM_REF(UInt32, creatorP, Marshal::kInOut);

    // Call the function.
    sub.Call(sysTrapDmDatabaseInfo);

    // Write back any "by ref" parameters.
    CALLER_GET_PARAM_REF(attributesP);
    CALLER_GET_PARAM_REF(versionP);
    CALLER_GET_PARAM_REF(crDateP);
    CALLER_GET_PARAM_REF(modDateP);
    CALLER_GET_PARAM_REF(bckUpDateP);
    CALLER_GET_PARAM_REF(modNumP);
    CALLER_GET_PARAM_REF(appInfoIDP);
    CALLER_GET_PARAM_REF(sortInfoIDP);
    CALLER_GET_PARAM_REF(typeP);
    CALLER_GET_PARAM_REF(creatorP);

    // Return the result.
    RETURN_RESULT_VAL(Err);
}

Err ExgDBWrite(emuptr writeProcP, emuptr userDataP, const Char* nameP, LocalID dbID,
               UInt16 cardNo) {
    CALLER_SETUP("Err",
                 "ExgDBWriteProcPtr writeProcP, void* userDataP, const char* nameP, LocalID dbID, "
                 "UInt16 cardNo");

    CALLER_PUT_PARAM_VAL(emuptr, writeProcP);
    CALLER_PUT_PARAM_VAL(emuptr, userDataP);
    CALLER_PUT_PARAM_STR(Char, nameP);
    CALLER_PUT_PARAM_VAL(LocalID, dbID);
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);

    sub.Call(sysTrapExgDBWrite);

    RETURN_RESULT_VAL(Err);
}

Err ExgDBRead(emuptr readProcP, emuptr deleteProcP, emuptr userDataP, LocalID* dbIDP, UInt16 cardNo,
              Boolean* needResetP, Boolean keepDates) {
    CALLER_SETUP(
        "Err",
        "ExgDBReadProcPtr readProcP, ExgDBDeleteProcPtr deleteProcP, void* userDataP, LocalID* "
        "dbIDP, UInt16 cardNo, Boolean* needResetP, Boolean keepDates");

    CALLER_PUT_PARAM_VAL(emuptr, readProcP);
    CALLER_PUT_PARAM_VAL(emuptr, deleteProcP);
    CALLER_PUT_PARAM_VAL(emuptr, userDataP);
    CALLER_PUT_PARAM_REF(LocalID, dbIDP, Marshal::kOutput);
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_REF(Boolean, needResetP, Marshal::kOutput);
    CALLER_PUT_PARAM_VAL(Boolean, keepDates);

    sub.Call(sysTrapExgDBRead);

    CALLER_GET_PARAM_REF(dbIDP);
    CALLER_GET_PARAM_REF(needResetP);

    RETURN_RESULT_VAL(Err);
}

void ClipboardAddItem(const ClipboardFormatType format, const void* ptr, UInt16 length) {
    // Prepare the stack.
    CALLER_SETUP("void", "const ClipboardFormatType format, const void* ptr, UInt16 length");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(ClipboardFormatType, format);
    CALLER_PUT_PARAM_PTR(void, ptr, length, Marshal::kInput);
    CALLER_PUT_PARAM_VAL(UInt16, length);

    // Call the function.
    sub.Call(sysTrapClipboardAddItem);

    // Write back any "by ref" parameters.

    // Return the result.
}

emuptr ClipboardGetItem(const ClipboardFormatType format, UInt16* length) {
    // Prepare the stack.
    CALLER_SETUP("emuptr", "const ClipboardFormatType format, UInt16* length");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(ClipboardFormatType, format);
    CALLER_PUT_PARAM_REF(UInt16, length, Marshal::kInOut);

    // Call the function.
    sub.Call(sysTrapClipboardGetItem);

    // Write back any "by ref" parameters.
    CALLER_GET_PARAM_REF(length);

    // Return the result.
    RETURN_RESULT_VAL(emuptr);
}

emuptr DmGetResource(DmResType type, DmResID id) {
    // Prepare the stack.
    CALLER_SETUP("emuptr", "DmResType type, DmResID id");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(DmResType, type);
    CALLER_PUT_PARAM_VAL(DmResID, id);

    // Call the function.
    sub.Call(sysTrapDmGetResource);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(emuptr);
}

emuptr PrefOpenPreferenceDBV10(void) {
    // Prepare the stack.
    CALLER_SETUP("emuptr", "void");

    // Set the parameters.

    // Call the function.
    sub.Call(sysTrapPrefOpenPreferenceDBV10);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(emuptr);
}

emuptr PrefOpenPreferenceDB(Boolean saved) {
    // Prepare the stack.
    CALLER_SETUP("emuptr", "Boolean saved");

    // Set the parameters.
    CALLER_PUT_PARAM_VAL(Boolean, saved);

    // Call the function.
    sub.Call(sysTrapPrefOpenPreferenceDB);

    // Write back any "by ref" parameters.

    // Return the result.
    RETURN_RESULT_VAL(emuptr);
}

Err SysUIAppSwitch(UInt16 cardNo, LocalID dbID, UInt16 cmd, emuptr cmdPBP) {
    CALLER_SETUP("Err", "UInt16 cardNo, LocalID dbID, UInt16 cmd, emuptr cmdPBP");

    CALLER_PUT_PARAM_VAL(UInt16, cardNo);
    CALLER_PUT_PARAM_VAL(LocalID, dbID);
    CALLER_PUT_PARAM_VAL(UInt16, cmd);
    CALLER_PUT_PARAM_VAL(emuptr, cmdPBP);

    sub.Call(sysTrapSysUIAppSwitch);

    RETURN_RESULT_VAL(Err);
}

UInt16 DmNumResources(emuptr dbP) {
    CALLER_SETUP("UInt16", "DmOpenRef dbP");

    CALLER_PUT_PARAM_VAL(emuptr, dbP);

    sub.Call(sysTrapDmNumResources);

    RETURN_RESULT_VAL(UInt16);
}

UInt16 DmNumRecords(emuptr dbP) {
    CALLER_SETUP("UInt16", "DmOpenRef dbP");

    CALLER_PUT_PARAM_VAL(emuptr, dbP);

    sub.Call(sysTrapDmNumRecords);

    RETURN_RESULT_VAL(UInt16);
}

emuptr MemLocalIDToLockedPtr(LocalID local, UInt16 cardNo) {
    CALLER_SETUP("emuptr", "LocalID local, UInt16 cardNo");

    CALLER_PUT_PARAM_VAL(LocalID, local);
    CALLER_PUT_PARAM_VAL(UInt16, cardNo);

    sub.Call(sysTrapMemLocalIDToLockedPtr);

    RETURN_RESULT_VAL(emuptr);
}

Err MemPtrUnlock(emuptr p) {
    CALLER_SETUP("Err", "MemPtr p");

    CALLER_PUT_PARAM_VAL(emuptr, p);

    sub.Call(sysTrapMemPtrUnlock);

    RETURN_RESULT_VAL(Err);
}

Err DmResourceInfo(emuptr dbP, UInt16 index, DmResType* resTypeP, DmResID* resIDP,
                   LocalID* chunkLocalIDP) {
    CALLER_SETUP(
        "Err",
        "emuptr dbP, UInt16 index, DmResType* resTypeP, DmResID* resIDP, LocalID* chunkLocalIDP");

    CALLER_PUT_PARAM_VAL(emuptr, dbP);
    CALLER_PUT_PARAM_VAL(UInt16, index);
    CALLER_PUT_PARAM_REF(DmResType, resTypeP, Marshal::kOutput);
    CALLER_PUT_PARAM_REF(DmResID, resIDP, Marshal::kOutput);
    CALLER_PUT_PARAM_REF(LocalID, chunkLocalIDP, Marshal::kOutput);

    sub.Call(sysTrapDmResourceInfo);

    CALLER_GET_PARAM_REF(resTypeP);
    CALLER_GET_PARAM_REF(resIDP);
    CALLER_GET_PARAM_REF(chunkLocalIDP);

    RETURN_RESULT_VAL(Err);
}

Err DmRecordInfo(emuptr dbP, UInt16 index, UInt16* attrP, UInt32* uniqueIDP, LocalID* chunkIDP) {
    CALLER_SETUP(
        "Err", "DmOpenRef dbP, UInt16 index, UInt16 *attrP, UInt32 *uniqueIDP, LocalID *chunkIDP");

    CALLER_PUT_PARAM_VAL(emuptr, dbP);
    CALLER_PUT_PARAM_VAL(UInt16, index);
    CALLER_PUT_PARAM_REF(UInt16, attrP, Marshal::kOutput);
    CALLER_PUT_PARAM_REF(UInt32, uniqueIDP, Marshal::kOutput);
    CALLER_PUT_PARAM_REF(LocalID, chunkIDP, Marshal::kOutput);

    sub.Call(sysTrapDmRecordInfo);

    CALLER_GET_PARAM_REF(attrP);
    CALLER_GET_PARAM_REF(uniqueIDP);
    CALLER_GET_PARAM_REF(chunkIDP);

    RETURN_RESULT_VAL(Err);
}
