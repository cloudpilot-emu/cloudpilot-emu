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

#include "EmSPISlave330Current.h"

#include "EmCommon.h"
#include "Logging.h"
#include "savestate/ChunkHelper.h"
#include "savestate/Savestate.h"
#include "savestate/SavestateLoader.h"
#include "savestate/SavestateProbe.h"

// #define LOGGING 0
#ifdef LOGGING
    #define PRINTF logPrintf
#else
    #define PRINTF(...) ;
#endif

namespace {
    constexpr uint32 SAVESTATE_VERSION = 1;
}

/*****************************************************************************
 * The HandEra 330 includes a battery current sense.  It is switched in and
 * out of channel 2 of the ADC.
 ****************************************************************************/

// ---------------------------------------------------------------------------
//		� EmSPISlave330Current::EmSPISlave330Current
// ---------------------------------------------------------------------------

EmSPISlave330Current::EmSPISlave330Current()
    : EmSPISlave(),
      fBitBufferIn(0),
      fBitBufferOut(0),
      fNumBitsIn(0),
      fPendingResult(0),
      fHavePending(false),
      fCommandBitsSeen(0),
      fPowerConnected(false) {}

// ---------------------------------------------------------------------------
//		� EmSPISlave330Current::~EmSPISlave330Current
// ---------------------------------------------------------------------------

EmSPISlave330Current::~EmSPISlave330Current(void) {}

void EmSPISlave330Current::Load(SavestateLoader<ChunkType>& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::spiSlave330Current);
    if (!chunk) return;

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logPrintf("unable to restore SPISlave330Current: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

void EmSPISlave330Current::Save(Savestate<ChunkType>& savestate) { DoSave(savestate); }

void EmSPISlave330Current::Save(SavestateProbe<ChunkType>& savestateProbe) {
    DoSave(savestateProbe);
}

template <typename T>
void EmSPISlave330Current::DoSave(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::spiSlave330Current);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

template <typename T>
void EmSPISlave330Current::DoSaveLoad(T& helper) {
    helper.Do32(fBitBufferIn)
        .Do32(fNumBitsIn)
        .Do32(fCommandBitsSeen)
        .Do(typename T::Pack16() << fBitBufferOut << fPendingResult)
        .Do(typename T::BoolPack() << fHavePending << fPowerConnected);
}

// ---------------------------------------------------------------------------
//		� EmSPISlave330Current::DoExchange
// ---------------------------------------------------------------------------

uint16 EmSPISlave330Current::DoExchange(uint16 control, uint16 data) {
    PRINTF("");
    PRINTF("EmSPISlave330Current::DoExchange");
    PRINTF("control = 0x%04X, data = 0x%04X", control, data);

    // -----------------------------------------------------------------------
    // Merge the incoming bits with our current buffer.
    // -----------------------------------------------------------------------

    uint16 numBits = (control & hwrVZ328SPIMControlBitsMask) + 1;
    uint32 oldBitsMask = ~0u << numBits;
    uint32 newBitsMask = ~oldBitsMask;

    PRINTF("Before merging input:   fBitBufferIn = 0x%04X, fNumBitsIn = 0x%04X", fBitBufferIn,
           fNumBitsIn);

    fBitBufferIn = ((fBitBufferIn << numBits) & oldBitsMask) | (data & newBitsMask);
    fNumBitsIn += numBits;

    PRINTF("After merging input:    fBitBufferIn = 0x%04X, fNumBitsIn = 0x%04X", fBitBufferIn,
           fNumBitsIn);

    // -----------------------------------------------------------------------
    // Start processing the command bits.
    // -----------------------------------------------------------------------

    EmAssert(fNumBitsIn - fCommandBitsSeen - 1 >= 0);

    uint16 result = 0;
    uint32 mask = 1 << (fNumBitsIn - fCommandBitsSeen - 1);

    while (mask) {
        // Shift out a bit.

        {
            result = (result << 1) | (fBitBufferOut >> 15);
            fBitBufferOut <<= 1;
        }

        // If we haven't seen the Start bit yet, look for it.

        if (fCommandBitsSeen == 0) {
            // If we found the Start bit, start counting the
            // number of command bits as we stream through them.

            if ((mask & fBitBufferIn) != 0) {
                fCommandBitsSeen++;
            }

            // Otherwise, adjust fNumBitsIn so that when we *do*
            // find the Start bit, we know where it is.

            else {
                fNumBitsIn--;
            }

            // If there's a pending conversion, load it into the
            // output shift register after receiving the first
            // bit after the last bit of the previous command.

            this->LoadPendingConversion();
        } else {
            fCommandBitsSeen++;

            // If we've seen 8 bits, process the command, and then
            // prepare for the next one.

            if (fCommandBitsSeen == 8) {
                fNumBitsIn -= 8;
                fCommandBitsSeen = 0;

                uint8 command = fBitBufferIn >> fNumBitsIn;
                this->ProcessCommand(command);

                PRINTF("After ProcessCommand:   fPendingResult = 0x%04X", fPendingResult);
            }
        }

        mask >>= 1;
    }

    // ----------------------------------------------------------------------
    // Return the result.
    // ----------------------------------------------------------------------

    PRINTF("result = 0x%04X", result);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmSPISlave330Current::ProcessCommand
// ---------------------------------------------------------------------------

void EmSPISlave330Current::ProcessCommand(uint8 command) {
    fPendingResult = 0xffff;
    fHavePending = true;
}

// ---------------------------------------------------------------------------
//		� EmSPISlave330Current::LoadPendingConversion
// ---------------------------------------------------------------------------

void EmSPISlave330Current::LoadPendingConversion(void) {
    // -----------------------------------------------------------------------
    // If there's a pending conversion, move it into the output shift register.
    // -----------------------------------------------------------------------
    if (fHavePending) {
        fHavePending = false;
        fBitBufferOut = fPendingResult;
    }
}
