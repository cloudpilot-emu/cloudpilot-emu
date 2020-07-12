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
