/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmBankROM.h"

#include <memory>

#include "Byteswapping.h"  // ByteswapWords
#include "EmCPU68K.h"      // gCPU68K
#include "EmCommon.h"
#include "EmHAL.h"          // EmHAL
#include "EmMemory.h"       // Memory::InitializeBanks, EmMem_memset
#include "EmPalmStructs.h"  // EmProxyCardHeaderType
#include "EmSession.h"      // GetDevice, ScheduleDeferredError
#include "Miscellaneous.h"  // StWordSwapper, NextPowerOf2
#include "Platform.h"

// Private function declarations

class Card {
   public:
    static Bool CheckCardHeader(const EmProxyCardHeaderType& cardHdr);
    static Bool CheckChecksum(const void* p, uint32 romLength);
    static Bool Supports328(const EmAliasCardHeaderType<LAS>& cardHdr);
    static Bool SupportsEZ(const EmAliasCardHeaderType<LAS>& cardHdr);
    static Bool SupportsVZ(const EmAliasCardHeaderType<LAS>& cardHdr);
    static Bool SupportsSZ(const EmAliasCardHeaderType<LAS>& cardHdr);
};

// static member initialization

emuptr EmBankROM::gROMMemoryStart = kDefaultROMMemoryStart;

// ===========================================================================
//		� ROM Bank Accessors
// ===========================================================================
// These functions provide fetch and store access to the emulator's read only
// memory.

static EmAddressBank gROMAddressBank = {
    EmBankROM::GetLong,        EmBankROM::GetWord,      EmBankROM::GetByte,
    EmBankROM::SetLong,        EmBankROM::SetWord,      EmBankROM::SetByte,
    EmBankROM::GetRealAddress, EmBankROM::ValidAddress, nullptr,
    EmBankROM::AddOpcodeCycles};

static uint32 gROMBank_Size;
static uint32 gManagedROMSize;
static uint32 gROMImage_Size;
static uint32 gROMBank_Mask;
static uint8* gROM_Memory;

/***********************************************************************
 *
 * FUNCTION:	EmBankROM::Initialize
 *
 * DESCRIPTION: Standard initialization function.  Responsible for
 *				initializing this sub-system when a new session is
 *				created.  Will be followed by at least one call to
 *				Reset or Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

bool EmBankROM::Initialize(size_t len, const uint8* buffer) {
    // When creating a new session, Initialize is called followed by
    // Reset.  When loading a session, Initialize is called by Load.
    // So it makes sense to load the ROM image here.  However, when
    // reloading a session as part of a Gremlin Horde, only Load is
    // called, so it is critical that all parts of a horde use the
    // same ROM image.

    return EmBankROM::LoadROM(len, buffer);
}

/***********************************************************************
 *
 * FUNCTION:	EmBankROM::Reset
 *
 * DESCRIPTION: Standard reset function.  Sets the sub-system to a
 *				default state.	This occurs not only on a Reset (as
 *				from the menu item), but also when the sub-system
 *				is first initialized (Reset is called after Initialize)
 *				as well as when the system is re-loaded from an
 *				insufficient session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankROM::Reset(Bool /*hardwareReset*/) {}

/***********************************************************************
 *
 * FUNCTION:	EmBankROM::Dispose
 *
 * DESCRIPTION: Standard dispose function.	Completely release any
 *				resources acquired or allocated in Initialize and/or
 *				Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankROM::Dispose(void) { Platform::DisposeMemory(gROM_Memory); }

/***********************************************************************
 *
 * FUNCTION:    EmBankROM::SetBankHandlers
 *
 * DESCRIPTION: Set the bank handlers UAE uses to dispatch memory
 *				access operations.
 *
 * PARAMETERS:  None
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void EmBankROM::SetBankHandlers(void) {
    if (EmHAL::ChipSelectsConfigured()) {
        gROMMemoryStart = EmHAL::GetROMBaseAddress();
    }

    gManagedROMSize = EmHAL::GetROMSize();

    // Memory banks gROMMemoryStart to <mumble> are managed by the functions
    // in EmBankROM.	<mumble> is based on the amount of ROM being emulated.

    uint32 first_bank = EmMemBankIndex(EmBankROM::GetMemoryStart());
    uint32 last_bank = EmMemBankIndex(EmBankROM::GetMemoryStart() + gManagedROMSize - 1);

    Memory::InitializeBanks(gROMAddressBank, first_bank, last_bank - first_bank + 1);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::GetLong
// ---------------------------------------------------------------------------

uint32 EmBankROM::GetLong(emuptr address) {
#if (CHECK_FOR_ADDRESS_ERROR)
    if ((address & 1) != 0) {
        AddressError(address, sizeof(uint32), true);
        address &= ~1;
    }
#endif

    address &= gROMBank_Mask;

    return EmMemDoGet32(gROM_Memory + address);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::GetWord
// ---------------------------------------------------------------------------

uint32 EmBankROM::GetWord(emuptr address) {
#if (CHECK_FOR_ADDRESS_ERROR)
    if ((address & 1) != 0) {
        AddressError(address, sizeof(uint16), true);
        address &= ~1;
    }
#endif

    address &= gROMBank_Mask;

    return EmMemDoGet16(gROM_Memory + address);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::GetByte
// ---------------------------------------------------------------------------

uint32 EmBankROM::GetByte(emuptr address) {
    address &= gROMBank_Mask;

    return EmMemDoGet8(gROM_Memory + address);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::SetLong
// ---------------------------------------------------------------------------

void EmBankROM::SetLong(emuptr address, uint32 value) {
#if (CHECK_FOR_ADDRESS_ERROR)
    if ((address & 1) != 0) {
        AddressError(address, sizeof(uint32), false);
    }
#endif

    EmAssert(ValidAddress(address, sizeof(uint32)));

    InvalidAccess(address, sizeof(uint32), false);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::SetWord
// ---------------------------------------------------------------------------

void EmBankROM::SetWord(emuptr address, uint32 value) {
#if (CHECK_FOR_ADDRESS_ERROR)
    if ((address & 1) != 0) {
        AddressError(address, sizeof(uint16), false);
    }
#endif

    EmAssert(ValidAddress(address, sizeof(uint16)));

    InvalidAccess(address, sizeof(uint16), false);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::SetByte
// ---------------------------------------------------------------------------

void EmBankROM::SetByte(emuptr address, uint32 value) {
    EmAssert(ValidAddress(address, sizeof(uint8)));

    InvalidAccess(address, sizeof(uint8), false);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::ValidAddress
// ---------------------------------------------------------------------------

int EmBankROM::ValidAddress(emuptr address, uint32 size) {
    address -= gROMMemoryStart;
    int result = (address + size) <= (gROMMemoryStart + gROMBank_Size);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmBankROM::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmBankROM::GetRealAddress(emuptr address) {
    // Strip the uppermost bit of the address.

    address &= gROMBank_Mask;

    return (uint8*)&gROM_Memory[address];
}

// ---------------------------------------------------------------------------
//		� EmBankROM::AddOpcodeCycles
// ---------------------------------------------------------------------------

void EmBankROM::AddOpcodeCycles(void) {}

// ---------------------------------------------------------------------------
//		� EmBankROM::AddressError
// ---------------------------------------------------------------------------

void EmBankROM::AddressError(emuptr address, long size, Bool forRead) {
    EmAssert(gCPU68K);
    gCPU68K->AddressError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::InvalidAccess
// ---------------------------------------------------------------------------

void EmBankROM::InvalidAccess(emuptr address, long size, Bool forRead) {}

/***********************************************************************
 *
 * FUNCTION:	EmBankROM::LoadROM
 *
 * DESCRIPTION: Does the work of actually loading (and validating a ROM
 *				from a stream.
 *
 * PARAMETERS:	EmStream hROMs
 *
 * RETURNED:	Nothing
 *
 ***********************************************************************/

bool EmBankROM::LoadROM(size_t len, const uint8* buffer) {
    // Make sure the file is big enough to have a card header.

    if (len < EmProxyCardHeaderType::GetSize()) return false;

    // Read the card header.

    EmProxyCardHeaderType cardHeader;
    memcpy(cardHeader.GetPtr(), buffer, cardHeader.GetSize());

    // Validate the card header.

    if (!Card::CheckCardHeader(cardHeader)) return false;

    // The ROM is made up of two parts: a Big ROM and a Small ROM.	ROM image
    // files can contain both parts or just the Big ROM part.  If it contains
    // both parts, then everything is just fine, and we can use it no problem.
    // If the Small ROM part is missing, we have to detect that, dummy up a
    // small ROM for correct operation, and relocate the Big ROM higher in memory.

    // Both the Big ROM and Small ROM start off with a CardHeaderType struct.
    // We just read in one of them.  Let's look at where the Big ROM _should_
    // be to see if there's another CardHeader there.  If so, we have both parts.
    // If not, we have just the Big ROM.

    int32 bigROMOffset;

    if (cardHeader.hdrVersion == 1) {
        bigROMOffset = 0x3000;
    } else {
        // Version 2 CardHeaders contain a "bigROMOffset" field that contains the
        // offset from the beginning of the card to where the ROM should
        // appear in memory.  Normally the card appears at 0x10000000 (which
        // would make bigROMOffset something like 0x00C08000), but that
        // isn't always the case.  The problem is, we don't always know
        // where the card beginning will be.  For now, we'll assume that
        // the ROM wants to live at 0x10C00000, and that the Big ROM will
        // want to begin somewhere soon after that.  This means we can get by
        // with using only the lower bits of bigROMOffset.

        bigROMOffset = cardHeader.bigROMOffset;
        bigROMOffset &= 0x000FFFFF;  // Allows for 1 Meg offset.
    }

    // This is a hack to work around a broken card header in the 330c small ROM.
    // It would be nicer to handle this generically and scan for the big ROM header,
    // but there is only one known 330c ROM in existence, so...
    if (gSession->GetDevice().GetIDString() == "HandEra330c") bigROMOffset = 0x10000;

    // Make sure the file is big enough to have a Big ROM.

    if (len < (size_t)bigROMOffset) return false;

    // We'll use bigROMOffset for later to get to some Big ROM data.
    // But we'll also need a value when we read the ROM image into
    // memory.	This value will be used as an offset into a buffer
    // where we'll be reading the ROM image.  If we have a Small ROM,
    // the offset will be zero.  If we have only a Big ROM, the offset
    // will be the same value as bigROMOffset.

    uint32 bufferOffset = bigROMOffset;

    // See if there's a card header there, too.

    EmProxyCardHeaderType cardHeader2;
    memcpy(cardHeader2.GetPtr(), buffer + bigROMOffset, cardHeader2.GetSize());

    if (Card::CheckCardHeader(cardHeader2)) {
        // Looks like, so we don't have to relocate the ROM image.

        bufferOffset = 0;
    }

    // The ROM size is now the size of the file plus any offset for
    // any small ROM that we have to add and dummy up.

    gROMImage_Size = len + bufferOffset;
    gROMBank_Size = ::NextPowerOf2(gROMImage_Size);

    // Read in the ROM image.

    auto romImage = make_unique<uint8[]>(gROMImage_Size);

    memcpy(romImage.get() + bufferOffset, buffer, len);

    // See if the big ROM checksum looks OK.

    Card::CheckChecksum(romImage.get() + bigROMOffset, gROMImage_Size - bigROMOffset);

    // If we only had a Big ROM, dummy up the Small ROM.  All we really
    // need to do here is copy the Big ROM's card header to the Small
    // ROM area.
    //
    // Also, clear out this area to 0xFF to look like new Flash RAM.

    if (bufferOffset) {
        memset(romImage.get(), 0xFF, bigROMOffset);
        memcpy(romImage.get(), romImage.get() + bigROMOffset, EmProxyCardHeaderType::GetSize());
    } else {
        // See if the small ROM checksum looks OK.
        // Note that checksumBytes is invalid for v1 card headers,
        // but in those cases, it's not really used anyway.

        EmAliasCardHeaderType<LAS> cardHdr(romImage.get());
        uint32 smallROMSize = cardHdr.checksumBytes;
        Card::CheckChecksum(romImage.get(), smallROMSize);
    }

    // Check that the ROM we just loaded can be run on this device.

    EmAliasCardHeaderType<LAS> cardHdr(romImage.get());

    EmAssert(gSession);
    if (Card::SupportsEZ(cardHdr)) {
        if (!gSession->GetDevice().Supports68EZ328()) {
            /*	Make a hack for the Prism, Platinum and Edge below since
                    Handspring seems to report the EZ bit in their ROMs. */

            if (!gSession->GetDevice().HasBogusEZFlag()) {
                return false;
            }
        }
    } else if (Card::SupportsVZ(cardHdr)) {
        if (!gSession->GetDevice().Supports68VZ328()) {
            return false;
        }
    } else if (Card::SupportsSZ(cardHdr)) {
        if (!gSession->GetDevice().Supports68SZ328()) {
            return false;
        }
    } else {
        if (!gSession->GetDevice().Supports68328()) {
            return false;
        }
    }

    // Byteswap all the words in the ROM (if necessary). Most accesses
    // are 16-bit accesses, so we optimize for that case.

    ByteswapWords(romImage.get(), gROMImage_Size);

    // Everything seems to be OK.  Save the ROM data in some global
    // variables for the CPU emulator to access.  Make sure that
    // gROMBank_Size is a power-of-2.  The EmBankROM memory routines
    // require this.

    EmAssert(gROM_Memory == NULL);

    gROM_Memory = (uint8*)romImage.release();
    gROMBank_Mask = gROMBank_Size - 1;

    // Guess the default ROM base address.
    // This will be used until the chip selects are set up.
    //
    // (I tried just setting this to zero, but the Palm OS 3.0
    // startup code probes at 0x10c00008 and 0x10d00008 to see if
    // it can find signatures there.  If we map the ROM to zero,
    // then we'll get bus errors when those accesses are made.)

    gROMMemoryStart = cardHeader.resetVector & 0xFFF00000;

    return true;
}

uint32 EmBankROM::GetRomSize() { return gManagedROMSize; }

// ===========================================================================
//		� Card
// ===========================================================================

// ---------------------------------------------------------------------------
//		� Card::CheckCardHeader
// ---------------------------------------------------------------------------
// Determines if the memory pointed to looks like a card header.

Bool Card::CheckCardHeader(const EmProxyCardHeaderType& cardHdr) {
    // Make sure the stack size isn't something ludicrous.

    if (cardHdr.initStack == 0 || cardHdr.initStack > 0x000FFFFF)  // 1 Meg
        return false;

    // Make sure the signature is right.

    if (cardHdr.signature != sysCardSignature) return false;

    // Make sure the header version isn't ludicrous.

    if (cardHdr.hdrVersion == 0 || cardHdr.hdrVersion > 255)  // arbitrary value
        return false;

    // Make sure this isn't a "RAM only" card.

    if (cardHdr.flags & memCardHeaderFlagRAMOnly) return false;

    return true;
}

// ---------------------------------------------------------------------------
//		� Card::CheckChecksum
// ---------------------------------------------------------------------------
// Determines if the memory pointed to looks like a card header.

Bool Card::CheckChecksum(const void* p, uint32 fileLength) {
    EmAliasCardHeaderType<LAS> cardHdr((void*)p);

    if (cardHdr.hdrVersion == 1) {
        static const UInt16 kDebug20 = 0x9BED;

        UInt16 checksumValue = Crc16CalcBigBlock((/*non-const*/ void*)p, fileLength, 0);

        // Special hack for 2.0 Debug ROMs.  The version with this checksum was
        // bogus, so let's make sure we never load it.

        if (checksumValue == kDebug20) {
            // Throw a special error for this one.

            EmAssert(false);
        }

        return true;
    } else {
        // The checksum is the cumulative checksum of the ROM image before
        // the stored checksum value and the ROM image following the checksum
        // value.  First, calculate the first part.

        UInt32 chunkSize = cardHdr.offsetof_checksumValue();
        UInt16 checksumValue = Crc16CalcBigBlock((/*non-const*/ void*)p, chunkSize, 0);

        // Now calculate the second part.

        UInt32 skipSize = chunkSize + cardHdr.checksumValue.GetSize();
        checksumValue = Crc16CalcBigBlock(((char*)p) + skipSize, cardHdr.checksumBytes - skipSize,
                                          checksumValue);

        if (checksumValue == cardHdr.checksumValue) {
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
//		� Card::Supports328
// ---------------------------------------------------------------------------

Bool Card::Supports328(const EmAliasCardHeaderType<LAS>& cardHdr) {
    Bool dbMode = false;

    if (cardHdr.hdrVersion < 3 || (cardHdr.flags & memCardHeaderFlag328)) {
        dbMode = true;
    }

    return dbMode;
}

// ---------------------------------------------------------------------------
//		� Card::SupportsEZ
// ---------------------------------------------------------------------------

Bool Card::SupportsEZ(const EmAliasCardHeaderType<LAS>& cardHdr) {
    if (gSession->GetDevice().Supports68SZ328()) return false;

    Bool ezMode = false;

    if (cardHdr.hdrVersion >= 3 && (cardHdr.flags & memCardHeaderFlagEZ)) {
        ezMode = true;
    }

    return ezMode;
}

// ---------------------------------------------------------------------------
//		� Card::SupportsVZ
// ---------------------------------------------------------------------------
#define memCardHeaderFlagVZ 0x0040  // ROM Supports 68VZ328 processor

Bool Card::SupportsVZ(const EmAliasCardHeaderType<LAS>& cardHdr) {
    if (gSession->GetDevice().Supports68SZ328()) return false;

    Bool vzMode = false;

    if (cardHdr.hdrVersion >= 3 && (cardHdr.flags & memCardHeaderFlagVZ)) {
        vzMode = true;
    }

    return vzMode;
}

// ---------------------------------------------------------------------------
//		� Card::SupportsSZ
// ---------------------------------------------------------------------------
#define memCardHeaderFlagSZ 0x0080  // ROM Supports 68SZ328 processor

Bool Card::SupportsSZ(const EmAliasCardHeaderType<LAS>& cardHdr) {
    if (gSession->GetDevice().Supports68SZ328()) return true;

    Bool szMode = false;

    if (cardHdr.hdrVersion >= 3 && (cardHdr.flags & memCardHeaderFlagSZ)) {
        szMode = true;
    }

    return szMode;
}
