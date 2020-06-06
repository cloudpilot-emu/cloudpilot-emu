/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
        Copyright (c) 2001 PocketPyro, Inc.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

\* ===================================================================== */

#include "EmPatchModule.h"

#include "EmPalmFunction.h"  // SysTrapIndex, IsLibraryTrap

EmPatchModule::~EmPatchModule() {}

void EmPatchModule::LoadProtoPatchTable(ProtoPatchTableEntry *protoPatchTable) {
    // Create a fast dispatch table for the managed module.  A "fast
    // dispatch table" is a table with a headpatch and tailpatch entry
    // for each possible function in the module.  If the function is
    // not head or tailpatched, the corresponding entry is NULL.  When
    // a patch function is needed, the trap dispatch number is used as
    // an index into the table in order to get the right patch function.
    //
    // For simplicity, "fast patch tables" are created from "proto patch
    // tables".  A proto patch table is a compact table containing the
    // information needed to create a fast patch table.  Each entry in
    // the proto patch table is a trap-number/headpatch/tailpatch tupple.
    // Each tuple is examined in turn.	If there is a head or tail patch
    // function for the indicated module function, that patch function
    // is entered in the fast dispatch table, using the trap number as
    // the index.

    for (long ii = 0; protoPatchTable[ii].fTrapWord; ++ii) {
        // If there is a headpatch function...

        if (protoPatchTable[ii].fHeadpatch) {
            // Get the trap number.

            uint16 index = ::SysTrapIndex(protoPatchTable[ii].fTrapWord);

            // If the trap number is 0xA800-based, make it zero based.

            if (::IsLibraryTrap(index)) index -= SysTrapIndex(sysLibTrapBase);

            // Resize the fast patch table, if necessary.

            if (index >= fHeadpatches.size()) {
                fHeadpatches.resize(index + 1);
            }

            // Add the headpatch function.

            fHeadpatches[index] = protoPatchTable[ii].fHeadpatch;
        }

        // If there is a tailpatch function...

        if (protoPatchTable[ii].fTailpatch) {
            // Get the trap number.

            uint16 index = SysTrapIndex(protoPatchTable[ii].fTrapWord);

            // If the trap number is 0xA800-based, make it zero based.

            if (IsLibraryTrap(index)) index -= SysTrapIndex(sysLibTrapBase);

            // Resize the fast patch table, if necessary.

            if (index >= fTailpatches.size()) {
                fTailpatches.resize(index + 1);
            }

            // Add the tailpatch function.

            fTailpatches[index] = protoPatchTable[ii].fTailpatch;
        }
    }
}

Err EmPatchModule::GetHeadpatch(uint16 index, HeadpatchProc &procP) {
    Err err = kPatchErrInvalidIndex;
    procP = NULL;

    if (index < fHeadpatches.size()) {
        procP = fHeadpatches[index];
        err = kPatchErrNone;
    }

    return err;
}

// Return the patch function for the given module function   The given
// module function *must* be given as a zero-based index.  If there is
// no patch function for the modeule function, procP == NULL.
//
Err EmPatchModule::GetTailpatch(uint16 index, TailpatchProc &procP) {
    Err err = kPatchErrInvalidIndex;
    procP = NULL;

    if (index < fTailpatches.size()) {
        procP = fTailpatches[index];
        err = kPatchErrNone;
    }

    return err;
}
