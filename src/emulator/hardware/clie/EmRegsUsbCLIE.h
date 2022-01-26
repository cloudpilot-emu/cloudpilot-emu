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

#ifndef _BANK_USBSONY_H_
#define _BANK_USBSONY_H_

#include "EmPalmStructs.h"
#include "EmRegs.h"
#include "PalmPack.h"

struct HwrUsbCLIEType {
    UInt8 dataWrite;  // 0x0000
    UInt8 dataRead;   // 0x0001
    UInt8 cmdWrite;   // 0x0002
    UInt8 cmdRead;    // 0x0003
};
#include "PalmPackPop.h"

class EmRegsUsbCLIE : public EmRegs {
   public:
    EmRegsUsbCLIE();
    EmRegsUsbCLIE(emuptr baseAddr, uint32 offset);
    EmRegsUsbCLIE(uint32 offset);
    virtual ~EmRegsUsbCLIE(void);

    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Dispose(void);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

   private:
    uint32 Read(emuptr address, int size);
    void Write(emuptr address, int size, uint32 value);

    HwrUsbCLIEType fRegs;
    uint32 fOffsetAddr;
    emuptr fBaseAddr;
};

#endif /* _BANK_USBSONY_H_ */
