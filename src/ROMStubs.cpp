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
