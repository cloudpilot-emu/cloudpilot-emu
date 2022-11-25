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

#include "EmTRGCFMem.h"

#include "EmCommon.h"

// ------------------------------------------------------------------------
// This file emulates the Registers on the CF Memory Card aside from the
// ATA registers (they are handled in a different source file).
//
// Address                   Description            Class
// ---------------------     --------------------   ---------------------
// 0x18000000-0x180001FF     Card Tuple             EmRegsCFTuple
// 0x18000200-0x18000FFF     Configuration Regs     EmRegsCFConfig
// 0x18001000-0x18001FFF     ATA Registers
// 0x18002000                CF Reset               EmRegsCFReset
// ------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Configuration Option Register (Base + 00h in Attribute Memory)
//
// The Configuration Option Register is used to configure the cards interface,
// address decoding and interrupt and to issue a soft reset to the CompactFlash
// Storage Card or CF+ Card.
//
//         D7     D6     D5    D4    D3    D2    D1    D0
//  R/W SRESET LevlREQ Conf5 Conf4 Conf3 Conf2 Conf1 Conf0
//
// SRESET - Soft Reset: setting this bit to one (1), waiting the minimum reset
// width time and returning to zero (0) places the CompactFlash Storage Card
// or CF+ Card in the Reset state.  Setting this bit to one (1) is equivalent
// to assertion of the +RESET signal except that the SRESET bit is not cleared.
// Returning this bit to zero (0) leaves the CompactFlash Storage
// Card or CF+ Card in the same un-configured, Reset state as following
// power-up and hardware reset. This bit is set to zero (0) by power-up and
// hardware reset. For CompactFlash Storage Cards, using the PCMCIA Soft
// Reset is considered a hard Reset by the ATA Commands. Contrast with Soft
// Reset in the Device Control Register.
//
// LevlREQ: this bit is set to one (1) when Level Mode Interrupt is selected,
// and zero (0) when Pulse Mode is selected. Set to zero (0) by Reset.
//
// Conf5 - Conf0 - Configuration Index: set to zero (0) by reset. It is used
// to select operation mode of the CompactFlash Storage Card or CF+ Card
// as shown below.
///  Note: Conf5 and Conf4 are reserved for CompactFlash Storage cards and
//   must be written as zero (0) These bits are vendor defined for CF+ Cards.
//
// CompactFlash Storage Card Configurations
//  Conf5 Conf4 Conf3 Conf2 Conf1 Conf0   Disk Card Mode
//     0     0     0     0     0     0    Memory Mapped
//     0     0     0     0     0     1    I/O Mapped, Any 16 byte system
//                                        decoded boundary
//     0     0     0     0     1     0    I/O Mapped, 1F0h-1F7h/3F6h-3F7h
//     0     0     0     0     1     1    I/O Mapped, 170h-177h/376h-377h
//
// CF+ Card Configurations
//  Conf5 Conf4 Conf3 Conf2 Conf1 Conf0   CF+ Card Mode
//     0     0     0     0     0     0    Memory Mapped, I/O cycles are ignored
//     X     X     X     X     X     X    Any non-zero value, vendor defined
//
// On Multiple Function CF+ Cards, bits in this field enable the following
// functionality:
//
// Bit 0 Enable Function - If this bit is reset to zero (0), the function is
// disabled. If this bit is set to one (1), the function is enabled.
//
// Bit 1 Enable Base and Limit Registers - If this bit is reset to zero (0)
// and Bit 0 is set to one (1), all I/O addresses on the host system are
// passed to the function. If this is set to one (1) and Bit 0 is set to one
// (1), only I/O addresses that are qualified by the Base and Limit registers
// are passed to the function. If Bit 0 is reset to zero (0), this bit is
// undefined.
//
// Bit 2 Enable IREQ# Routing - If this bit is reset to zero (0) and Bit 0
// is set to one (1), this function shall not generate interrupt requests
// on the CF+ Card's IREQ# line. If this is set to one (1) and
// Bit 0 is set to one (1), this function shall generate interrupt requests
// on the CF+ Card's IREQ# line. If Bit 0 is reset to zero (0), this bit is
// undefined.
//
// Bit 3..5 Reserved for vendor implementation.
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Card Configuration and Status Register (Base + 02h in Attribute Memory)
// The Card Configuration and Status Register contains information about the
//   Card's condition.
//
// Operation       D7      D6     D5      D4      D3     D2      D1    D0
// ----------   ------- ------ ------- ------- ------- ------ ------ ------
// Read         Changed SigChg  IOis8    -XE    Audio  PwrDwn   Int     0
// Write           0    SigChg  IOis8    -XE    Audio  PwrDwn    0      0
//
// Changed: indicates that one or both of the Pin Replacement register CRdy,
// or CWProt bits are set to one (1). When the Changed bit is set,
// -STSCHG Pin 46 is held low if the SigChg bit is a One (1) and the
// CompactFlash Storage Card or CF+ Card is configured for the I/O interface.
//
// SigChg: this bit is set and reset by the host to enable and disable a
// state-change signal from the Status Register, the Changed bit control
// pin 46 the Changed Status signal. If no state change signal is desired,
// this bit should be set to zero (0) and pin 46 (-STSCHG) signal will
// be held high while the CompactFlash Storage Card or CF+ Card is configured
// for I/O.
//
// IOis8: the host sets this bit to a one (1) if the CompactFlash Storage
// Card or CF+ Card is to be configured in an 8 bit I/O Mode. The
// CompactFlash Storage Card is always configured for both 8- and 16-bit I/O,
// so this bit is ignored. Some CF+ cards can be configured for either 8-
// bit I/O mode or 16-bit I/O mode, so CF+ cards may respond to this bit.
//
// -XE: this bit is set and reset by the host to disable and enable Power
// Level 1 commands in CF+ cards. If the value is 0, Power Level 1 commands
// are enabled; if it is 1, Power Level 1 commands are disabled. Default
// value at power on or after reset is 0. The host may read the value of
// this bit to determine whether Power Level 1 commands are currently enabled.
// For CompactFlash Storage cards (which must not support Power Level 1),
// this bit has value 0 and is not writeable.
//
// Audio: this bit is set and reset by the host to enable and disable
// audio information on SPKR# when the CF+ card is configured. This bit
// should always be zero for CompactFlash Storage cards.
//
// PwrDwn: this bit indicates whether the host requests the CompactFlash
// Storage Card or CF+ Card to be in the power saving or active mode. When
// the bit is one (1), the CompactFlash Storage Card or CF+ Card enters a
// power down mode. When zero (0), the host is requesting the CompactFlash
// Storage Card or CF+ Card to enter the active mode. The PCMCIA Rdy/-Bsy
// value becomes BUSY when this bit is changed. Rdy/-Bsy will not become
// Ready until the power state requested has been entered. The CompactFlash
// Storage Card automatically powers down when it is idle and powers back up
// when it receives a command.
//
// Int: this bit represents the internal state of the interrupt request.
// This value is available whether or not I/O interface has been configured.
// This signal remains true until the condition which caused the interrupt
// request has been serviced. If interrupts are disabled by the -IEN bit in
// the Device Control Register, this bit is a zero (0).
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Pin Replacement Register (Base + 04h in Attribute Memory)
//
// Operation   D7     D6      D5       D4     D3     D2      D1     D0
// --------- ------ ------ --------- ------ ------ ------ -------- ------
//   Read       0      0   CRdy/-Bsy CWProt    1      1   Rdy/-Bsy WProt
//   Write      0      0   CRdy/-Bsy CWProt    0      0  MRdy/-Bsy MWProt
//
// CRdy/-Bsy: this bit is set to one (1) when the bit RRdy/-Bsy changes state.
// This bit can also be written by the host.
//
// CWProt: this bit is set to one (1) when the RWprot changes state.
// This bit may also be written by the host.
//
// Rdy/-Bsy: this bit is used to determine the internal state of the Rdy/-Bsy
// signal. This bit may be used to determine the state of the Ready/-Busy as
// this pin has been reallocated for use as Interrupt Request on an I/O card.
// When written, this bit acts as a mask for writing the corresponding bit
// CRdy/-Bsy.
//
// Wprot: this bit is always zero (0) since the CompactFlash Storage Card or
// CF+ Card does not have a Write Protect switch. When written, this bit acts
// as a mask for writing the corresponding bit CWProt.
//
// MRdy/-Bsy: this bit acts as a mask for writing the corresponding bit
// CRdy/-Bsy.
//
// MWProt: this bit when written acts as a mask for writing the corresponding
// bit CWProt.
//
// Pin Replacement Changed Bit/Mask Bit Values
//
// Initial Value | Written by Host | Final | Comments
// of (C) Status | C-Bit   M-Bit   | C-Bit |
// ------------- | --------------- | ----- | ----------
//       0       |    X      0     |   0   | Unchanged
//       1       |    X      0     |   1   | Unchanged
//       X       |    0      1     |   0   | Cleared by Host
//       X       |    1      1     |   1   | Set by Host
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Socket and Copy Register (Base + 06h in Attribute Memory)
//
// This register contains additional configuration information. This register
// is always written by the system before writing the card's Configuration
// Index Register. This register is not required for CF+ Cards.
//
// Operation    D7     D6    D5     D4     D3     D2     D1     D0
// --------- ------ ------ ------ ------ ------ ------ ------ ------
// Read      Resrvd     0     0   Drive#    0      0      0      0
// Write         0      0     0   Drive#    X      X      X      X
//
// Reserved: this bit is reserved for future standardization. This bit must
// be set to zero (0) by the software when the register is written.
//
// Drive #: this bit indicates the drive number of the card for twin card
// configuration.
//
// X: the socket number is ignored by the CompactFlash Storage Card.
//----------------------------------------------------------------------------
static uint8 CFRegMem[] = {
    0x00, 0x00, 0x00, 0x80, 0x00, 0x2E, 0x00, 0x00,
};

// ---------------------------------------------------------------------------
//		� EmRegsCFConfig::InRange
// ---------------------------------------------------------------------------
Boolean EmRegsCFConfig::InRange(uint32 offset) {
    if (offset < sizeof(CFRegMem))
        return (true);
    else
        return (false);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFConfig::Initialize
// ---------------------------------------------------------------------------
void EmRegsCFConfig::Initialize() {}

// ---------------------------------------------------------------------------
//		� EmRegsCFConfig::ReadByte
// ---------------------------------------------------------------------------
void EmRegsCFConfig::ReadByte(uint32 offset, uint8* val) {
    if (InRange(offset))
        *val = CFRegMem[offset];
    else
        *val = 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsCFConfig::WriteByte
// ---------------------------------------------------------------------------
void EmRegsCFConfig::WriteByte(uint32 /*offset*/, uint8 /*val*/) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFConfig::ReadWord
// ---------------------------------------------------------------------------
void EmRegsCFConfig::ReadWord(uint32 offset, _Word* val) {
    ReadByte(offset, &val->Bytes[0]);
    ReadByte(offset + 1, &val->Bytes[1]);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFConfig::WriteWord
// ---------------------------------------------------------------------------
void EmRegsCFConfig::WriteWord(uint32 /*offset*/, _Word /*val*/) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFReset::Initialize
// ---------------------------------------------------------------------------
void EmRegsCFReset::Initialize() {}

// ---------------------------------------------------------------------------
//		� EmRegsCFReset::ReadByte
// ---------------------------------------------------------------------------
void EmRegsCFReset::ReadByte(uint32 /*offset*/, uint8* val) { *val = 0; }

// ---------------------------------------------------------------------------
//		� EmRegsCFReset::WriteByte
// ---------------------------------------------------------------------------
void EmRegsCFReset::WriteByte(uint32 /*offset*/, uint8 /*val*/) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFReset::ReadWord
// ---------------------------------------------------------------------------
void EmRegsCFReset::ReadWord(uint32 /*offset*/, _Word* val) { val->Bytes[0] = val->Bytes[1] = 0; }

// ---------------------------------------------------------------------------
//		� EmRegsCFReset::WriteWord
// ---------------------------------------------------------------------------
void EmRegsCFReset::WriteWord(uint32 /*offset*/, _Word /*val*/) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFTuple::Initialize
// ---------------------------------------------------------------------------
void EmRegsCFTuple::Initialize() {}

// ---------------------------------------------------------------------------
//		� EmRegsCFTuple::ReadByte
// ---------------------------------------------------------------------------
void EmRegsCFTuple::ReadByte(uint32 offset, uint8* val) { *val = 0; }

// ---------------------------------------------------------------------------
//		� EmRegsCFTuple::WriteByte
// ---------------------------------------------------------------------------
void EmRegsCFTuple::WriteByte(uint32 /*offset*/, uint8 /*val*/) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFTuple::ReadWord
// ---------------------------------------------------------------------------
void EmRegsCFTuple::ReadWord(uint32 offset, _Word* val) {
    ReadByte(offset, &val->Bytes[0]);
    ReadByte(offset + 1, &val->Bytes[1]);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFTuple::WriteWord
// ---------------------------------------------------------------------------
void EmRegsCFTuple::WriteWord(uint32 /*offset*/, _Word /*val*/) {}
