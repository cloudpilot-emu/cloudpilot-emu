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

#ifndef EmRegs_h
#define EmRegs_h

#include <vector>

#include "EmCommon.h"

class SessionFile;
struct EmAddressBank;

class EmRegs {
   public:
    EmRegs(void);
    virtual ~EmRegs(void);

    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Save(SessionFile&);
    virtual void Load(SessionFile&);
    virtual void Dispose(void);

    void SetBankHandlers(EmAddressBank&);
    virtual void SetSubBankHandlers(void) = 0;

    virtual uint32 GetLong(emuptr address);
    virtual uint32 GetWord(emuptr address);
    virtual uint32 GetByte(emuptr address);
    virtual void SetLong(emuptr address, uint32 value);
    virtual void SetWord(emuptr address, uint32 value);
    virtual void SetByte(emuptr address, uint32 value);
    virtual int ValidAddress(emuptr address, uint32 size);
    virtual uint8* GetRealAddress(emuptr address) = 0;
    virtual emuptr GetAddressStart(void) = 0;
    virtual uint32 GetAddressRange(void) = 0;

   protected:
    typedef uint32 (EmRegs::*ReadFunction)(emuptr address, int size);
    typedef void (EmRegs::*WriteFunction)(emuptr address, int size, uint32 value);

    void SetHandler(ReadFunction read, WriteFunction write, uint32 start, int count);

    uint32 UnsupportedRead(emuptr address, int size);
    uint32 StdRead(emuptr address, int size);
    uint32 StdReadBE(emuptr address, int size);
    uint32 ZeroRead(emuptr address, int size);

    void UnsupportedWrite(emuptr address, int size, uint32 value);
    void StdWrite(emuptr address, int size, uint32 value);
    void StdWriteBE(emuptr address, int size, uint32 value);
    void NullWrite(emuptr address, int size, uint32 value);

   private:
    typedef vector<ReadFunction> ReadFunctionList;
    typedef vector<WriteFunction> WriteFunctionList;

    ReadFunctionList fReadFunctions;
    WriteFunctionList fWriteFunctions;
};

using EmRegsList = vector<EmRegs*>;

#endif /* EmRegs_h */
