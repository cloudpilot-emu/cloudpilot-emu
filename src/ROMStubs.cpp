/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "ROMStubs.h"

#include "EmBankMapped.h"  // EmBankMapped::GetEmulatedAddress
#include "EmCommon.h"
#include "EmSubroutine.h"  // EmSubroutine
#include "Marshal.h"       // CALLER_PUT_PARAM_VAL

// ---------------------------------------------------------------------------
//		� Feature Manager functions
// ---------------------------------------------------------------------------

// --------------------
// Called:
//
//	*	by Gremlin code (IntlMgrExists) to see if the Intenational Manager
//		exists before trying to call it.
//
//	*	by Gremlin code (Gremlins::GetFakeEvent) to see what language system
//		we're using (so we know whether to use the English or Japanese quotes).
//
//	*	during bootup (patch on FtrInit) to get the ROM version.
// --------------------

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
