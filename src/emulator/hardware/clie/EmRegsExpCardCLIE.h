/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2000 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmRegsExpCardCLIE_h
#define EmRegsExpCardCLIE_h

#include "EmPalmStructs.h"
#include "EmRegs.h"

class SessionFile;

#define ExpCard_BaseAddress 0x10800000
#define ExpCard_Range 0x000FFFFF

struct HwrExpCardCLIEType {
    UInt8 dataWrite;  // 0x0000
    UInt8 dataRead;   // 0x0001
    UInt8 cmdWrite;   // 0x0002
    UInt8 cmdRead;    // 0x0003

    UInt8 _filler01[4];  // 0x0004-0007

    UInt8 ExpCard0008;  // 0x0008
    UInt8 ExpCard0009;  // 0x0009

    UInt8 _filler02[6];  // 0x000A-000F
};

class EmRegsExpCardCLIE : public EmRegs {
   public:
    EmRegsExpCardCLIE(void);
    EmRegsExpCardCLIE(uint32 offset);
    virtual ~EmRegsExpCardCLIE(void);

    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Dispose(void);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

    void UnsupportedWrite(emuptr address, int size, uint32 value);

   private:
    uint32 Read(emuptr address, int size);
    void Write(emuptr address, int size, uint32 value);

    uint32 ReadFromDummy(emuptr address, int size);
    void WrireToDummy(emuptr address, int size, uint32 value);

    HwrExpCardCLIEType fRegs;
    uint32 fAddressOffset;
    emuptr fBaseAddr;
};

#endif /* EmRegsExpCardCLIE_h */
