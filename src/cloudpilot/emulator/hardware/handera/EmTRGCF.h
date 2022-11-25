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

#ifndef EmTRGCF_h
#define EmTRGCF_h

#include "EmRegs.h"
#include "EmTRGATA.h"
#include "EmTRGCFDefs.h"
#include "EmTRGCFMem.h"

class EmRegsCFMemCard : public EmRegs {
   public:
    EmRegsCFMemCard(CFBusManager* fCFBusManager);
    virtual ~EmRegsCFMemCard(void);

    // EmRegs overrides
    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Dispose(void);

    uint8* GetRealAddress(emuptr address);
    void SetSubBankHandlers(void);
    emuptr GetAddressStart(void);
    uint32 GetAddressRange(void);

    uint32 GetLong(emuptr iAddress);
    uint32 GetWord(emuptr iAddress);
    uint32 GetByte(emuptr iAddress);
    void SetLong(emuptr iAddress, uint32 iLongValue);
    void SetWord(emuptr iAddress, uint32 iWordValue);
    void SetByte(emuptr iAddress, uint32 iByteValue);

   private:
    uint8 Buffer[kMemorySizeCF];
    uint32 Read(emuptr address, int size);
    void Write(emuptr address, int size, uint32 value);
    EmRegsCFAta Ata;
    EmRegsCFTuple Tuple;
    EmRegsCFConfig Config;
    EmRegsCFReset CFReset;
    CFBusManager* fBusMgr;
};

#endif /* EmTRGCF_h */
