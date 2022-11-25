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

#ifndef EmTRGATA_h
#define EmTRGATA_h

#include "EmRegs.h"
#include "EmTRGCFDefs.h"
#include "EmTRGCFIO.h"

#define IS_ALT_REG true
#define IS_BASE_REG false

typedef enum { MODE_DISK_IO = 0, MODE_DRIVE_DIAGNOSTICS = 1 } AtaModeType;

class EmRegsCFAta {
   public:
    EmRegsCFAta(void);
    virtual ~EmRegsCFAta(void);

    void Initialize();
    void Reset(void);
    void Dispose(void);
    void ReadByte(uint32 offset, uint8* val);
    void WriteByte(uint32 offset, uint8 val);
    void ReadWord(uint32 offset, _Word* val);
    void WriteWord(uint32 offset, _Word val);

   private:
    uint8 RegMem[NUM_IDE_REGS];
    AtaModeType AtaMode;
    DiskIOParams DiskParams;
    void GetDiskIOParams(DiskIOParams* params);
    uint8 Reg0ReadData(void);
    uint8 RegReadError(Boolean is_alt_reg);
    uint8 Reg2ReadSectorCount(void);
    void Reg0WriteData(void);
    void Reg7WriteCmd(void);
    uint8 RegReadStatus(Boolean is_alt_reg);
    uint8 Reg8ReadDataEven(void);
    void Reg8WriteDataEven(void);
    uint8 Reg9ReadDataOdd(void);
    void Reg9WriteDataOdd(void);
    void RegEWriteDeviceControl(void);
    EmCFIO DiskIO;
    uint8 ConvertStatus(void);
    void CmdIdentifyDrive(void);
    void CmdWriteSectors(void);
    void CmdReadSectors(void);
    void CmdDriveDiagnostic(void);
};

#endif  // EmTRGATA_h
