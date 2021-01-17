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

#ifndef EmBankDRAM_h
#define EmBankDRAM_h

#include "EmCommon.h"
class EmBankDRAM {
   public:
    static void Initialize(void);
    static void Reset(Bool hardwareReset);
    static void Dispose(void);

    static void SetBankHandlers(void);

    static uint32 GetLong(emuptr address);
    static uint32 GetWord(emuptr address);
    static uint32 GetByte(emuptr address);
    static void SetLong(emuptr address, uint32 value);
    static void SetWord(emuptr address, uint32 value);
    static void SetByte(emuptr address, uint32 value);
    static int ValidAddress(emuptr address, uint32 size);
    static uint8* GetRealAddress(emuptr address);
    static uint8* GetMetaAddress(emuptr address);
    static void AddOpcodeCycles(void);

   private:
    static void AddressError(emuptr address, long size, Bool forRead);
    static void InvalidAccess(emuptr address, long size, Bool forRead);
};

#endif /* EmBankDRAM_h */
