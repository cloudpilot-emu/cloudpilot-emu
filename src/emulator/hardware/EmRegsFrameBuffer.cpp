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

#include "EmRegsFrameBuffer.h"

#include "Byteswapping.h"  // ByteswapWords
#include "EmCommon.h"
#include "EmMemory.h"  // EmMemDoGet32
#include "EmSystemState.h"
#include "Miscellaneous.h"  // StWordSwapper
#include "Platform.h"       // Platform::AllocateMemoryClear
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::EmRegsFrameBuffer
// ---------------------------------------------------------------------------

EmRegsFrameBuffer::EmRegsFrameBuffer(emuptr baseAddr, int32 size)
    : fBaseAddr(baseAddr), fSize(size), fVideoMem(NULL) {}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::~EmRegsFrameBuffer
// ---------------------------------------------------------------------------

EmRegsFrameBuffer::~EmRegsFrameBuffer(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::Initialize
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::Initialize(void) {
    EmRegs::Initialize();

    // Allocate a chunk of memory for the VRAM space.

    if (!fVideoMem) {
        fVideoMem = Platform::AllocateMemoryClear(fSize);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::Reset
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::Reset(Bool hardwareReset) { EmRegs::Reset(hardwareReset); }

#if 0

void c::Save(SessionFile& f) {
    EmRegs::Save(f);

    StWordSwapper swapper(fVideoMem, fSize);
    f.WriteSED1375Image(fVideoMem, fSize);
}

void EmRegsFrameBuffer::Load(SessionFile& f) {
    EmRegs::Load(f);

    // Read in the LCD image, and then byteswap it.

    if (f.ReadSED1375Image(fVideoMem)) {
        ByteswapWords(fVideoMem, fSize);
    } else {
        f.SetCanReload(false);
    }
}

#endif

void EmRegsFrameBuffer::Save(Savestate& savestate) { savestate.NotifyError(); }

void EmRegsFrameBuffer::Save(SavestateProbe& savestate) {}

void EmRegsFrameBuffer::Load(SavestateLoader& loader) {}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::Dispose
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::Dispose(void) {
    EmRegs::Dispose();

    Platform::DisposeMemory(fVideoMem);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetLong
// ---------------------------------------------------------------------------

uint32 EmRegsFrameBuffer::GetLong(emuptr address) {
    uint32 offset = address - fBaseAddr;
    return EmMemDoGet32(((uint8*)fVideoMem) + offset);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetWord
// ---------------------------------------------------------------------------

uint32 EmRegsFrameBuffer::GetWord(emuptr address) {
    uint32 offset = address - fBaseAddr;
    return EmMemDoGet16(((uint8*)fVideoMem) + offset);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetByte
// ---------------------------------------------------------------------------

uint32 EmRegsFrameBuffer::GetByte(emuptr address) {
    uint32 offset = address - fBaseAddr;
    return EmMemDoGet8(((uint8*)fVideoMem) + offset);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::SetLong
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::SetLong(emuptr address, uint32 value) {
    uint32 offset = address - fBaseAddr;
    EmMemDoPut32(((uint8*)fVideoMem) + offset, value);

    gSystemState.MarkScreenDirty();
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::SetWord
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::SetWord(emuptr address, uint32 value) {
    uint32 offset = address - fBaseAddr;
    EmMemDoPut16(((uint8*)fVideoMem) + offset, value);

    gSystemState.MarkScreenDirty();
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::SetByte
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::SetByte(emuptr address, uint32 value) {
    uint32 offset = address - fBaseAddr;
    EmMemDoPut8(((uint8*)fVideoMem) + offset, value);

    gSystemState.MarkScreenDirty();
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::ValidAddress
// ---------------------------------------------------------------------------

int EmRegsFrameBuffer::ValidAddress(emuptr address, uint32 size) {
    UNUSED_PARAM(address);
    UNUSED_PARAM(size);

    return true;
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::SetSubBankHandlers(void) {
    // We don't have handlers for each byte of memory, so there's nothing
    // to install here.
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsFrameBuffer::GetRealAddress(emuptr address) {
    uint32 offset = address - fBaseAddr;
    return (uint8*)fVideoMem + offset;
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsFrameBuffer::GetAddressStart(void) { return fBaseAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsFrameBuffer::GetAddressRange(void) { return fSize; }
