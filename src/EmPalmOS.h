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

#ifndef EmPalmOS_h
#define EmPalmOS_h

#include "EmCPU68K.h"   // ExceptionNumber
#include "EmStructs.h"  // EmStackFrameList

class SessionFile;

class EmPalmOS {
   public:
    static void Initialize(void);
    static void Reset(void);
    static void Save(SessionFile&);
    static void Load(SessionFile&);
    static void Dispose(void);

   protected:
    static Bool HandleTrap15(ExceptionNumber);
    static Bool HandleJSR_Ind(emuptr oldPC, emuptr dest);

   private:
    static Bool HandleSystemCall(Bool fromTrap);
};

#endif /* EmPalmOS_h */
