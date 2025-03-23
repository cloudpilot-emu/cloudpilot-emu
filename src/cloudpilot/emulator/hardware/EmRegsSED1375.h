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

#ifndef EmRegsSED1375_h
#define EmRegsSED1375_h

#include "EmHAL.h"  // EmHALHandler
#include "EmPalmStructs.h"
#include "EmRegs.h"
#include "EmStructs.h"  // RGBList

class EmRegsFrameBuffer;

class EmRegsSED1375 : public EmRegs, public EmHALHandler {
   public:
    static constexpr uint32 FRAMEBUFFER_SIZE = 80 * 1024;

   public:
    EmRegsSED1375(emuptr baseRegsAddr, emuptr baseVideoAddr, EmRegsFrameBuffer& framebuffer);
    virtual ~EmRegsSED1375(void);

    // EmRegs overrides
    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Save(Savestate<ChunkType>&);
    virtual void Save(SavestateProbe<ChunkType>&);
    virtual void Load(SavestateLoader<ChunkType>&);
    virtual void Dispose(void);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

    // EmHAL overrides
    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLCDHasFrame(void);
    virtual bool CopyLCDFrame(Frame& frame, bool fullRefresh);
    virtual uint16 GetLCD2bitMapping();

   private:
    enum class LutColorIndex : uint8 { red = 0, green = 1, blue = 2 };

   private:
    void ClearLut();

    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper, uint32 version);

   private:
    uint32 vertNonDisplayRead(emuptr address, int size);
    uint32 lookUpTableDataRead(emuptr address, int size);

    void invalidateWrite(emuptr address, int size, uint32 value);
    void lookUpTableAddressWrite(emuptr address, int size, uint32 value);
    void lookUpTableDataWrite(emuptr address, int size, uint32 value);

   private:
    emuptr fBaseRegsAddr;
    emuptr fBaseVideoAddr;
    EmProxySED1375RegsType fRegs;
    EmRegsFrameBuffer& framebuffer;

    uint32 fClutData[256];
    uint8 lutEntry{0};
    LutColorIndex lutColorIndex{LutColorIndex::red};
};

#endif /* EmRegsSED1375_h */
