/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmLowMem_h
#define EmLowMem_h

#include "EmPalmStructs.h"

class EmLowMem {
   public:
    static uint8 GetEvtMgrIdle(void);
    static Bool TrapExists(uint16 iATrap);
    static emuptr GetTrapAddress(uint16 iATrap);

#define EmLowMem_GetGlobal(x) EmLowMem::fgLowMem.globals.x
#define EmLowMem_SetGlobal(x, v) EmLowMem::fgLowMem.globals.x = v
#define EmLowMem_AddressOf(x) EmLowMem::fgLowMem.globals.x.GetPtr()

    static EmAliasLowMemHdrType<PAS> fgLowMem;
};

#endif  // EmLowMem_h
