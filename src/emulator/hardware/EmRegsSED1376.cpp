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

#include "EmRegsSED1376.h"

#include "EmCommon.h"
#include "EmMemory.h"  // EmMem_memcpy
#include "EmSystemState.h"
#include "Frame.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

// Given a register (specified by its field name), return its address
// in emulated space.

#define addressof(reg) (this->GetAddressStart() + fRegs.offsetof_##reg())

// Macro to help the installation of handlers for a register.

#define INSTALL_HANDLER(read, write, reg)                                                      \
    this->SetHandler((ReadFunction)&EmRegsSED1376::read, (WriteFunction)&EmRegsSED1376::write, \
                     addressof(reg), fRegs.reg.GetSize())

// Panel type register [10h]
#define sed1376MonoMask 0x40

// Display mode register [70h]
#define sed1376DisplayBlankMask 0x80
#define sed1376BPPMask 0x07
#define sed1376BPPShift 0x00

// REG[0x70] displayMode register
// (Same as previous set, but with Handspring names for their contributed code)
#define sed1376DisplayModeBlank 0x80
#define sed1376DisplayModeDitherOff 0x40
#define sed1376DisplayModeHwInvert 0x20
#define sed1376DisplayModeSwInvert 0x10
#define sed1376DisplayModeDepthMask 0x07
#define sed1376DisplayModeDepth16 0x04
#define sed1376DisplayModeDepth8 0x03
#define sed1376DisplayModeDepth4 0x02
#define sed1376DisplayModeDepth2 0x01
#define sed1376DisplayModeDepth1 0x00

// Special effects register [71h]
#define sed1376WordSwapMask 0x80
#define sed1376ByteSwapMask 0x40
#define sed1376SubWindowEnableMask 0x10

#define hwrDisplayGPIOPCI 0x08     // in GPIOStatusControl0
#define hwrDisplayGPIOEL_ON 0x10   // in GPIOStatusControl0
#define hwrDisplayGPIOLCD_ON 0x20  // in GPIOStatusControl0
#define hwrDisplayGPIOMOD 0x40     // in GPIOStatusControl0

// The current SED1376 driver code is completely whacked.  It's initialized to use
// the main display for drawing (see HwrDisplayInit in HwrDisplayBootSED1376.c), but
// if you change the base address, it (a) always returns the base of the embedded
// SRAM buffer as the old base address, and (b) stores the new base address in
// ovlyStartAddress (see PrvDisplayBaseAddr in HwrDisplaySED1376.c).  Also, the
// splash screen appears to be drawn relative to ovlyStartAddress.  Finally,
// there appears to be code to draw a border in the main screen around the
// overlay screen (see PrvDisplayBorder in HwrDisplayBootSED1376.c), however,
// the mainStartAddress registers are never changed to point to this border.
// On top of that, the border is only programmed on the left and right of the
// of the overlay screen, not on the top and bottom.

#define OVERLAY_IS_MAIN 1

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::EmRegsSED1376
// ---------------------------------------------------------------------------

EmRegsSED1376::EmRegsSED1376(emuptr baseRegsAddr, emuptr baseVideoAddr)
    : fBaseRegsAddr(baseRegsAddr), fBaseVideoAddr(baseVideoAddr), fRegs() {}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::~EmRegsSED1376
// ---------------------------------------------------------------------------

EmRegsSED1376::~EmRegsSED1376(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::Initialize
// ---------------------------------------------------------------------------

void EmRegsSED1376::Initialize(void) { EmRegs::Initialize(); }

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::Reset
// ---------------------------------------------------------------------------

void EmRegsSED1376::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);

    if (hardwareReset) {
        memset(fRegs.GetPtr(), 0, fRegs.GetSize());

        //		EmAssert ((sed1376ProductCodeExpected | sed1376RevisionCodeExpected) ==
        // 0x28);
        fRegs.productRevisionCode = 0x28;
        fRegs.displayBufferSize = 20;  // 80K / 4K
        fRegs.configurationReadback = 0;
    }
}

void EmRegsSED1376::Save(Savestate& savestate) { savestate.NotifyError(); }

void EmRegsSED1376::Save(SavestateProbe& prove) {}

void EmRegsSED1376::Load(SavestateLoader& loader) {}

#if 0

void void::Save(SessionFile& f) {
    EmRegs::Save(f);

    f.WriteSED1376RegsType(*(SED1376RegsType*)fRegs.GetPtr());
    f.WriteSED1376Palette(fClutData);
}

void EmRegsSED1376::Load(SessionFile& f) {
    EmRegs::Load(f);

    // Read in the SED registers.

    if (!f.ReadSED1376RegsType(*(SED1376RegsType*)fRegs.GetPtr())) {
        f.SetCanReload(false);
    }

    // Read in the LCD palette.

    if (!f.ReadSED1376Palette(fClutData)) {
        f.SetCanReload(false);
    }
}

#endif

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::Dispose
// ---------------------------------------------------------------------------

void EmRegsSED1376::Dispose(void) { EmRegs::Dispose(); }

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsSED1376::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(StdReadBE, NullWrite, productRevisionCode);
    INSTALL_HANDLER(StdReadBE, NullWrite, displayBufferSize);
    INSTALL_HANDLER(StdReadBE, NullWrite, configurationReadback);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, memoryClockConfiguration);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, pixelClockConfiguration);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, lutWriteBlue);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, lutWriteGreen);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, lutWriteRed);
    INSTALL_HANDLER(ZeroRead, lutWriteAddressWrite, lutWriteAddress);
    INSTALL_HANDLER(StdReadBE, NullWrite, lutReadBlue);
    INSTALL_HANDLER(StdReadBE, NullWrite, lutReadGreen);
    INSTALL_HANDLER(StdReadBE, NullWrite, lutReadRed);
    INSTALL_HANDLER(ZeroRead, lutReadAddressWrite, lutReadAddress);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, panelType);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, MODRate);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, horizontalTotal);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, horizontalPeriod);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, horizontalPeriodStart0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, horizontalPeriodStart1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, verticalTotal0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, verticalTotal1);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, verticalPeriod0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, verticalPeriod1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, verticalPeriodStart0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, verticalPeriodStart1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FPLINEPulseWidth);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FPLINEPulseStart0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FPLINEPulseStart1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FPFRAMEPulseWidth);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FPFRAMEPulseStart0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, FPFRAMEPulseStart1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, DTFD_GCPIndex);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, DTFD_GCPData);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, displayMode);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, specialEffects);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, mainStartAddress0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, mainStartAddress1);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, mainStartAddress2);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, mainLineAddressOffset0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, mainLineAddressOffset1);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyStartAddress0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyStartAddress1);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyStartAddress2);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyLineAddressOffset0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyLineAddressOffset1);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyStartXPosition0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyStartXPosition1);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyStartYPosition0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyStartYPosition1);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyEndXPosition0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyEndXPosition1);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyEndYPosition0);
    INSTALL_HANDLER(StdReadBE, invalidateWrite, ovlyEndYPosition1);
    INSTALL_HANDLER(powerSaveConfigurationRead, StdWriteBE, powerSaveConfiguration);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, SoftwareReset);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, scratchPad0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, scratchPad1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GPIOConfiguration0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GPIOConfiguration1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GPIOStatusControl0);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, GPIOStatusControl1);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, PWMClockCVPulseControl);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, PWMClockCVPulseConfig);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, CVPulseBurstLength);
    INSTALL_HANDLER(StdReadBE, StdWriteBE, PWMOutDutyCycle);
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsSED1376::GetRealAddress(emuptr address) {
    return (uint8*)fRegs.GetPtr() + address - this->GetAddressStart();
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsSED1376::GetAddressStart(void) { return fBaseRegsAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsSED1376::GetAddressRange(void) { return fRegs.GetSize(); }

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsSED1376::GetLCDScreenOn(void) {
    //	return ((fRegs.displayMode) & sed1376DisplayBlankMask) == 0;
    return ((fRegs.GPIOStatusControl0) & hwrDisplayGPIOLCD_ON) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsSED1376::GetLCDBacklightOn(void) {
    return ((fRegs.GPIOStatusControl0) & hwrDisplayGPIOEL_ON) != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmRegsSED1376::GetLCDHasFrame(void) { return true; }

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::invalidateWrite
// ---------------------------------------------------------------------------

void EmRegsSED1376::invalidateWrite(emuptr address, int size, uint32 value) {
    this->StdWriteBE(address, size, value);
    gSystemState.MarkScreenDirty();
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::powerSaveConfigurationRead
// ---------------------------------------------------------------------------

uint32 EmRegsSED1376::powerSaveConfigurationRead(emuptr address, int size) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    // Always set the vertical non-display status high since in the real
    // hardware, the ROM will check this flag in order to write the CLUT
    // registers.

    return (fRegs.powerSaveConfiguration) | 0x80;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::lutWriteAddressWrite
// ---------------------------------------------------------------------------

void EmRegsSED1376::lutWriteAddressWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    value &= 0x0FF;

    uint8 red = fRegs.lutWriteRed;
    uint8 green = fRegs.lutWriteGreen;
    uint8 blue = fRegs.lutWriteBlue;

    fClutData[value] = RGBType((red & 0xFC) | (red >> 6), (green & 0xFC) | (green >> 6),
                               (blue & 0xFC) | (blue >> 6));

    gSystemState.MarkScreenDirty();
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376::lutReadAddressWrite
// ---------------------------------------------------------------------------

void EmRegsSED1376::lutReadAddressWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    value &= 0x0FF;

    RGBType rgb = fClutData[value];

    fRegs.lutReadRed = rgb.fRed & 0xFC;
    fRegs.lutReadGreen = rgb.fGreen & 0xFC;
    fRegs.lutReadBlue = rgb.fBlue & 0xFC;
}

inline void EmRegsSED1376::SetFromPalette(uint8* target, uint16 index, bool mono) {
    RGBType entry = fClutData[index];

    if (mono)
        target[0] = target[1] = target[2] = entry.fGreen;
    else {
        target[0] = entry.fRed;
        target[1] = entry.fGreen;
        target[2] = entry.fBlue;
    }
}

#if 0

// ---------------------------------------------------------------------------
//		� EmRegsSED1376VisorPrism::EmRegsSED1376VisorPrism
// ---------------------------------------------------------------------------

EmRegsSED1376VisorPrism::EmRegsSED1376VisorPrism(emuptr baseRegsAddr, emuptr baseVideoAddr)
    : EmRegsSED1376(baseRegsAddr, baseVideoAddr) {}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376VisorPrism::~EmRegsSED1376VisorPrism
// ---------------------------------------------------------------------------

EmRegsSED1376VisorPrism::~EmRegsSED1376VisorPrism(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376VisorPrism::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsSED1376VisorPrism::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegsSED1376::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    #undef INSTALL_HANDLER
    #define INSTALL_HANDLER(read, write, reg)                                            \
        this->SetHandler((ReadFunction)&EmRegsSED1376VisorPrism::read,                   \
                         (WriteFunction)&EmRegsSED1376VisorPrism::write, addressof(reg), \
                         fRegs.reg.GetSize())

    INSTALL_HANDLER(StdReadBE, reservedWrite, reserved);
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376VisorPrism::GetLCDBeginEnd
// ---------------------------------------------------------------------------

void EmRegsSED1376VisorPrism::GetLCDBeginEnd(emuptr& begin, emuptr& end) {
    // Get the screen metrics.

    // The hardware is written to in reverse, so the mainStartOffsetX registers
    // report the END of the frame buffer, not the beginning.
    emuptr baseAddr = fBaseVideoAddr;

    int32 width = ((fRegs.horizontalPeriod + 1) * 8);
    int32 height = ((fRegs.verticalPeriod1 << 8) | fRegs.verticalPeriod0) + 1;
    int32 rowBytes = ((width * this->PrvGetLCDDepth()) / 8);

    begin = baseAddr;
    end = baseAddr + rowBytes * height;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376VisorPrism::GetLCDScanlines
// ---------------------------------------------------------------------------

void EmRegsSED1376VisorPrism::GetLCDScanlines(EmScreenUpdateInfo& info) {
    // Get the screen metrics.

    Bool byteSwapped = (fRegs.specialEffects & sed1376ByteSwapMask) != 0;
    Bool mono = (fRegs.displayMode & sed1376MonoMask) != 0;
    int32 bpp = 1 << ((fRegs.displayMode & sed1376BPPMask) >> sed1376BPPShift);

    // The hardware is written to in reverse, so the mainStartOffsetX registers
    // report the END of the frame buffer, not the beginning.
    emuptr baseAddr = fBaseVideoAddr;

    int32 width = ((fRegs.horizontalPeriod + 1) * 8);
    int32 height = ((fRegs.verticalPeriod1 << 8) | fRegs.verticalPeriod0) + 1;
    int32 rowBytes = ((width * this->PrvGetLCDDepth()) / 8);

    info.fLeftMargin = 0;

    if (bpp <= 8) {
        EmPixMapFormat format = bpp == 1   ? kPixMapFormat1
                                : bpp == 2 ? kPixMapFormat2
                                : bpp == 4 ? kPixMapFormat4
                                           : kPixMapFormat8;

        RGBList colorTable;
        this->PrvGetPalette(colorTable);

        // Set format, size, and color table of EmPixMap.

        info.fImage.SetSize(EmPoint(width, height));
        info.fImage.SetFormat(format);
        info.fImage.SetRowBytes(rowBytes);
        info.fImage.SetColorTable(colorTable);

        // Determine first and last scanlines to fetch, and fetch them.

        info.fFirstLine = (info.fScreenLow - baseAddr) / rowBytes;
        info.fLastLine = (info.fScreenHigh - baseAddr - 1) / rowBytes + 1;

        long firstLineOffset = info.fFirstLine * rowBytes;
        long lastLineOffset = info.fLastLine * rowBytes;

        EmMem_memcpy((void*)((uint8*)info.fImage.GetBits() + firstLineOffset),
                     baseAddr + firstLineOffset, lastLineOffset - firstLineOffset);
    } else {
        // Set depth, size, and color table of EmPixMap.

        info.fImage.SetSize(EmPoint(width, height));
        info.fImage.SetFormat(kPixMapFormat24RGB);

        // Determine first and last scanlines to fetch.

        info.fFirstLine = (info.fScreenLow - baseAddr) / rowBytes;
        info.fLastLine = (info.fScreenHigh - baseAddr - 1) / rowBytes + 1;

        // Get location and rowBytes of source bytes.

        uint8* srcStart = EmMemGetRealAddress(baseAddr);
        int32 srcRowBytes = rowBytes;
        uint8* srcPtr = srcStart + srcRowBytes * info.fFirstLine;
        uint8* srcPtr0 = srcPtr;

        // Get location and rowBytes of destination bytes.

        uint8* destStart = (uint8*)info.fImage.GetBits();
        int32 destRowBytes = info.fImage.GetRowBytes();
        uint8* destPtr = destStart + destRowBytes * info.fFirstLine;
        uint8* destPtr0 = destPtr;

        // Get height of range to copy.

        int32 height = info.fLastLine - info.fFirstLine;

        // Copy the pixels from source to dest.

        for (int yy = 0; yy < height; ++yy) {
            for (int xx = 0; xx < width; ++xx) {
                uint8 p1 = EmMemDoGet8(srcPtr++);  // GGGBBBBB
                uint8 p2 = EmMemDoGet8(srcPtr++);  // RRRRRGGG

                // Merge the two together so that we get RRRRRGGG GGGBBBBB

                uint16 p;

                if (!byteSwapped)
                    p = (p2 << 8) | p1;
                else
                    p = (p1 << 8) | p2;

                // Shift the bits around, forming RRRRRrrr, GGGGGGgg, and
                // BBBBBbbb values, where the lower-case bits are copies of
                // the least significant bits in the upper-case bits.
                //
                // Note that all of this could also be done with three 64K
                // lookup tables.  If speed is an issue, we might want to
                // investigate that.

                if (mono) {
                    uint8 green = ((p >> 3) & 0xFC) | ((p >> 5) & 0x03);
                    *destPtr++ = green;
                    *destPtr++ = green;
                    *destPtr++ = green;
                } else {
                    *destPtr++ = ((p >> 8) & 0xF8) | ((p >> 11) & 0x07);
                    *destPtr++ = ((p >> 3) & 0xFC) | ((p >> 5) & 0x03);
                    *destPtr++ = ((p << 3) & 0xF8) | ((p >> 0) & 0x07);
                }
            }

            srcPtr = srcPtr0 += srcRowBytes;
            destPtr = destPtr0 += destRowBytes;
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376VisorPrism::PrvGetLCDDepth
// ---------------------------------------------------------------------------

int32 EmRegsSED1376VisorPrism::PrvGetLCDDepth(void) {
    UInt8 depth = fRegs.displayMode;

    depth &= sed1376DisplayModeDepthMask;

    switch (depth) {
        case sed1376DisplayModeDepth16:
            return 16;
        case sed1376DisplayModeDepth8:
            return 8;
        case sed1376DisplayModeDepth4:
            return 4;
        case sed1376DisplayModeDepth2:
            return 2;
        case sed1376DisplayModeDepth1:
            return 1;
        default:
            EmAssert(false);
    }

    return 8;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376VisorPrism::reservedWrite
// ---------------------------------------------------------------------------

void EmRegsSED1376VisorPrism::reservedWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address);
    UNUSED_PARAM(size);
    UNUSED_PARAM(value);
}

#endif

// ---------------------------------------------------------------------------
//		� EmRegsSED1376PalmGeneric::EmRegsSED1376PalmGeneric
// ---------------------------------------------------------------------------

EmRegsSED1376PalmGeneric::EmRegsSED1376PalmGeneric(emuptr baseRegsAddr, emuptr baseVideoAddr)
    : EmRegsSED1376(baseRegsAddr, baseVideoAddr) {}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376PalmGeneric::~EmRegsSED1376PalmGeneric
// ---------------------------------------------------------------------------

EmRegsSED1376PalmGeneric::~EmRegsSED1376PalmGeneric(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376PalmGeneric::GetLCDBeginEnd
// ---------------------------------------------------------------------------

void EmRegsSED1376PalmGeneric::GetLCDBeginEnd(emuptr& begin, emuptr& end) {
    // Get the screen metrics.

//	Bool	wordSwapped	= (fRegs.specialEffects & sed1376WordSwapMask) != 0;
//	Bool	byteSwapped	= (fRegs.specialEffects & sed1376ByteSwapMask) != 0;
//	int32	bpp			= 1 << ((fRegs.displayMode & sed1376BPPMask) >>
// sed1376BPPShift);
#if !OVERLAY_IS_MAIN
    //	int32	width		= ((fRegs.horizontalPeriod + 1) * 8);
    int32 height = ((fRegs.verticalPeriod1 << 8) | fRegs.verticalPeriod0) + 1;
    int32 rowBytes = ((fRegs.mainLineAddressOffset1 << 8) | fRegs.mainLineAddressOffset0) * 4;
    uint32 offset = (fRegs.mainStartAddress2 << 18) | (fRegs.mainStartAddress1 << 10) |
                    (fRegs.mainStartAddress0 << 2);
#else
    //	int32	left		= ((fRegs.ovlyStartXPosition1	<< 8) | fRegs.ovlyStartXPosition0)
    //* 32
    /// bpp; 	int32	right		= ((fRegs.ovlyEndXPosition1		<< 8) |
    // fRegs.ovlyEndXPosition0)	* 32 / bpp;
    int32 top = ((fRegs.ovlyStartYPosition1 << 8) | fRegs.ovlyStartYPosition0);
    int32 bottom = ((fRegs.ovlyEndYPosition1 << 8) | fRegs.ovlyEndYPosition0);

    //	int32	width		= right - left + (32 / bpp);
    int32 height = bottom - top + 1;
    int32 rowBytes = ((fRegs.ovlyLineAddressOffset1 << 8) | fRegs.ovlyLineAddressOffset0) * 4;
    uint32 offset = (fRegs.ovlyStartAddress2 << 18) | (fRegs.ovlyStartAddress1 << 10) |
                    (fRegs.ovlyStartAddress0 << 2);
#endif
    emuptr baseAddr = fBaseVideoAddr + offset;

    begin = baseAddr;
    end = baseAddr + rowBytes * height;
}

// ---------------------------------------------------------------------------
//		� EmRegsSED1376PalmGeneric::GetLCDScanlines
// ---------------------------------------------------------------------------

bool EmRegsSED1376PalmGeneric::CopyLCDFrame(Frame& frame) {
    // Get the screen metrics.

    //	Bool	wordSwapped	= (fRegs.specialEffects & sed1376WordSwapMask) != 0;
    Bool byteSwapped = (fRegs.specialEffects & sed1376ByteSwapMask) != 0;
    Bool mono = (fRegs.displayMode & sed1376MonoMask) != 0;
    int32 bpp = 1 << ((fRegs.displayMode & sed1376BPPMask) >> sed1376BPPShift);
#if !OVERLAY_IS_MAIN
    int32 width = ((fRegs.horizontalPeriod + 1) * 8);
    int32 height = ((fRegs.verticalPeriod1 << 8) | fRegs.verticalPeriod0) + 1;
    int32 rowBytes = ((fRegs.mainLineAddressOffset1 << 8) | fRegs.mainLineAddressOffset0) * 4;
    uint32 offset = (fRegs.mainStartAddress2 << 18) | (fRegs.mainStartAddress1 << 10) |
                    (fRegs.mainStartAddress0 << 2);
#else
    int32 left = ((fRegs.ovlyStartXPosition1 << 8) | fRegs.ovlyStartXPosition0) * 32 / bpp;
    int32 right = ((fRegs.ovlyEndXPosition1 << 8) | fRegs.ovlyEndXPosition0) * 32 / bpp;
    int32 top = ((fRegs.ovlyStartYPosition1 << 8) | fRegs.ovlyStartYPosition0);
    int32 bottom = ((fRegs.ovlyEndYPosition1 << 8) | fRegs.ovlyEndYPosition0);

    int32 width = right - left + (32 / bpp);
    int32 height = bottom - top + 1;
    int32 rowBytes = ((fRegs.ovlyLineAddressOffset1 << 8) | fRegs.ovlyLineAddressOffset0) * 4;
    uint32 offset = (fRegs.ovlyStartAddress2 << 18) | (fRegs.ovlyStartAddress1 << 10) |
                    (fRegs.ovlyStartAddress0 << 2);
#endif
    if (width != 160 || height != 160) return false;

    emuptr baseAddr = fBaseVideoAddr + offset;

    frame.bpp = 24;
    frame.lineWidth = width;
    frame.lines = height;
    frame.margin = 0;
    frame.bytesPerLine = width * 3;
    if (3 * width * height > static_cast<ssize_t>(frame.GetBufferSize())) return false;
    uint8* buffer = frame.GetBuffer();

    switch (bpp) {
        case 1:
            for (int32 y = 0; y < height; y++)
                for (int32 x = 0; x < width; x++) {
                    SetFromPalette(
                        buffer,
                        (EmMemGet8(baseAddr + y * rowBytes + x / 8) >> (7 - (x % 8))) & 0x01, mono);

                    buffer += 3;
                }

            break;

        case 2:
            for (int32 y = 0; y < height; y++)
                for (int32 x = 0; x < width; x++) {
                    SetFromPalette(
                        buffer,
                        (EmMemGet8(baseAddr + y * rowBytes + x / 4) >> 2 * (3 - (x % 4))) & 0x03,
                        mono);

                    buffer += 3;
                }

            break;

        case 4:
            for (int32 y = 0; y < height; y++)
                for (int32 x = 0; x < width; x++) {
                    SetFromPalette(
                        buffer,
                        (EmMemGet8(baseAddr + y * rowBytes + x / 2) >> 4 * (1 - (x % 2))) & 0x0f,
                        mono);

                    buffer += 3;
                }

            break;

        case 8:
            for (int32 y = 0; y < height; y++)
                for (int32 x = 0; x < width; x++) {
                    SetFromPalette(buffer, EmMemGet8(baseAddr + y * rowBytes + x), mono);

                    buffer += 3;
                }

            break;

        default:
            for (int32 y = 0; y < height; y++)
                for (int32 x = 0; x < width; x++) {
                    uint8 p1 = EmMemGet8(baseAddr++);  // GGGBBBBB
                    uint8 p2 = EmMemGet8(baseAddr++);  // RRRRRGGG

                    // Merge the two together so that we get RRRRRGGG GGGBBBBB

                    uint16 p;

                    if (!byteSwapped)
                        p = (p2 << 8) | p1;
                    else
                        p = (p1 << 8) | p2;

                    // Shift the bits around, forming RRRRRrrr, GGGGGGgg, and
                    // BBBBBbbb values, where the lower-case bits are copies of
                    // the least significant bits in the upper-case bits.
                    //
                    // Note that all of this could also be done with three 64K
                    // lookup tables.  If speed is an issue, we might want to
                    // investigate that.

                    if (mono) {
                        uint8 green = ((p >> 3) & 0xFC) | ((p >> 5) & 0x03);
                        *buffer++ = green;
                        *buffer++ = green;
                        *buffer++ = green;
                    } else {
                        *buffer++ = ((p >> 8) & 0xF8) | ((p >> 11) & 0x07);
                        *buffer++ = ((p >> 3) & 0xFC) | ((p >> 5) & 0x03);
                        *buffer++ = ((p << 3) & 0xF8) | ((p >> 0) & 0x07);
                    }
                }

            break;
    }

    return true;
}

uint16 EmRegsSED1376PalmGeneric::GetLCD2bitMapping() { return 0xfa50; }
