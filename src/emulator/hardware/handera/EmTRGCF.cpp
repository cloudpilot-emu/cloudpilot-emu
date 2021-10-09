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

#include "EmTRGCF.h"

#include "EmCommon.h"
#include "EmMemory.h"

//-------------------------------------------------------------------------
// This file contains the class EmRegsCFMemCard, which emulates a
// CompactFlash memory card.  The bulk of this class is a series of
// routines to dispatch calls to the appropriate section of a memory
// card ... either its Tuple memory, its Configuration Registers, or its
// ATA registers.  This class also handles all decisions about how to
// emulate the effect of the current bus state (8-bit vs. 16-bit, swap vs.
// no swap) on the data.
//-------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::EmRegsCFMemCard
// ---------------------------------------------------------------------------

EmRegsCFMemCard::EmRegsCFMemCard(CFBusManager* fBusManager) { fBusMgr = fBusManager; }

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::~EmRegsCFMemCard
// ---------------------------------------------------------------------------

EmRegsCFMemCard::~EmRegsCFMemCard(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::Initialize
// ---------------------------------------------------------------------------

void EmRegsCFMemCard::Initialize(void) {
    Ata.Initialize();
    Config.Initialize();
    Tuple.Initialize();
    CFReset.Initialize();
}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::Reset
// ---------------------------------------------------------------------------
void EmRegsCFMemCard::Reset(Bool /*hardwareReset*/) { Ata.Reset(); }

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::Dispose
// ---------------------------------------------------------------------------
void EmRegsCFMemCard::Dispose(void) { Ata.Dispose(); }

// -------------------------------------------------------------------------
//		� EmRegsCFMemCard::SetSubBankHandlers
// ---------------------------------------------------------------------------
void EmRegsCFMemCard::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();
    // Now add standard/specialized handers for the defined registers.
    this->SetHandler((ReadFunction)&EmRegsCFMemCard::Read, (WriteFunction)&EmRegsCFMemCard::Write,
                     kMemoryStartCF, kMemorySizeCF);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::GetRealAddress
// ---------------------------------------------------------------------------
uint8* EmRegsCFMemCard::GetRealAddress(emuptr address) {
    return (address - kMemoryStartCF) + Buffer;
}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::GetLong
// ---------------------------------------------------------------------------
uint32 EmRegsCFMemCard::GetLong(emuptr /*address*/) { return (0); }

#define INVALID_READ 0x3F

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::GetByte
// ---------------------------------------------------------------------------
uint32 EmRegsCFMemCard::GetByte(emuptr address) {
    uint8 retVal = 0;
    uint32 offset;

    if (!fBusMgr->bEnabled) return (INVALID_READ);

    if ((fBusMgr->Width == kCFBusWidth8) && (!fBusMgr->bSwapped)) return (INVALID_READ);

    offset = address - kMemoryStartCF;
    if (offset < kMemoryOffsetCFConfig)
        Tuple.ReadByte(offset - kMemoryOffsetCFTuple, &retVal);
    else if (offset < kMemoryOffsetCFAta)
        Config.ReadByte(offset - kMemoryOffsetCFConfig, &retVal);
    else if (offset < kMemoryOffsetCFReset)
        Ata.ReadByte(offset - kMemoryOffsetCFAta, &retVal);
    else
        CFReset.ReadByte(offset - kMemoryOffsetCFReset, &retVal);

    return ((uint32)retVal);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::GetWord
// ---------------------------------------------------------------------------
uint32 EmRegsCFMemCard::GetWord(emuptr address) {
    _Word val;
    uint32 retval;
    int msb, lsb;
    uint32 offset;

    if (!fBusMgr->bEnabled) return (INVALID_READ);
    if ((fBusMgr->Width == kCFBusWidth8) && (!fBusMgr->bSwapped)) return (INVALID_READ);
    offset = address - kMemoryStartCF;

    if (offset < kMemoryOffsetCFConfig)
        Tuple.ReadWord(offset - kMemoryOffsetCFTuple, &val);
    else if (offset < kMemoryOffsetCFAta)
        Config.ReadWord(offset - kMemoryOffsetCFConfig, &val);
    else if (offset < kMemoryOffsetCFReset)
        Ata.ReadWord(offset - kMemoryOffsetCFAta, &val);
    else
        CFReset.ReadWord(offset - kMemoryOffsetCFReset, &val);

    if (fBusMgr->bSwapped) {
        msb = 0;
        lsb = 1;
    } else {
        msb = 1;
        lsb = 0;
    }
    retval = ((uint32)(val.Bytes[msb]) << 8) | (uint32)val.Bytes[lsb];
    return (retval);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::SetLong
// ---------------------------------------------------------------------------
void EmRegsCFMemCard::SetLong(emuptr /*address*/, uint32 /*value*/) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::SetWord
// ---------------------------------------------------------------------------
void EmRegsCFMemCard::SetWord(emuptr address, uint32 val) {
    _Word setVal;
    int msb, lsb;

    if (!fBusMgr->bEnabled) return;

    if (fBusMgr->bSwapped) {
        msb = 0;
        lsb = 1;
    } else {
        msb = 1;
        lsb = 0;
    }
    setVal.Bytes[msb] = (uint8)((val & 0xFF00) >> 8);
    setVal.Bytes[lsb] = (uint8)(val & 0x00FF);

    if (address < kMemoryStartCFConfig)
        Tuple.WriteWord(address - kMemoryStartCFTuple, setVal);
    else if (address < kMemoryStartCFAta)
        Config.WriteWord(address - kMemoryStartCFConfig, setVal);
    else if (address < kMemoryStartCFReset)
        Ata.WriteWord(address - kMemoryStartCFAta, setVal);
    else
        CFReset.WriteWord(address - kMemoryStartCFReset, setVal);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::SetByte
// ---------------------------------------------------------------------------
void EmRegsCFMemCard::SetByte(emuptr address, uint32 value) {
    uint8 setVal = 0;

    if (!fBusMgr->bEnabled) return;

    setVal = (uint8)value;
    if (address < kMemoryStartCFConfig)
        Tuple.WriteByte(address - kMemoryStartCFTuple, setVal);
    else if (address < kMemoryStartCFAta)
        Config.WriteByte(address - kMemoryStartCFConfig, setVal);
    else if (address < kMemoryStartCFReset)
        Ata.WriteByte(address - kMemoryStartCFAta, setVal);
    else
        CFReset.WriteByte(address - kMemoryStartCFReset, setVal);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::GetAddressStart
// ---------------------------------------------------------------------------
emuptr EmRegsCFMemCard::GetAddressStart(void) { return (kMemoryStartCF); }

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::GetAddressRange
// ---------------------------------------------------------------------------
uint32 EmRegsCFMemCard::GetAddressRange(void) { return kMemorySizeCF; }

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::Read
// ---------------------------------------------------------------------------
uint32 EmRegsCFMemCard::Read(emuptr address, int size) {
    switch (size) {
        case sizeof(uint8):
            return (GetByte(address));
        case sizeof(uint16):
            return (GetWord(address));
    }
    return 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsCFMemCard::Write
// ---------------------------------------------------------------------------
void EmRegsCFMemCard::Write(emuptr address, int size, uint32 val) {
    switch (size) {
        case sizeof(uint8):
            SetByte(address, val);
            break;
        case sizeof(uint16):
            SetWord(address, val);
            break;
    }
}
