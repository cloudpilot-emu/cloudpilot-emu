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

#ifndef EmRegsFrameBuffer_h
#define EmRegsFrameBuffer_h

#include "EmRegs.h"

class SessionFile;

class EmRegsFrameBuffer : public EmRegs {
   public:
    EmRegsFrameBuffer(emuptr baseAddr);
    virtual ~EmRegsFrameBuffer(void);

    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);

    virtual uint32 GetLong(emuptr address);
    virtual uint32 GetWord(emuptr address);
    virtual uint32 GetByte(emuptr address);
    virtual void SetLong(emuptr address, uint32 value);
    virtual void SetWord(emuptr address, uint32 value);
    virtual void SetByte(emuptr address, uint32 value);
    virtual int ValidAddress(emuptr address, uint32 size);
    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

   private:
    template <typename T>
    void DoSave(T& savestate);

   private:
    emuptr fBaseAddr;
};

#endif /* EmRegsFrameBuffer_h */
