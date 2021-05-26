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

#ifndef EmRegsPLDPalmVIIEZ_h
#define EmRegsPLDPalmVIIEZ_h

#include "EmHAL.h"          // EmHALHandler
#include "EmPalmStructs.h"  // EmProxyHwrJerryPLDType
#include "EmRegs.h"         // EmRegs

class SessionFile;

class EmRegsPLDPalmVIIEZ : public EmRegs, public EmHALHandler {
   public:
    EmRegsPLDPalmVIIEZ(void);
    virtual ~EmRegsPLDPalmVIIEZ(void);

    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Dispose(void);

    virtual Bool GetSerialPortOn(int uartNum);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

   protected:
    void rs232ShdnWrite(emuptr address, int size, uint32 value);
    uint32 iXtrnl2Read(emuptr address, int size);

   private:
    EmProxyHwrJerryPLDType fRegs;
};

#endif /* EmRegsPLDPalmVIIEZ_h */
