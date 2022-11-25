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

#include "EmTRGATA.h"

#include "EmCommon.h"

// ------------------------------------------------------------------------
// This file emulates the ATA Registers on the CF Memory Card.
//
// Address                   Description
// ---------------------     --------------------
// 0x18001000-0x1800100F     ATA Registers
// 0x18001010-0x180013FF     Mirror of the ATA Registers
// 0x18001400-0x18001FFF     Equivalent to the Data-Even/Data-Odd Registers
// ------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::EmRegsCFAta
// ---------------------------------------------------------------------------
EmRegsCFAta::EmRegsCFAta(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::~EmRegsCFATA
// ---------------------------------------------------------------------------
EmRegsCFAta::~EmRegsCFAta(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Initialize
// ---------------------------------------------------------------------------
void EmRegsCFAta::Initialize() {
    int i;

    AtaMode = MODE_DISK_IO;
    DiskIO.Initialize();
    for (i = 0; i < NUM_IDE_REGS; i++) RegMem[i] = i;
    RegMem[6] = 0xA0;
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Reset
// ---------------------------------------------------------------------------
void EmRegsCFAta::Reset(void) { DiskIO.Reset(); }

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Dispose
// ---------------------------------------------------------------------------
void EmRegsCFAta::Dispose(void) { DiskIO.Dispose(); }

//----------------------------------------------------------------------------
// Status and Alternate Status Registers Offsets 7h and Eh
//
// These registers return the CompactFlash Storage Card status when read by
// the host. Reading the Status register does clear a pending interrupt while
// reading the Auxiliary Status register does not. The status bits are
// described as follows:
//
//   D7  D6  D5  D4  D3  D2  D1  D0
//  --- --- --- --- --- --- --- ---
//  BSY RDY DWF DSC DRQ CRR  0  ERR
//
// Bit 7 (BUSY): the busy bit is set when the CompactFlash Storage Card has
// access to the command buffer and registers and the host is locked out
// from accessing the command register and buffer. No other bits in this
// register are valid when this bit is set to a 1.
// Bit 6 (RDY): RDY indicates whether the device is capable of performing
// CompactFlash Storage Card operations. This bit is cleared at power up
// and remains cleared until the CompactFlash Storage Card is ready to
// accept a command.
// Bit 5 (DWF): This bit, if set, indicates a write fault has occurred.
// Bit 4 (DSC): This bit is set when the CompactFlash Storage Card is ready.
// Bit 3 (DRQ): The Data Request is set when the CompactFlash Storage Card
// requires that information be transferred either to or from the host
// through the Data register.
// Bit 2 (CORR): This bit is set when a Correctable data error has been
// encountered and the data has been corrected. This condition does not
// terminate a multi-sector read operation.
// Bit 1 (IDX): This bit is always set to 0.
// Bit 0 (ERR): This bit is set when the previous command has ended in
// some type of error. The bits in the Error register contain additional
// information describing the error. It is recommended that media access
// commands (such as Read Sectors and Write Sectors) that end with an
// error condition should have the address of the first sector in error
// in the command block registers.
//----------------------------------------------------------------------------
uint8 EmRegsCFAta::RegReadStatus(Boolean /*is_alt_reg*/) {
    uint8 retVal;
    DiskIOStatus status;
    DiskDataStatus dataStatus;

    DiskIO.GetStatus(&status, &dataStatus);
    if (status == DIO_SUCCESS) {
        retVal = (IDE_STS_RDY | IDE_STS_DSC);
        if (dataStatus == DIO_MORE_DATA) {
            retVal |= IDE_STS_DRQ;
        } else {
            DiskParams.Lba += DiskParams.SectorCnt;
            RegMem[IDE_REG_5_LBA_23_16] = (uint8)(DiskParams.Lba >> 16);
            RegMem[IDE_REG_4_LBA_15_8] = (uint8)(DiskParams.Lba >> 8);
            RegMem[IDE_REG_3_LBA_7_0] = (uint8)DiskParams.Lba;
        }
    } else {
        retVal = (IDE_STS_RDY | IDE_STS_DSC | IDE_STS_ERR);
    }
    return (retVal);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::GetDiskIOParams
// ---------------------------------------------------------------------------
void EmRegsCFAta::GetDiskIOParams(DiskIOParams* Params) {
    Params->DriveNum = 0;
    Params->Lba = (((uint32)RegMem[IDE_REG_6_DRV_HEAD] & 0x0F) << 24) +
                  ((uint32)RegMem[IDE_REG_5_LBA_23_16] << 16) +
                  ((uint32)RegMem[IDE_REG_4_LBA_15_8] << 8) + (uint32)RegMem[IDE_REG_3_LBA_7_0];
    Params->SectorCnt = (uint32)RegMem[IDE_REG_2_SECTOR_CNT];
    if (Params->SectorCnt == 0) Params->SectorCnt = 256;

    DiskParams = *Params;
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::CmdIdentifyDrive
// ---------------------------------------------------------------------------
void EmRegsCFAta::CmdIdentifyDrive(void) { DiskIO.StartDriveID(); }

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::CmdReadSectors
// ---------------------------------------------------------------------------
void EmRegsCFAta::CmdReadSectors(void) {
    DiskIOParams params;

    GetDiskIOParams(&params);
    DiskIO.StartRead(&params);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::CmdWriteSectors
// ---------------------------------------------------------------------------
void EmRegsCFAta::CmdWriteSectors(void) {
    DiskIOParams params;

    GetDiskIOParams(&params);
    DiskIO.StartWrite(&params);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::CmdDriveDiagnostic
// ---------------------------------------------------------------------------
void EmRegsCFAta::CmdDriveDiagnostic(void) { AtaMode = MODE_DRIVE_DIAGNOSTICS; }

//----------------------------------------------------------------------------
// Data Register Offset 0 (8-bit) and 8-9 (16-bit)
//
// The Data Register is a 16-bit register, and it is used to transfer data
// blocks between the CompactFlash Storage Card data buffer and the Host.
// This register overlaps the Error Register.  The table below describes the
// combinations of data register access and is provided to assist in
// understanding the overlapped Data Register and Error/Feature Register
// rather than to attempt to define general PCMCIA word and byte access
// modes and operations. See the PCMCIA PC Card Standard Release 2.0 for
// definitions of the Card Accessing Modes for I/O and Memory cycles.
// Note: Because of the overlapped registers, access to the 1F1h, 171h or
// offset 1 are not defined for word (-CE2 = 0 and -CE1 = 0) operations.
// These accesses are treated as accesses to the Word Data Register.
// The duplicated registers at offsets 8, 9 and Dh have no restrictions on
// the operations that can be performed by the socket.
//----------------------------------------------------------------------------
uint8 EmRegsCFAta::Reg0ReadData(void) {
    DiskIO.ReadNextDataByte(&RegMem[IDE_REG_0_DATA]);
    return (RegMem[IDE_REG_0_DATA]);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Reg0WriteData
// ---------------------------------------------------------------------------
void EmRegsCFAta::Reg0WriteData(void) { DiskIO.WriteNextDataByte(RegMem[IDE_REG_0_DATA]); }

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Reg8ReadDataEven
// ---------------------------------------------------------------------------
uint8 EmRegsCFAta::Reg8ReadDataEven(void) {
    DiskIO.ReadNextDataByte(&RegMem[IDE_REG_8_DATA_EVEN]);
    return (RegMem[IDE_REG_8_DATA_EVEN]);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Reg8WriteDataEven
// ---------------------------------------------------------------------------
void EmRegsCFAta::Reg8WriteDataEven(void) { DiskIO.WriteNextDataByte(RegMem[IDE_REG_8_DATA_EVEN]); }

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Reg9ReadDataOdd
// ---------------------------------------------------------------------------
uint8 EmRegsCFAta::Reg9ReadDataOdd(void) {
    DiskIO.ReadNextDataByte(&RegMem[IDE_REG_9_DATA_ODD]);
    return (RegMem[IDE_REG_9_DATA_ODD]);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Reg9WriteDataOdd
// ---------------------------------------------------------------------------
void EmRegsCFAta::Reg9WriteDataOdd(void) { DiskIO.WriteNextDataByte(RegMem[IDE_REG_9_DATA_ODD]); }

//----------------------------------------------------------------------------
// Error Register -- Offset 1h and Dh (read-only)
//
// This register contains additional information about the source of
// an error when an error is indicated in bit 0 of the Status register.
// The bits are defined as follows:
//
// Error Register
// This register is also accessed on data bits D15-D8 during a write
// operation to offset 0 with -CE2 low and -CE1 high.
//
// Bit 7 (BBK): this bit is set when a Bad Block is detected.
// Bit 6 (UNC): this bit is set when an Uncorrectable Error is encountered.
// Bit 5: this bit is 0.
// Bit 4 (IDNF): the requested sector ID is in error or cannot be found.
// Bit 3: this bit is 0.
// Bit 2 (Abort) This bit is set if the command has been aborted because of
//    a CompactFlash Storage Card status condition: (Not Ready, Write Fault,
//    etc.) or when an invalid command has been issued.
// Bit 1 This bit is 0.
// Bit 0 (AMNF) This bit is set in case of a general error.
//----------------------------------------------------------------------------
uint8 EmRegsCFAta::RegReadError(Boolean /*is_alt_reg*/) {
    if (AtaMode == MODE_DRIVE_DIAGNOSTICS)
        return (IDE_DIA_01_NO_ERROR);
    else
        return DiskIO.GetError();
}

//----------------------------------------------------------------------------
// Sector Count Register (Offset 2h)
//
// This register contains the numbers of sectors of data requested to be
// transferred on a read or write operation between the host and the
// CompactFlash Storage Card. If the value in this register is zero,
// a count of 256 sectors is specified. If the command was successful,
// this register is zero at command completion. If not successfully completed,
// the register contains the number of sectors that need to be transferred
// in order to complete the request.
//----------------------------------------------------------------------------
uint8 EmRegsCFAta::Reg2ReadSectorCount(void) {
    uint32 cnt;

    cnt = DiskIO.GetSectorCount();
    if (cnt > 0xFF) cnt = 0x0;
    return (cnt);
}

//----------------------------------------------------------------------------
// Device Control Register -- Offset Eh
// This register is used to control the CompactFlash Storage Card interrupt
// request and to issue an ATA soft reset to the card. This register can
// be written even if the device is BUSY. The bits are defined as follows:
//
//  D7  D6  D5  D4  D3   D2    D1  D0
// --- --- --- --- --- ------ ---- ---
//  X   X   X   X   1  SW Rst -IEn  0
//
// Bit 7: this bit is an X (don't care).
// Bit 6: this bit is an X (don't care).
// Bit 5: this bit is an X (don't care).
// Bit 4: this bit is an X (don't care).
// Bit 3: this bit is ignored by the CompactFlash Storage Card.
// Bit 2 (SW Rst): this bit is set to 1 in order to force the CompactFlash
// Storage Card to perform an AT Disk controller Soft Reset operation.
// This does not change the PCMCIA Card Configuration Registers (4.3.2
// to 4.3.5) as a hardware Reset does. The Card remains in Reset until this
// bit is reset to 0.
// Bit 1 (-IEn): the Interrupt Enable bit enables interrupts when the bit is
// 0. When the bit is 1, interrupts from the CompactFlash Storage Card are
// disabled. This bit also controls the Int bit in the Configuration and
// Status Register. This bit is set to 0 at power on and Reset.
// Bit 0: this bit is ignored by the CompactFlash Storage Card.
//----------------------------------------------------------------------------
void EmRegsCFAta::RegEWriteDeviceControl(void) { DiskIO.Reset(); }

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::Reg7WriteCmd
// ---------------------------------------------------------------------------
void EmRegsCFAta::Reg7WriteCmd(void) {
    AtaMode = MODE_DISK_IO;
    DiskIO.ClearError();
    switch (RegMem[IDE_REG_7_COMMAND]) {
        case IDE_CMD_EC_IDENTIFY_DRIVE:
            CmdIdentifyDrive();
            break;

        case IDE_CMD_20_READ_SECTORS:
        case IDE_CMD_21_READ_SECTORS:
            CmdReadSectors();
            break;

        case IDE_CMD_30_WRITE_SECTORS:
        case IDE_CMD_31_WRITE_SECTORS:
            CmdWriteSectors();
            break;

        case IDE_CMD_90_EXEC_DRIVE_DIAGNOSTIC:
            CmdDriveDiagnostic();
            break;

        case IDE_CMD_C6_SET_MULTIPLE_MODE:
            DiskIO.SetError(IDE_ERR_04_ABORT);
            break;

        default:
            break;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::ReadByte
// ---------------------------------------------------------------------------
void EmRegsCFAta::ReadByte(uint32 offset, uint8* val) {
    *val = 0;
    switch (offset) {
        case IDE_REG_0_DATA:
            *val = Reg0ReadData();
            break;
        case IDE_REG_1_ERROR:
            *val = RegReadError(IS_BASE_REG);
            break;
        case IDE_REG_D_ERROR:
            *val = RegReadError(IS_ALT_REG);
            break;
        case IDE_REG_2_SECTOR_CNT:
            *val = Reg2ReadSectorCount();
            break;
        case IDE_REG_7_STATUS:
            *val = RegReadStatus(IS_BASE_REG);
            break;
        case IDE_REG_E_ALT_STATUS:
            *val = RegReadStatus(IS_ALT_REG);
            break;
        case IDE_REG_8_DATA_EVEN:
            *val = Reg8ReadDataEven();
            break;
        case IDE_REG_9_DATA_ODD:
            *val = Reg9ReadDataOdd();
            break;
        default:
            if (offset < NUM_IDE_REGS)
                *val = RegMem[offset];
            else
                *val = 0;
            break;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::WriteByte
// ---------------------------------------------------------------------------
void EmRegsCFAta::WriteByte(uint32 offset, uint8 val) {
    RegMem[offset] = val;
    switch (offset) {
        case IDE_REG_0_DATA:
            Reg0WriteData();
            break;
        case IDE_REG_7_COMMAND:
            Reg7WriteCmd();
            break;
        case IDE_REG_8_DATA_EVEN:
            Reg8WriteDataEven();
            break;
        case IDE_REG_9_DATA_ODD:
            Reg9WriteDataOdd();
            break;
        case IDE_REG_E_DEVICE_CONTROL:
            RegEWriteDeviceControl();
            break;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::ReadWord
// ---------------------------------------------------------------------------
void EmRegsCFAta::ReadWord(uint32 offset, _Word* val) {
    // 16-bit reads of Register 0 are NOT equivalent to 2 8-bit reads
    // of Register 0 and 1 ... instead it reads the Even & Odd Data
    // bytes, so we change the register to be 8
    if (offset == IDE_REG_0_DATA) offset = IDE_REG_8_DATA_EVEN;
    ReadByte(offset, &val->Bytes[0]);
    ReadByte(offset + 1, &val->Bytes[1]);
}

// ---------------------------------------------------------------------------
//		� EmRegsCFAta::WriteWord
// ---------------------------------------------------------------------------
void EmRegsCFAta::WriteWord(uint32 offset, _Word val) {
    // 16-bit writes of Register 0 are NOT equivalent to 2 8-bit writes
    // of Register 0 and 1 ... instead it writes the Even & Odd Data
    // bytes, so we change the register to be 8
    if (offset == IDE_REG_0_DATA) offset = IDE_REG_8_DATA_EVEN;
    WriteByte(offset, val.Bytes[0]);
    WriteByte(offset + 1, val.Bytes[1]);
}
