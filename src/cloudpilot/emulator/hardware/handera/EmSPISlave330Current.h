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

#ifndef EmSPISlave330Current_h
#define EmSPISlave330Current_h

#include "EmSPISlave.h"  // EmSPISlave

#define kChannelBits 0x70

/*--------------------------------------------------------------------
 * HandEra 330 has an analog switch which switches between voltage and
 * current sense on channel 2.  Need to cobble together something so
 * it can sense some battery current and not think we are always on
 * external power.
 *-------------------------------------------------------------------*/
class EmSPISlave330Current : public EmSPISlave {
   public:
    EmSPISlave330Current();
    virtual ~EmSPISlave330Current(void);

    virtual void Save(Savestate<ChunkType>&);
    virtual void Save(SavestateProbe<ChunkType>&);
    virtual void Load(SavestateLoader<ChunkType>&);

    virtual uint16 DoExchange(uint16 control, uint16 data);
    void SetMode(Bool powerConnected) { fPowerConnected = powerConnected; }

   protected:
    void ProcessCommand(uint8);
    void LoadPendingConversion(void);

   private:
    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper);

   private:
    uint32 fBitBufferIn;
    uint16 fBitBufferOut;
    int fNumBitsIn;
    uint16 fPendingResult;
    Bool fHavePending;
    int fCommandBitsSeen;

    Bool fPowerConnected;
};

#endif
