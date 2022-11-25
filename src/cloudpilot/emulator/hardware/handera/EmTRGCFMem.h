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

/****************************************************************************************
**	File:		Bank_CF_Config.h
**	Date:		6/14/00
**	Project:	POSE
**	Author:		Dan Kolz
**	Notes:		This class is an emulation of the config area on a CF card.
**				It takes its information from a hardcoded array in the source code.
****************************************************************************************/

#ifndef EmTRGCFMem_h
#define EmTRGCFMem_h

#include "EmRegs.h"
#include "EmTRGCFDefs.h"

class EmRegsCFConfig {
   public:
    void Initialize();
    void ReadByte(uint32 offset, uint8* val);
    void WriteByte(uint32 offset, uint8 val);
    void ReadWord(uint32 offset, _Word* val);
    void WriteWord(uint32 offset, _Word val);

   private:
    Boolean InRange(uint32 offset);
};

class EmRegsCFReset {
   public:
    void Initialize();
    void ReadByte(uint32 offset, uint8* val);
    void WriteByte(uint32 offset, uint8 val);
    void ReadWord(uint32 offset, _Word* val);
    void WriteWord(uint32 offset, _Word val);

   private:
};

class EmRegsCFTuple {
   public:
    void Initialize();
    void ReadByte(uint32 offset, uint8* val);
    void WriteByte(uint32 offset, uint8 val);
    void ReadWord(uint32 offset, _Word* val);
    void WriteWord(uint32 offset, _Word val);
};

#endif  // EmTRGCFMem_h
