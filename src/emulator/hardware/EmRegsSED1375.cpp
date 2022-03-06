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

#include "EmRegsSED1375.h"

#include "Byteswapping.h"  // Canonical
#include "ChunkHelper.h"
#include "EmCommon.h"
#include "EmRegsFrameBuffer.h"
#include "EmSystemState.h"
#include "Frame.h"
#include "Miscellaneous.h"  // StWordSwapper
#include "Nibbler.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

// Given a register (specified by its field name), return its address
// in emulated space.

#define addressof(reg) (this->GetAddressStart() + fRegs.offsetof_##reg())

// Macro to help the installation of handlers for a register.

#define INSTALL_HANDLER(read, write, reg)                                                      \
    this->SetHandler((ReadFunction)&EmRegsSED1375::read, (WriteFunction)&EmRegsSED1375::write, \
                     addressof(reg), fRegs.reg.GetSize())

#define kCLUTColorIndexMask 0xf000
#define kCLUTColorsMask 0x0fff

#define kCLUTRedMask 0x0f00
#define kCLUTGreenMask 0x00f0
#define kCLUTBlueMask 0x000f

#define kCLUTIndexRed 0x4000
#define kCLUTIndexGreen 0x2000
#define kCLUTIndexBlue 0x1000

namespace {
    constexpr int SAVESTATE_VERSION = 1;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::EmRegsSED1375
// ---------------------------------------------------------------------------

EmRegsSED1375::EmRegsSED1375(emuptr baseRegsAddr, emuptr baseVideoAddr,
                             EmRegsFrameBuffer& framebuffer)
    : fBaseRegsAddr(baseRegsAddr),
      fBaseVideoAddr(baseVideoAddr),
      fRegs(),
      framebuffer(framebuffer) {
    ClearLut();
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::~EmRegsSED1375
// ---------------------------------------------------------------------------

EmRegsSED1375::~EmRegsSED1375(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::Initialize
// ---------------------------------------------------------------------------

void EmRegsSED1375::Initialize(void) {
    EmRegs::Initialize();

    lutEntry = 0;
    lutColorIndex = LutColorIndex::red;
    ClearLut();
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::Reset
// ---------------------------------------------------------------------------

void EmRegsSED1375::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);

    if (hardwareReset) {
        memset(fRegs.GetPtr(), 0, fRegs.GetSize());

        //		EmAssert ((sed1375ProductCodeExpected | sed1375RevisionCodeExpected) ==
        // 0x24);
        fRegs.productRevisionCode = 0x24;

        lutEntry = 0;
        lutColorIndex = LutColorIndex::red;
        ClearLut();
    }
}

void EmRegsSED1375::Save(Savestate& savestate) { DoSave(savestate); }

void EmRegsSED1375::Save(SavestateProbe& savestate) { DoSave(savestate); }

void EmRegsSED1375::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::regsSED1375);
    if (!chunk) {
        logging::printf("unable to restore RegsSED1375: missing savestate\n");
        loader.NotifyError();

        return;
    }

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore RegsSED1375: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper, version);
}

template <typename T>
void EmRegsSED1375::DoSave(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsSED1375);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper, SAVESTATE_VERSION);
}

template <typename T>
void EmRegsSED1375::DoSaveLoad(T& helper, uint32 version) {
    uint8 colorIndexTmp = static_cast<uint8>(lutColorIndex);

    helper.DoBuffer(fClutData, sizeof(fClutData))
        .DoBuffer(fRegs.GetPtr(), fRegs.GetSize())
        .Do(typename T::Pack8() << lutEntry << colorIndexTmp);

    lutColorIndex = LutColorIndex(colorIndexTmp);
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::Dispose
// ---------------------------------------------------------------------------

void EmRegsSED1375::Dispose(void) { EmRegs::Dispose(); }

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsSED1375::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(StdReadBE, NullWrite, productRevisionCode);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, mode0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, mode1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, mode2);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, horizontalPanelSize);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, verticalPanelSizeLSB);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, verticalPanelSizeMSB);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FPLineStartPosition);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, horizontalNonDisplayPeriod);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FPFRAMEStartPosition);
    INSTALL_HANDLER(vertNonDisplayRead, StdWriteBE, verticalNonDisplayPeriod);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, MODRate);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, screen1StartAddressLSB);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, screen1StartAddressMSB);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, screen2StartAddressLSB);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, screen2StartAddressMSB);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, screen1StartAddressMSBit);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, memoryAddressOffset);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, screen1VerticalSizeLSB);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, screen1VerticalSizeMSB);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, unused1);
    INSTALL_HANDLER(StdReadBE, lookUpTableAddressWrite, lookUpTableAddress);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, unused2);
    INSTALL_HANDLER(lookUpTableDataRead, lookUpTableDataWrite, lookUpTableData);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GPIOConfigurationControl);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GPIOStatusControl);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, scratchPad);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, portraitMode);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, lineByteCountRegister);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, unused3);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, unused4);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, unused5);
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsSED1375::GetRealAddress(emuptr address) {
    return (uint8*)fRegs.GetPtr() + address - this->GetAddressStart();
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsSED1375::GetAddressStart(void) { return fBaseRegsAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsSED1375::GetAddressRange(void) { return fRegs.GetSize(); }

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsSED1375::GetLCDScreenOn(void) { return ((fRegs.mode1) & sed1375DisplayBlank) == 0; }

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsSED1375::GetLCDBacklightOn(void) {
    return true;  // The Backlight is always on for these units.
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmRegsSED1375::GetLCDHasFrame(void) { return true; }

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::GetLCDBeginEnd
// ---------------------------------------------------------------------------

void EmRegsSED1375::GetLCDBeginEnd(emuptr& begin, emuptr& end) {
    // Get the screen metrics.

    int32 bpp = 1 << ((fRegs.mode1 & sed1375BPPMask) >> sed1375BPPShift);
    int32 height = ((fRegs.verticalPanelSizeMSB << 8) | fRegs.verticalPanelSizeLSB) + 1;
    int32 rowBytes = (fRegs.horizontalPanelSize + 1) * bpp;
    uint32 offset = (fRegs.screen1StartAddressMSBit << 17) | (fRegs.screen1StartAddressMSB << 9) |
                    (fRegs.screen1StartAddressLSB << 1);
    emuptr baseAddr = fBaseVideoAddr + offset;

    begin = baseAddr;
    end = baseAddr + rowBytes * height;
}

bool EmRegsSED1375::CopyLCDFrame(Frame& frame, bool fullRefresh) {
    const int32 bpp = 1 << ((fRegs.mode1 & sed1375BPPMask) >> sed1375BPPShift);
    const int32 width = (fRegs.horizontalPanelSize + 1) * 8;
    const int32 height = ((fRegs.verticalPanelSizeMSB << 8) | fRegs.verticalPanelSizeLSB) + 1;
    const uint32 offset = (fRegs.screen1StartAddressMSBit << 17) |
                          (fRegs.screen1StartAddressMSB << 9) | (fRegs.screen1StartAddressLSB << 1);
    const emuptr baseAddr = fBaseVideoAddr + offset;
    const uint32 rowBytes = (bpp * width) / 8;

    if (width != 160 || height != 160) return false;

    frame.bpp = 24;
    frame.lineWidth = width;
    frame.lines = height;
    frame.margin = 0;
    frame.bytesPerLine = width * 4;
    frame.hasChanges = true;

    if (4 * width * height > static_cast<ssize_t>(frame.GetBufferSize())) return false;

    if (!gSystemState.IsScreenDirty() && !fullRefresh) {
        frame.hasChanges = false;
        return true;
    }

    if (gSystemState.ScreenRequiresFullRefresh() || fullRefresh) {
        frame.firstDirtyLine = 0;
        frame.lastDirtyLine = frame.lines - 1;
    } else {
        if (gSystemState.GetScreenHighWatermark() < baseAddr) {
            frame.hasChanges = false;
            return true;
        }

        frame.firstDirtyLine =
            min((max(gSystemState.GetScreenLowWatermark(), baseAddr) - baseAddr) / rowBytes,
                frame.lines - 1);

        frame.lastDirtyLine =
            min((gSystemState.GetScreenHighWatermark() - baseAddr) / rowBytes, frame.lines - 1);
    }

    uint32* buffer =
        reinterpret_cast<uint32*>(frame.GetBuffer() + frame.firstDirtyLine * frame.bytesPerLine);

    switch (bpp) {
        case 1: {
            Nibbler<1, true> nibbler;
            nibbler.reset(framebuffer.GetRealAddress(baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (int32 x = 0; x < width; x++) *(buffer++) = fClutData[nibbler.nibble()];

            return true;
        }

        case 2: {
            Nibbler<2, true> nibbler;
            nibbler.reset(framebuffer.GetRealAddress(baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (int32 x = 0; x < width; x++) *(buffer++) = fClutData[nibbler.nibble()];

            return true;
        }

        case 4: {
            Nibbler<4, true> nibbler;
            nibbler.reset(framebuffer.GetRealAddress(baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (int32 x = 0; x < width; x++) *(buffer++) = fClutData[nibbler.nibble()];

            return true;
        }

        case 8: {
            uint8* fbuf = framebuffer.GetRealAddress(baseAddr + frame.firstDirtyLine * rowBytes);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (int32 x = 0; x < width; x++)
                    *(buffer++) = fClutData[*(uint8*)((long)(fbuf++) ^ 1)];

            return true;
        }
    }

    return false;
}

uint16 EmRegsSED1375::GetLCD2bitMapping() { return 0xfa50; }

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::invalidateWrite
// ---------------------------------------------------------------------------

void EmRegsSED1375::invalidateWrite(emuptr address, int size, uint32 value) {
    this->StdWriteBE(address, size, value);
    gSystemState.MarkScreenDirty();
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::vertNonDisplayRead
// ---------------------------------------------------------------------------

uint32 EmRegsSED1375::vertNonDisplayRead(emuptr address, int size) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    // Always set the vertical non-display status high since in the real
    // hardware, the ROM will check this flag in order to write the CLUT
    // registers.

    return (fRegs.verticalNonDisplayPeriod) | sed1375VerticalNonDisplayStatus;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::lookUpTableAddressWrite
// ---------------------------------------------------------------------------

void EmRegsSED1375::lookUpTableAddressWrite(emuptr address, int size, uint32 value) {
    this->StdWriteBE(address, size, value);

    lutEntry = value & 0xff;
    lutColorIndex = LutColorIndex::red;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::lookUpTableDataRead
// ---------------------------------------------------------------------------

uint32 EmRegsSED1375::lookUpTableDataRead(emuptr address, int size) {
    EmAssert(size == 1);

    if (size != 1) return 0;  // Error case.

    uint32 entry = fClutData[lutEntry];

    switch (lutColorIndex) {
        case LutColorIndex::red:
            lutColorIndex = LutColorIndex::green;
            return entry & 0xf0;

        case LutColorIndex::green:
            lutColorIndex = LutColorIndex::blue;
            return (entry >> 8) & 0xf0;

        case LutColorIndex::blue:
            lutEntry++;
            lutColorIndex = LutColorIndex::red;
            return (entry >> 16) & 0xf0;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1375::lookUpTableDataWrite
// ---------------------------------------------------------------------------

void EmRegsSED1375::lookUpTableDataWrite(emuptr address, int size, uint32 value) {
    EmAssert(size == 1);

    if (size != 1) return;  // Error case.

    value &= 0xf0;

    switch (lutColorIndex) {
        case LutColorIndex::red:
            fClutData[lutEntry] = (fClutData[lutEntry] & 0xffffff00) | value;
            lutColorIndex = LutColorIndex::green;
            break;

        case LutColorIndex::green:
            fClutData[lutEntry] = (fClutData[lutEntry] & 0xffff00ff) | (value << 8);
            lutColorIndex = LutColorIndex::blue;
            break;

        case LutColorIndex::blue:
            fClutData[lutEntry] = (fClutData[lutEntry] & 0xff00ffff) | (value << 16);
            lutEntry++;
            lutColorIndex = LutColorIndex::red;
            break;
    }

    gSystemState.MarkScreenDirty();
}

void EmRegsSED1375::ClearLut() {
    for (int i = 0; i < 256; i++) {
        fClutData[i] = 0xff000000;
    }
}
