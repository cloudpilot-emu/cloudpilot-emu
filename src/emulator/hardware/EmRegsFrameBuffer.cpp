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

namespace {
    constexpr int SAVESTATE_VERSION = 1;

    inline void markDirty(emuptr offset) {
        gFramebufferDirtyPages[offset >> 13] |= (1 << ((offset >> 10) & 0x07));
    }
}  // namespace

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::EmRegsFrameBuffer
// ---------------------------------------------------------------------------

EmRegsFrameBuffer::EmRegsFrameBuffer(emuptr baseAddr) : fBaseAddr(baseAddr) {}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::~EmRegsFrameBuffer
// ---------------------------------------------------------------------------

EmRegsFrameBuffer::~EmRegsFrameBuffer(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::Initialize
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::Initialize(void) {
    EmRegs::Initialize();
    memset(gFramebufferMemory, 0, gFramebufferMemorySize);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::Reset
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::Reset(Bool hardwareReset) { EmRegs::Reset(hardwareReset); }

void EmRegsFrameBuffer::Load(SavestateLoader& loader) {
    if (!loader.HasChunk(ChunkType::regsFrameBuffer)) return;

    Chunk* chunk = loader.GetChunk(ChunkType::regsFrameBuffer);
    if (!chunk) return;

    if (chunk->Get32() > SAVESTATE_VERSION) {
        logging::printf("unable to restore regsFrameBuffer: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    // We used to save the framebuffer in the savestate, but we have since moved
    // to appending it to the memory and saving it page by page. Savestate contains
    // framebuffer? -> load it and make sure the corrersponding pages will all be
    // saved.
    //
    // NOT ENDIANESS SAFE
    chunk->GetBuffer(gFramebufferMemory, gFramebufferMemorySize);
    memset(gFramebufferDirtyPages, 0xff, gFramebufferMemorySize / 1024 / 8);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetLong
// ---------------------------------------------------------------------------

uint32 EmRegsFrameBuffer::GetLong(emuptr address) {
    uint32 offset = address - fBaseAddr;
    return EmMemDoGet32(gFramebufferMemory + offset);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetWord
// ---------------------------------------------------------------------------

uint32 EmRegsFrameBuffer::GetWord(emuptr address) {
    uint32 offset = address - fBaseAddr;
    return EmMemDoGet16((gFramebufferMemory) + offset);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetByte
// ---------------------------------------------------------------------------

uint32 EmRegsFrameBuffer::GetByte(emuptr address) {
    uint32 offset = address - fBaseAddr;
    return EmMemDoGet8((gFramebufferMemory) + offset);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::SetLong
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::SetLong(emuptr address, uint32 value) {
    uint32 offset = address - fBaseAddr;
    EmMemDoPut32((gFramebufferMemory) + offset, value);

    gSystemState.MarkScreenDirty(address, address + 4);

    markDirty(offset);
    markDirty(offset + 2);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::SetWord
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::SetWord(emuptr address, uint32 value) {
    uint32 offset = address - fBaseAddr;
    EmMemDoPut16((gFramebufferMemory) + offset, value);

    gSystemState.MarkScreenDirty(address, address + 2);

    markDirty(offset);
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::SetByte
// ---------------------------------------------------------------------------

void EmRegsFrameBuffer::SetByte(emuptr address, uint32 value) {
    uint32 offset = address - fBaseAddr;
    EmMemDoPut8((gFramebufferMemory) + offset, value);

    gSystemState.MarkScreenDirty(address, address);

    markDirty(offset);
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
    return gFramebufferMemory + offset;
}

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsFrameBuffer::GetAddressStart(void) { return fBaseAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsFrameBuffer::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsFrameBuffer::GetAddressRange(void) { return gFramebufferMemorySize; }
