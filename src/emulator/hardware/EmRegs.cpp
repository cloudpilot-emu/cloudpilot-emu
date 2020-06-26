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

#include "EmRegs.h"

#include "Byteswapping.h"  // Canonical
#include "EmBankRegs.h"    // EmBankRegs::InvalidAccess
#include "EmCommon.h"
#include "EmMemory.h"  // Memory::InitializeBanks, EmMemBankIndex

/*
        EmRegs is a base class for subclasses that manage the accessing of
        emulated memory at a byte, word, or long level.  See comments in
        EmRegsBank.cpp for a description on how EmRegs objects are managed,
        and EmMemory for how memory in general is emulated.

        EmRegs subclasses are suited for managing hardware registers, such
        as the Dragonball [EZ | VZ] hardware registers, the USB registers
        in the Handspring Visor, or the special PLD registers in some of the
        Palm VII devices.

        To create a new EmRegs subclass that can handle accesses to a special
        range of memory, do the following:

        * Create a subclass of EmRegs

        * Provide implementations for the pure virtual functions:

                * SetSubBankHandlers: this method needs to call EmRegs::SetHandler
                  to install a read and write function for each memory location
                  that can be accessed.  It should also first call the base
                  class SetSubBankHandlers in order to install default handlers
                  for all memory locations in the memory range the subclass manages.

                * GetRealAddress: given an address in emulated space, this function
                  returns the real address from the point of view of the emulator.
                  For instance, if your EmRegs subclass manages the memory range
                  0x17000000 to 0x17001000, and you have created a 4K buffer to
                  represent this memory range, then GetRealAddress would be like:

                        return &fMyBuffer[address - 0x17000000];

                * GetAddressStart: returns the base address of the memory range
                  managed by this class.  In the example above, this method would
                  return 0x17000000.

                * GetAddressRange: returns the range of memory managed by this
                  class.  In the example above, this method would return 0x1000.

        * Define read and write function handlers for each memory location
          in the managed range.  These functions handlers are installed in
          your SetSubBankHandlers override.  EmBankRegs will call these
          handlers as appropriate.

        * Optionally implement Initialize, Reset, Save, Load, and Dispose
          overrides.  You will usually override Initialize to create any
          buffers required to hold your data, and correspondingly dispose
          of those resources in a Dispose override.  Most subclasses will
          need to override Reset in order to initialize their emulated
          registers.  And override Save and Load if you want to save and
          restore your registers to the session (.psf) file.

        * In the appropriate case statement in EmDevice::CreateRegs, create
          an instance of your EmRegs class and install it by calling
          EmBankRegs::AddSubBank.
*/

// ---------------------------------------------------------------------------
//		� EmRegs::EmRegs
// ---------------------------------------------------------------------------

EmRegs::EmRegs(void) : fReadFunctions(), fWriteFunctions() {}

// ---------------------------------------------------------------------------
//		� EmRegs::~EmRegs
// ---------------------------------------------------------------------------

EmRegs::~EmRegs(void) {}

// ---------------------------------------------------------------------------
//		� EmRegs::Initialize
// ---------------------------------------------------------------------------

void EmRegs::Initialize(void) {}

// ---------------------------------------------------------------------------
//		� EmRegs::Reset
// ---------------------------------------------------------------------------

void EmRegs::Reset(Bool /*hardwareReset*/) {}

// ---------------------------------------------------------------------------
//		� EmRegs::Save
// ---------------------------------------------------------------------------

void EmRegs::Save(SessionFile&) {}

// ---------------------------------------------------------------------------
//		� EmRegs::Load
// ---------------------------------------------------------------------------

void EmRegs::Load(SessionFile&) {}

// ---------------------------------------------------------------------------
//		� EmRegs::Dispose
// ---------------------------------------------------------------------------

void EmRegs::Dispose(void) {}

// ---------------------------------------------------------------------------
//		� EmRegs::SetBankHandlers
// ---------------------------------------------------------------------------

void EmRegs::SetBankHandlers(EmAddressBank& bank) {
    emuptr address = this->GetAddressStart();
    uint32 range = this->GetAddressRange();
    uint32 numBanks = EmMemBankIndex(address + range - 1) - EmMemBankIndex(address) + 1;

    Memory::InitializeBanks(bank, EmMemBankIndex(address), numBanks);

    this->SetSubBankHandlers();
}

// ---------------------------------------------------------------------------
//		� EmRegs::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegs::SetSubBankHandlers(void) {
    this->SetHandler(&EmRegs::UnsupportedRead, &EmRegs::UnsupportedWrite, this->GetAddressStart(),
                     this->GetAddressRange());
}

// ---------------------------------------------------------------------------
//		� EmRegs::GetLong
// ---------------------------------------------------------------------------

uint32 EmRegs::GetLong(emuptr address) {
    //	EmAssert (this->ValidAddress (address, 4));

    long offset = address - this->GetAddressStart();
    ReadFunction fn = fReadFunctions[offset];
    EmAssert(fn);

    return (this->*fn)(address, 4);
}

// ---------------------------------------------------------------------------
//		� EmRegs::GetWord
// ---------------------------------------------------------------------------

uint32 EmRegs::GetWord(emuptr address) {
    //	EmAssert (this->ValidAddress (address, 2));

    long offset = address - this->GetAddressStart();
    ReadFunction fn = fReadFunctions[offset];
    EmAssert(fn);

    return (this->*fn)(address, 2);
}

// ---------------------------------------------------------------------------
//		� EmRegs::GetByte
// ---------------------------------------------------------------------------

uint32 EmRegs::GetByte(emuptr address) {
    //	EmAssert (this->ValidAddress (address, 1));

    long offset = address - this->GetAddressStart();
    ReadFunction fn = fReadFunctions[offset];
    EmAssert(fn);

    return (this->*fn)(address, 1);
}

// ---------------------------------------------------------------------------
//		� EmRegs::SetLong
// ---------------------------------------------------------------------------

void EmRegs::SetLong(emuptr address, uint32 value) {
    //	EmAssert (this->ValidAddress (address, 4));

    long offset = address - this->GetAddressStart();
    WriteFunction fn = fWriteFunctions[offset];
    EmAssert(fn);

    (this->*fn)(address, 4, value);
}

// ---------------------------------------------------------------------------
//		� EmRegs::SetWord
// ---------------------------------------------------------------------------

void EmRegs::SetWord(emuptr address, uint32 value) {
    //	EmAssert (this->ValidAddress (address, 2));

    long offset = address - this->GetAddressStart();
    WriteFunction fn = fWriteFunctions[offset];
    EmAssert(fn);

    (this->*fn)(address, 2, value);
}

// ---------------------------------------------------------------------------
//		� EmRegs::SetByte
// ---------------------------------------------------------------------------

void EmRegs::SetByte(emuptr address, uint32 value) {
    //	EmAssert (this->ValidAddress (address, 1));

    long offset = address - this->GetAddressStart();
    WriteFunction fn = fWriteFunctions[offset];
    EmAssert(fn);

    (this->*fn)(address, 1, value);
}

// ---------------------------------------------------------------------------
//		� EmRegs::ValidAddress
// ---------------------------------------------------------------------------

int EmRegs::ValidAddress(emuptr address, uint32 size) {
    UNUSED_PARAM(size);

    int result = false;
    unsigned long offset = address - this->GetAddressStart();

    if (offset < fReadFunctions.size()) {
        ReadFunction fn = fReadFunctions[offset];
        result = (fn != &EmRegs::UnsupportedRead);
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegs::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegs::GetRealAddress(emuptr address) {
    UNUSED_PARAM(address);

    // Sub-class's responsibility.

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmRegs::SetHandler
// ---------------------------------------------------------------------------

void EmRegs::SetHandler(ReadFunction read, WriteFunction write, uint32 start, int count) {
    if (fReadFunctions.size() == 0) {
        uint32 range = this->GetAddressRange();

        fReadFunctions.resize(range, &EmRegs::UnsupportedRead);
        fWriteFunctions.resize(range, &EmRegs::UnsupportedWrite);
    }

    int index = start - this->GetAddressStart();

    EmAssert(index >= 0);
    EmAssert(index < (long)fReadFunctions.size());

    for (int ii = 0; ii < count; ++ii, ++index) {
        fReadFunctions[index] = read;
        fWriteFunctions[index] = write;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegs::UnsupportedRead
// ---------------------------------------------------------------------------

uint32 EmRegs::UnsupportedRead(emuptr address, int size) { return ~0; }

// ---------------------------------------------------------------------------
//		� EmRegs::UnsupportedWrite
// ---------------------------------------------------------------------------

void EmRegs::UnsupportedWrite(emuptr address, int size, uint32 value) {}

// ---------------------------------------------------------------------------
//		� EmRegs::StdReadBE
// ---------------------------------------------------------------------------
// Read registers in a strict Big Endian fashion.

uint32 EmRegs::StdReadBE(emuptr address, int size) {
    uint8* realAddr = this->GetRealAddress(address);

    if (size == 1) return *(uint8*)realAddr;

    if (size == 2) {
        uint16 result16 = *(uint16*)realAddr;
        Canonical(result16);
        return result16;
    }

#if UNALIGNED_LONG_ACCESS

    uint32 result32 = *(uint32*)realAddr;
    Canonical(result32);
    return result32;

#else

    return (realAddr[0] << 24) | (realAddr[1] << 16) | (realAddr[2] << 8) | (realAddr[3] << 0);

#endif
}

// ---------------------------------------------------------------------------
//		� EmRegs::StdWriteBE
// ---------------------------------------------------------------------------
// Write registers in a strict Big Endian fashion.

void EmRegs::StdWriteBE(emuptr address, int size, uint32 value) {
    uint8* realAddr = this->GetRealAddress(address);

    if (size == 1) {
        *(uint8*)realAddr = value;
    }

    else if (size == 2) {
        uint16 value16 = value;
        Canonical(value16);
        *(uint16*)realAddr = value16;
    }

    else if (size == 4) {
#if UNALIGNED_LONG_ACCESS

        Canonical(value);
        *(uint32*)realAddr = value;

#else

        realAddr[0] = (uint8)(value >> 24);
        realAddr[1] = (uint8)(value >> 16);
        realAddr[2] = (uint8)(value >> 8);
        realAddr[3] = (uint8)(value >> 0);

#endif
    }
}

// ---------------------------------------------------------------------------
//		� EmRegs::StdRead
// ---------------------------------------------------------------------------
// Read registers using the routines in maccess.h

uint32 EmRegs::StdRead(emuptr address, int size) {
    uint8* realAddr = this->GetRealAddress(address);

    if (size == 1) return EmMemDoGet8(realAddr);

    if (size == 2) return EmMemDoGet16(realAddr);

    return EmMemDoGet32(realAddr);
}

// ---------------------------------------------------------------------------
//		� EmRegs::StdWrite
// ---------------------------------------------------------------------------
// Write registers using the routines in maccess.h

void EmRegs::StdWrite(emuptr address, int size, uint32 value) {
    uint8* realAddr = this->GetRealAddress(address);

    if (size == 1)
        EmMemDoPut8(realAddr, value);

    else if (size == 2)
        EmMemDoPut16(realAddr, value);

    else
        EmMemDoPut32(realAddr, value);
}

// ---------------------------------------------------------------------------
//		� EmRegs::ZeroRead
// ---------------------------------------------------------------------------

uint32 EmRegs::ZeroRead(emuptr address, int size) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    return 0;
}

// ---------------------------------------------------------------------------
//		� EmRegs::NullWrite
// ---------------------------------------------------------------------------

void EmRegs::NullWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)
    UNUSED_PARAM(value)

    // Do nothing (for read-only registers).
}
