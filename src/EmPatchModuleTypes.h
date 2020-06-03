/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2001 PocketPyro, Inc.
        Portions Copyright (c) Palm, Inc.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmPatchModuleTypes_h
#define EmPatchModuleTypes_h

#include "EmStructs.h"  // SystemCallContext

enum { kPatchErrNone, kPatchErrNotImplemented, kPatchErrInvalidIndex };

enum CallROMType { kExecuteROM, kSkipROM };

// Function types for head- and Tailpatch functions.

typedef CallROMType (*HeadpatchProc)(void);
typedef void (*TailpatchProc)(void);

// ======================================================================
// At compile time, the list of functions we want to head- and tailpatch
// are stored in an array of ProtoPatchTableEntries.  At runtime, these
// compact arrays are expanded by PatchModule into a sparse array
// so that the function dispatch number can be used to look up the
// patch function.
// ======================================================================

struct ProtoPatchTableEntry {
    uint16 fTrapWord;
    HeadpatchProc fHeadpatch;
    TailpatchProc fTailpatch;
};

// ===========================================================================
//		� TailpatchType
// ===========================================================================
// Structure used to hold tail-patch information.

struct TailpatchType {
    SystemCallContext fContext;
    int32 fCount;
    TailpatchProc fTailpatch;
};

typedef vector<TailpatchType> TailPatchIndex;

#endif  // EmPatchModuleTypes_h
