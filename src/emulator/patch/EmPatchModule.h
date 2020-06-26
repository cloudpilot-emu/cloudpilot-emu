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

#ifndef EmPatchModule_h
#define EmPatchModule_h

#include <vector>

#include "EmPatchModuleTypes.h"

// ===========================================================================
//		EmPatchModule
// ===========================================================================

const uint16 kMaxProtoTables = 5;

class EmPatchModule {
   public:
    virtual ~EmPatchModule();

    virtual Err GetHeadpatch(uint16 index, HeadpatchProc &procP);
    virtual Err GetTailpatch(uint16 index, TailpatchProc &procP);

   protected:
    void LoadProtoPatchTable(ProtoPatchTableEntry *protoPatchTable);

   private:
    vector<HeadpatchProc> fHeadpatches;
    vector<TailpatchProc> fTailpatches;
};

#endif  // EmPatchModule_h
