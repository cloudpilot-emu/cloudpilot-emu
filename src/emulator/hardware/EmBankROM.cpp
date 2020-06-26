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

static EmAddressBank gROMAddressBank = {EmBankROM::GetLong,        EmBankROM::GetWord,
                                        EmBankROM::GetByte,        EmBankROM::SetLong,
                                        EmBankROM::SetWord,        EmBankROM::SetByte,
                                        EmBankROM::GetRealAddress, EmBankROM::ValidAddress,
                                        EmBankROM::GetMetaAddress, EmBankROM::AddOpcodeCycles};

static uint32 gROMBank_Size;
static uint32 gManagedROMSize;
static uint32 gROMImage_Size;
static uint32 gROMBank_Mask;
static uint8* gROM_Memory;
static uint8* gROM_MetaMemory;

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

void EmBankROM::Reset(Bool /*hardwareReset*/) { memset(gROM_MetaMemory, 0, gROMImage_Size); }

/***********************************************************************
 *
 * FUNCTION:	EmBankROM::Save
 *
 * DESCRIPTION: Standard save function.  Saves any sub-system state to
 *				the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankROM::Save(SessionFile& f) {
#if 0  // CSTODO
    EmAssert(gSession);
    Configuration cfg = gSession->GetConfiguration();
    f.WriteROMFileReference(cfg.fROMFile);

    StWordSwapper swapper1(gROM_MetaMemory, gROMImage_Size);
    f.WriteMetaROMImage(gROM_MetaMemory, gROMImage_Size);
#endif
}

/***********************************************************************
 *
 * FUNCTION:	EmBankROM::Load
 *
 * DESCRIPTION: Standard load function.  Loads any sub-system state
 *				from the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankROM::Load(SessionFile& f) {
#if 0  // CSTODO
    // ROM reference was read when the basic configuration was read
    // and passed to EmSession::Initialize, which then called
    // EmBankROM::Initialize, which then called EmBankROM::LoadROM.

    EmAssert(gROM_MetaMemory != NULL);

    if (f.ReadMetaROMImage(gROM_MetaMemory)) {
        ByteswapWords(gROM_MetaMemory, gROMImage_Size);
    } else {
        f.SetCanReload(false);
    }
#endif
}

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

void EmBankROM::Dispose(void) {
    Platform::DisposeMemory(gROM_Memory);
    Platform::DisposeMemory(gROM_MetaMemory);
}

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
    }
#endif

#if (VALIDATE_ROM_GET)
    if (gMemAccessFlags.fValidate_ROMGet && !ValidAddress(address, sizeof(uint32))) {
        InvalidAccess(address, sizeof(uint32), true);
    }
#endif

#if (PREVENT_USER_ROM_GET || PREVENT_SYSTEM_ROM_GET)
    if (EmMemory::IsPCInRAM()) {
        if (PREVENT_USER_ROM_GET && gMemAccessFlags.fProtect_ROMGet) {
            InvalidAccess(address, sizeof(uint32), true);
        }
    } else {
        if (PREVENT_SYSTEM_ROM_GET && gMemAccessFlags.fProtect_SysROMGet) {
            InvalidAccess(address, sizeof(uint32), true);
        }
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
    }
#endif

#if (VALIDATE_ROM_GET)
    if (gMemAccessFlags.fValidate_ROMGet && !ValidAddress(address, sizeof(uint16))) {
        InvalidAccess(address, sizeof(uint16), true);
    }
#endif

#if (PREVENT_USER_ROM_GET || PREVENT_SYSTEM_ROM_GET)
    if (EmMemory::IsPCInRAM()) {
        if (PREVENT_USER_ROM_GET && gMemAccessFlags.fProtect_ROMGet) {
            InvalidAccess(address, sizeof(uint16), true);
        }
    } else {
        if (PREVENT_SYSTEM_ROM_GET && gMemAccessFlags.fProtect_SysROMGet) {
            InvalidAccess(address, sizeof(uint16), true);
        }
    }
#endif

    address &= gROMBank_Mask;

    return EmMemDoGet16(gROM_Memory + address);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::GetByte
// ---------------------------------------------------------------------------

uint32 EmBankROM::GetByte(emuptr address) {
#if (VALIDATE_ROM_GET)
    if (gMemAccessFlags.fValidate_ROMGet && !ValidAddress(address, sizeof(uint8))) {
        InvalidAccess(address, sizeof(uint8), true);
    }
#endif

#if (PREVENT_USER_ROM_GET || PREVENT_SYSTEM_ROM_GET)
    if (EmMemory::IsPCInRAM()) {
        if (PREVENT_USER_ROM_GET && gMemAccessFlags.fProtect_ROMGet) {
            InvalidAccess(address, sizeof(uint8), true);
        }
    } else {
        if (PREVENT_SYSTEM_ROM_GET && gMemAccessFlags.fProtect_SysROMGet) {
            InvalidAccess(address, sizeof(uint8), true);
        }
    }
#endif

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

#if (PREVENT_USER_ROM_SET || PREVENT_SYSTEM_ROM_SET)
    if (EmMemory::IsPCInRAM()) {
        if (PREVENT_USER_ROM_SET && gMemAccessFlags.fProtect_ROMSet) {
            InvalidAccess(address, sizeof(uint32), false);
            return;
        }
    } else {
        if (PREVENT_SYSTEM_ROM_SET && gMemAccessFlags.fProtect_SysROMSet) {
            InvalidAccess(address, sizeof(uint32), false);
            return;
        }
    }
#endif

    address &= gROMBank_Mask;

    EmMemDoPut32(gROM_Memory + address, value);
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

#if (PREVENT_USER_ROM_SET || PREVENT_SYSTEM_ROM_SET)
    if (EmMemory::IsPCInRAM()) {
        if (PREVENT_USER_ROM_SET && gMemAccessFlags.fProtect_ROMSet) {
            InvalidAccess(address, sizeof(uint16), false);
            return;
        }
    } else {
        if (PREVENT_SYSTEM_ROM_SET && gMemAccessFlags.fProtect_SysROMSet) {
            InvalidAccess(address, sizeof(uint16), false);
            return;
        }
    }
#endif

    address &= gROMBank_Mask;

    EmMemDoPut16(gROM_Memory + address, value);
}

// ---------------------------------------------------------------------------
//		� EmBankROM::SetByte
// ---------------------------------------------------------------------------

void EmBankROM::SetByte(emuptr address, uint32 value) {
    EmAssert(ValidAddress(address, sizeof(uint8)));

#if (PREVENT_USER_ROM_SET || PREVENT_SYSTEM_ROM_SET)
    if (EmMemory::IsPCInRAM()) {
        if (PREVENT_USER_ROM_SET && gMemAccessFlags.fProtect_ROMSet) {
            InvalidAccess(address, sizeof(uint8), false);
            return;
        }
    } else {
        if (PREVENT_SYSTEM_ROM_SET && gMemAccessFlags.fProtect_SysROMSet) {
            InvalidAccess(address, sizeof(uint8), false);
            return;
        }
    }
#endif

    address &= gROMBank_Mask;

    EmMemDoPut8(gROM_Memory + address, value);
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
//		� EmBankROM::GetMetaAddress
// ---------------------------------------------------------------------------

uint8* EmBankROM::GetMetaAddress(emuptr address) {
    // Strip the uppermost bit of the address.

    address &= gROMBank_Mask;

    return (uint8*)&(gROM_MetaMemory[address]);
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

void EmBankROM::InvalidAccess(emuptr address, long size, Bool forRead) {
#if 0  // CSTODO
    EmAssert(gSession);
    gSession->ScheduleDeferredError(new EmDeferredErrROM(address, size, forRead));
#endif
}

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
    auto romMetaImage = make_unique<uint8[]>(gROMImage_Size);

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

            if (!gSession->GetDevice().PrismPlatinumEdgeHack()) {
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
    EmAssert(gROM_MetaMemory == NULL);

    gROM_Memory = (uint8*)romImage.release();
    gROM_MetaMemory = (uint8*)romMetaImage.release();
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

#pragma mark -

// ===========================================================================
//		� Flash Bank Accessors
// ===========================================================================
// These functions provide fetch and store access to the emulator's read only
// memory.

/*
        There are five types of flash that our flash manager routines identify:
        Mitsubishi, Hitachi, Intel, AMD, and Fujitsu.  Of these, only the last
        two are really supported right now.  Unfortunately, they are the hardest
        to emulate.  :-(

        To identify the kind of flash being used, our ROM routines:

                - Read a word from FLASHBASE
                - Write 0x00FF to FLASHBASE
                - Read a word from FLASHBASE
                - Write 0x0090 to FLASHBASE
                - Read a manufacturer ID (word) from FLASHBASE
                - Read a device ID (word) from FLASHBASE + 2
                - Write 0x00FF to FLASHBASE

        Mitsubishi: manufacturer == 0x001C, device == 0x005E
        Hitachi:	manufacturer == 0x0007, device == 0x0086
        Intel:		manufacturer == 0x0089, device == 0x0091

        If the flash is not one of those, our ROM routines:

                - Read a word from FLASHBASE
                - Write 0x00F0 to FLASHBASE
                - Write 0x00AA to FLASHBASE + 0xAAAA
                - Write 0x0055 to FLASHBASE + 0x5554
                - Write 0x0090 to FLASHBASE + 0xAAAA
                - Read a manufacturer ID (word) from FLASHBASE
                - Read a device ID (word) from FLASHBASE + 2
                - Write 0x00F0 to FLASHBASE

        AMD:		manufacturer == 0x0001, device == 0x0049
        Fujitsu:	manufacturer == 0x0004, device == 0x0049


        To erase a word of flash, our ROM routines:

                AMD, Fujitsu:

                - Read a word from FLASHBASE
                - Write 0x00F0 to FLASHBASE
                - Write 0x00AA to FLASHBASE + 0xAAAA
                - Write 0x0055 to FLASHBASE + 0x5554
                - Write 0x0080 to FLASHBASE + 0xAAAA
                - Write 0x00AA to FLASHBASE + 0xAAAA
                - Write 0x0055 to FLASHBASE + 0x5554
                - Write 0x0030 to location to be erased
                - Check erase location for 0x0080
                - Read from FLASHBASE
                - Write 0x00F0 to FLASHBASE

                Mitsubishi, Hitachi:

                - Read a word from FLASHBASE
                - Write 0x00FF to FLASHBASE
                - Write 0x0020 to FLASHBASE
                - Write 0x00D0 to location to be erased
                - Check erase location for 0x0080
                        -- If 0x0020 is also set, an error occurred
                - Read from FLASHBASE
                - Write 0x00FF to FLASHBASE

                Intel

                - Not supported

        To program a block of flash:

                AMD, Fujitsu:

                - Read a word from FLASHBASE
                - Write 0x00F0 to FLASHBASE
                - For each word to write
                        - If the word is already there, continue
                        - Write 0x00AA to FLASHBASE + 0xAAAA
                        - Write 0x0055 to FLASHBASE + 0x5554
                        - Write 0x00A0 to FLASHBASE + 0xAAAA
                        - Write the word to the dest location
                        - Check write location for 0x0080
                - Read from FLASHBASE
                - Write 0x00F0 to FLASHBASE

                Mitsubishi, Hitachi, Intel:

                - Not supported
*/

enum {
    kAMDState_Normal,  // Read-only mode. Acts like a normal ROM.
                       // Almost any write of 0x00F0 to any address will get us here.
                       // Also enter here automatically after Erase or Program operation.
                       // A write of 0x00AA to 0xAAAA will put us in kAMDState_Unlocked1.

    kAMDState_Unlocked1,  // After first unlock cycle (0x00AA written to 0xAAAA)
                          // Looking for the second unlock cycle (0x0055 written to 0x5554);
                          // If we find one, go to kAMDState_Unlocked2.
                          // ??? What happens on other operations?

    kAMDState_Unlocked2,  // After second unlock cycle (0x0055 written to 0x5554)
                          // Now looking for a command to be written to 0xAAAA
                          // If we find 0x0090, go to kAMDState_Autoselect.
                          // If we find 0x0080, set gEraseIsSetup and go to kAMDState_Normal. ???
                          // When should gEraseIsSetup get cleared? If we find 0x0030, if
                          // gEraseIsSetup erase the sector and go to kAMDState_EraseDone. If we
                          // fine 0x00A0, go to kAMDState_Program.
                          // ??? What happens on other operations?

    kAMDState_Autoselect,  // After a write of 0x0090 to 0x5555.
                           // A read of 0x0000 returns 0x0001 (manufacturer ID).
                           // A read of 0x0002 returns 0x0049 (device ID).
                           // A write of 0x00F0 to any address returns us to kAMDState_Normal.
                           // ??? What happens on other operations?

    kAMDState_Program,  // After a program sequence was entered.
                        // Accept any write operation and go to kAMDState_ProgramDone.
                        // ??? What happens on other operations?

    kAMDState_EraseDone,  // After a Erase operation.
                          // On the next read, return successful operation and return to
                          // kAMDState_Normal
                          // ??? What happens on other operations?

    kAMDState_ProgramDone,  // After a Program operation.
                            // On the next read, return successful operation and return to
                            // kAMDState_Normal
                            // ??? What happens on other operations?
    kAMDState_Dummy
};

static EmAddressBank gFlashAddressBank = {
    EmBankFlash::GetLong,        EmBankFlash::GetWord,      EmBankFlash::GetByte,
    EmBankFlash::SetLong,        EmBankFlash::SetWord,      EmBankFlash::SetByte,
    EmBankFlash::GetRealAddress, EmBankFlash::ValidAddress, EmBankFlash::GetMetaAddress,
    EmBankFlash::AddOpcodeCycles};

#define FLASHBASE (EmBankROM::GetMemoryStart())

static int gState = kAMDState_Normal;
static Bool gEraseIsSetup;

/***********************************************************************
 *
 * FUNCTION:	EmBankFlash::Initialize
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

void EmBankFlash::Initialize(void) {}

/***********************************************************************
 *
 * FUNCTION:	EmBankFlash::Reset
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

void EmBankFlash::Reset(Bool hardwareReset) {
    if (hardwareReset) {
        gState = kAMDState_Normal;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmBankFlash::Save
 *
 * DESCRIPTION: Standard save function.  Saves any sub-system state to
 *				the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankFlash::Save(SessionFile&) {
    // !!! Save gState && gEraseIsSetup?
}

/***********************************************************************
 *
 * FUNCTION:	EmBankFlash::Load
 *
 * DESCRIPTION: Standard load function.  Loads any sub-system state
 *				from the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmBankFlash::Load(SessionFile&) {
    // !!! Load gState && gEraseIsSetup?
}

/***********************************************************************
 *
 * FUNCTION:	EmBankFlash::Dispose
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

void EmBankFlash::Dispose(void) {}

/***********************************************************************
 *
 * FUNCTION:    EmBankFlash::SetBankHandlers
 *
 * DESCRIPTION: Set the bank handlers UAE uses to dispatch memory
 *				access operations.
 *
 * PARAMETERS:  None
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void EmBankFlash::SetBankHandlers(void) {
    gManagedROMSize = EmHAL::GetROMSize();

    // Memory banks gROMMemoryStart to <mumble> are managed by the functions
    // in EmBankROM.	<mumble> is based on the amount of ROM being emulated.

    uint32 first_bank = EmMemBankIndex(EmBankROM::GetMemoryStart());
    uint32 last_bank = EmMemBankIndex(EmBankROM::GetMemoryStart() + gManagedROMSize - 1);

    Memory::InitializeBanks(gFlashAddressBank, first_bank, last_bank - first_bank + 1);
}

// ---------------------------------------------------------------------------
//		� EmBankFlash::GetLong
// ---------------------------------------------------------------------------

uint32 EmBankFlash::GetLong(emuptr address) { return EmBankROM::GetLong(address); }

// ---------------------------------------------------------------------------
//		� EmBankFlash::GetWord
// ---------------------------------------------------------------------------

uint32 EmBankFlash::GetWord(emuptr address) {
    switch (gState) {
        case kAMDState_Normal:
            // Read-only mode. Acts like a normal ROM.
            // Almost any write of 0x00F0 to any address will get us here.
            // Also enter here automatically after Erase or Program operation.
            // A write of 0x00AA to 0xAAAA will put us in kAMDState_Unlocked1.

            break;

        case kAMDState_Unlocked1:
            // After first unlock cycle (0x00AA written to 0xAAAA)
            // Looking for the second unlock cycle (0x0055 written to 0x5554);
            // If we find one, go to kAMDState_Unlocked2.
            // ??? What happens on other operations?

            break;

        case kAMDState_Unlocked2:
            // After second unlock cycle (0x0055 written to 0x5554)
            // Now looking for a command to be written to 0xAAAA
            // If we find 0x0090, go to kAMDState_Autoselect.
            // If we find 0x0080, set gEraseIsSetup and go to kAMDState_Normal. ??? When should
            // gEraseIsSetup get cleared? If we find 0x0030, if gEraseIsSetup erase the sector and
            // go to kAMDState_ProgramDone. If we fine 0x00A0, go to kAMDState_Program.
            // ??? What happens on other operations?

            break;

        case kAMDState_Autoselect:
            // After a write of 0x0090 to 0x5555.
            // A read of 0x0000 returns 0x0001 (manufacturer ID).
            // A read of 0x0002 returns 0x0049 (device ID).
            // A write of 0x00F0 to any address returns us to kAMDState_Normal.
            // ??? What happens on other operations?

            if (address == FLASHBASE) {
                return 0x0001;
            } else if (address == FLASHBASE + 2) {
                return 0x0049;
            }
            break;

        case kAMDState_Program:
            // After a program sequence was entered.
            // Accept any write operation and go to kAMDState_ProgramDone.
            // ??? What happens on other operations?

            break;

        case kAMDState_EraseDone:
            // After a Program or Erase operation.
            // On the next read, return successful operation and return to kAMDState_Normal
            // ??? What happens on other operations?

            // !!! We should really check that address is the same as the
            // one specified in the Erase command.

            gState = kAMDState_Normal;
            return 0x0080;

        case kAMDState_ProgramDone:
            // After a Program or Erase operation.
            // On the next read, return successful operation and return to kAMDState_Normal
            // ??? What happens on other operations?

            // !!! We should really check that address is the same as the
            // one specified in the Program command.

            gState = kAMDState_Normal;
            return 0x0080 & EmBankROM::GetWord(address);
    }

    return EmBankROM::GetWord(address);
}

// ---------------------------------------------------------------------------
//		� EmBankFlash::GetByte
// ---------------------------------------------------------------------------

uint32 EmBankFlash::GetByte(emuptr address) { return EmBankROM::GetByte(address); }

// ---------------------------------------------------------------------------
//		� EmBankFlash::SetLong
// ---------------------------------------------------------------------------

void EmBankFlash::SetLong(emuptr address, uint32 value) { EmBankROM::SetLong(address, value); }

// ---------------------------------------------------------------------------
//		� EmBankFlash::SetWord
// ---------------------------------------------------------------------------

void EmBankFlash::SetWord(emuptr address, uint32 value) {
    switch (gState) {
        case kAMDState_Normal:
            // Read-only mode. Acts like a normal ROM.
            // Almost any write of 0x00F0 to any address will get us here.
            // Also enter here automatically after Erase or Program operation.
            // A write of 0x00AA to 0xAAAA will put us in kAMDState_Unlocked1.

            if (address == FLASHBASE + 0xAAAA && value == 0x00AA) {
                gState = kAMDState_Unlocked1;
                return;
            }

            // Allow these.  PrvIdentifyFlashCode probes with these values
            // to determine what Flash part we've got.

            if (address == FLASHBASE && (value == 0x00FF || value == 0x00F0 || value == 0x0090)) {
                return;
            }
            break;

        case kAMDState_Unlocked1:
            // After first unlock cycle (0x00AA written to 0xAAAA)
            // Looking for the second unlock cycle (0x0055 written to 0x5554);
            // If we find one, go to kAMDState_Unlocked2.
            // ??? What happens on other operations?

            if (value == 0x00F0) {
                gState = kAMDState_Normal;
                return;
            } else if (address == FLASHBASE + 0x5554 && value == 0x0055) {
                gState = kAMDState_Unlocked2;
                return;
            }
            break;

        case kAMDState_Unlocked2:
            // After second unlock cycle (0x0055 written to 0x5554)
            // Now looking for a command to be written to 0xAAAA
            // If we find 0x0090, go to kAMDState_Autoselect.
            // If we find 0x0080, set gEraseIsSetup and go to kAMDState_Normal. ??? When should
            // gEraseIsSetup get cleared? If we find 0x0030, if gEraseIsSetup erase the sector and
            // go to kAMDState_EraseDone. If we fine 0x00A0, go to kAMDState_Program.
            // ??? What happens on other operations?

            if (value == 0x00F0) {
                gState = kAMDState_Normal;
                return;
            } else if (value == 0x0030 && gEraseIsSetup) {
                const int kEraseValue = 0xFF;
#if 0
				DWord	readWriteParmsOffset	= EmMemGet32 (0x10C00000 + offsetof (CardHeaderType, readWriteParmsOffset));
				DWord	readWriteParmsSize		= EmMemGet32 (0x10C00000 + offsetof (CardHeaderType, readWriteParmsSize));
				DWord	readOnlyParmsOffset 	= EmMemGet32 (0x10C00000 + offsetof (CardHeaderType, readOnlyParmsOffset));
				DWord	readOnlyParmsSize		= 0x00002000;
				DWord	readWriteWorkingOffset	= EmMemGet32 (0x10C00000 + offsetof (CardHeaderType, readWriteWorkingOffset));
				DWord	readWriteWorkingSize	= EmMemGet32 (0x10C00000 + offsetof (CardHeaderType, readWriteWorkingSize));

				if (address >= 0x10000000 + readWriteParmsOffset &&
					address < 0x10000000 + readWriteParmsOffset + readWriteParmsSize)
				{
					EmMem_memset (0x10000000 + readWriteParmsOffset, kEraseValue, readWriteParmsSize);
				}
				else if (address >= 0x10000000 + readOnlyParmsOffset &&
					address < 0x10000000 + readOnlyParmsOffset + readOnlyParmsSize)
				{
					EmMem_memset (0x10000000 + readOnlyParmsOffset, kEraseValue, readOnlyParmsSize);
				}
				else if (address >= 0x10000000 + readWriteWorkingOffset &&
					address < 0x10000000 + readWriteWorkingOffset + readWriteWorkingSize)
				{
					EmMem_memset (0x10000000 + readWriteWorkingOffset, kEraseValue, readWriteWorkingSize);
				}
#endif

                const unsigned long kSector1Start = 0x10C00000;
                const unsigned long kSector1Size = 0x00004000;
                const unsigned long kSector2Start = 0x10C04000;
                const unsigned long kSector2Size = 0x00002000;
                const unsigned long kSector3Start = 0x10C06000;
                const unsigned long kSector3Size = 0x00002000;
                const unsigned long kSector4Start = 0x10C08000;
                const unsigned long kSector4Size = 0x00008000;

                CEnableFullAccess munge;

                if (address == kSector1Start) {
                    EmMem_memset(kSector1Start, kEraseValue, kSector1Size);
                } else if (address == kSector2Start) {
                    EmMem_memset(kSector2Start, kEraseValue, kSector2Size);
                } else if (address == kSector3Start) {
                    EmMem_memset(kSector3Start, kEraseValue, kSector3Size);
                } else if (address == kSector4Start) {
                    EmMem_memset(kSector4Start, kEraseValue, kSector4Size);
                }

                gState = kAMDState_EraseDone;
                return;
            } else if (address == FLASHBASE + 0xAAAA) {
                if (value == 0x0090) {
                    gState = kAMDState_Autoselect;
                    return;
                } else if (value == 0x0080) {
                    gEraseIsSetup = true;
                    gState = kAMDState_Normal;
                    return;
                } else if (value == 0x00A0) {
                    gState = kAMDState_Program;
                    return;
                }
            }
            break;

        case kAMDState_Autoselect:
            // After a write of 0x0090 to 0x5555.
            // A read of 0x0000 returns 0x0001 (manufacturer ID).
            // A read of 0x0002 returns 0x0049 (device ID).
            // A write of 0x00F0 to any address returns us to kAMDState_Normal.
            // ??? What happens on other operations?

            if (value == 0x00F0) {
                gState = kAMDState_Normal;
                return;
            }
            break;

        case kAMDState_Program:
            // After a program sequence was entered.
            // Accept any write operation and go to kAMDState_ProgramDone.
            // ??? What happens on other operations?

            address &= gROMBank_Mask;
            EmMemDoPut16(gROM_Memory + address, value);

            gState = kAMDState_ProgramDone;
            return;

        case kAMDState_EraseDone:
            // After a Program or Erase operation.
            // On the next read, return successful operation and return to kAMDState_Normal
            // ??? What happens on other operations?

            return;

        case kAMDState_ProgramDone:
            // After a Program or Erase operation.
            // On the next read, return successful operation and return to kAMDState_Normal
            // ??? What happens on other operations?

            return;
    }

    EmBankROM::SetWord(address, value);
}

// ---------------------------------------------------------------------------
//		� EmBankFlash::SetByte
// ---------------------------------------------------------------------------

void EmBankFlash::SetByte(emuptr address, uint32 value) { EmBankROM::SetByte(address, value); }

// ---------------------------------------------------------------------------
//		� EmBankFlash::ValidAddress
// ---------------------------------------------------------------------------

int EmBankFlash::ValidAddress(emuptr address, uint32 size) {
    return EmBankROM::ValidAddress(address, size);
}

// ---------------------------------------------------------------------------
//		� EmBankFlash::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmBankFlash::GetRealAddress(emuptr address) {
    // Strip the uppermost bit of the address.

    address &= gROMBank_Mask;

    return (uint8*)&gROM_Memory[address];
}

// ---------------------------------------------------------------------------
//		� EmBankFlash::GetMetaAddress
// ---------------------------------------------------------------------------

uint8* EmBankFlash::GetMetaAddress(emuptr address) {
    // Strip the uppermost bit of the address.

    address &= gROMBank_Mask;

    return (uint8*)&(gROM_MetaMemory[address]);
}

// ---------------------------------------------------------------------------
//		� EmBankFlash::AddOpcodeCycles
// ---------------------------------------------------------------------------

void EmBankFlash::AddOpcodeCycles(void) {
#if (HAS_PROFILING)
    CYCLE_GETWORD(WAITSTATES_ROM);
#endif
}

#pragma mark -

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
    Bool szMode = false;

    if (cardHdr.hdrVersion >= 3 && (cardHdr.flags & memCardHeaderFlagSZ)) {
        szMode = true;
    }

    return szMode;
}
