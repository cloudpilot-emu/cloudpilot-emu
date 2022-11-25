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

#include "EmTRGCFIO.h"

#include "EmCommon.h"

//-------------------------------------------------------------------------
//  This file implements card I/O emulation by performing read and write
//  operations on a file on the PC.  Note that this file doesn't know
//  anything about ATA registers ... it's more of a state machine for
//  a continuous series of reads and writes
//
//  These functions will create a disk file on the PC ... if one doesn't
//  exist, it will create it in a formatted state.
//
//  The code should work even if the disk file is replaced by an image
//  from a real card ... there is no additional info stored with the card
//  however, the tuple info won't agree with the card
//---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		� EmCFIO::EmCFIO
// ---------------------------------------------------------------------------
EmCFIO::EmCFIO(void) {}

// ---------------------------------------------------------------------------
//		� EmCFIO::~EmCFIO
// ---------------------------------------------------------------------------
EmCFIO::~EmCFIO(void) {}

// ---------------------------------------------------------------------------
//		� EmCFIO::~EmCFIO
// ---------------------------------------------------------------------------
void EmCFIO::Reset(void) {
    State.NumSectorsRequested = 0;
    State.NumSectorsCompleted = 0;
    State.SectorIndex = 0;
    State.Status = DIO_SUCCESS;
    State.Error = 0;
}

// ---------------------------------------------------------------------------
//		� EmCFIO::Initialize
// ---------------------------------------------------------------------------
void EmCFIO::Initialize() {
    DiskIO.Initialize(CF_DRIVE);
    Reset();
}

// ---------------------------------------------------------------------------
//		� EmCFIO::Dispose
// ---------------------------------------------------------------------------
void EmCFIO::Dispose(void) { DiskIO.Dispose(); }

// ---------------------------------------------------------------------------
//		� EmCFIO::ReadSector
// ---------------------------------------------------------------------------
DiskIOStatus EmCFIO::ReadSector(void) {
    if ((DiskIO.ReadSector(State.Lba, (char *)State.Sector.Bytes)) != 0) return DIO_ERROR;

    return DIO_SUCCESS;
}

// ---------------------------------------------------------------------------
//		� EmCFIO::WriteSector
// ---------------------------------------------------------------------------
DiskIOStatus EmCFIO::WriteSector(void) {
    if ((DiskIO.WriteSector(State.Lba, (char *)State.Sector.Bytes)) != 0) return DIO_ERROR;

    return DIO_SUCCESS;
}

// ---------------------------------------------------------------------------
//		� EmCFIO::ReadNextDataByte
// ---------------------------------------------------------------------------
void EmCFIO::ReadNextDataByte(uint8 *val) {
    // this first statement will likely not be called except when someone's
    // dumping memory at our address range
    if (State.NumSectorsCompleted >= State.NumSectorsRequested)
        *val = 0;
    else {
        *val = State.Sector.Bytes[State.SectorIndex++];
        if (State.SectorIndex >= SECTOR_SIZE) {
            if (++State.NumSectorsCompleted < State.NumSectorsRequested) {
                State.Lba++;
                State.SectorIndex = 0;
                State.Status = ReadSector();
            }
        }
    }
    if (State.Status != DIO_SUCCESS) State.Error = IDE_ERR_01_GENERAL_ERROR;
}

// ---------------------------------------------------------------------------
//		� EmCFIO::WriteNextDataByte
// ---------------------------------------------------------------------------
void EmCFIO::WriteNextDataByte(uint8 val) {
    State.Status = DIO_SUCCESS;
    if (State.NumSectorsCompleted < State.NumSectorsRequested) {
        State.Sector.Bytes[State.SectorIndex++] = val;
        if (State.SectorIndex >= SECTOR_SIZE) {
            if (WriteSector() == DIO_ERROR)
                State.Status = DIO_ERROR;
            else if (++State.NumSectorsCompleted < State.NumSectorsRequested) {
                State.Status = DIO_SUCCESS;
                State.Lba++;
                State.SectorIndex = 0;
            }
        }
    }
    if (State.Status != DIO_SUCCESS) State.Error = IDE_ERR_01_GENERAL_ERROR;
}

// ---------------------------------------------------------------------------
//		� EmCFIO::GetSectorCount
// ---------------------------------------------------------------------------
uint32 EmCFIO::GetSectorCount(void) { return State.NumSectorsCompleted; }

// ---------------------------------------------------------------------------
//		� EmCFIO::StartDriveID
// ---------------------------------------------------------------------------
void EmCFIO::StartDriveID(void) {
    State.NumSectorsRequested = 1;
    State.NumSectorsCompleted = 0;
    State.SectorIndex = 0;
    State.Status = DIO_SUCCESS;
    State.Error = 0;
}

// ---------------------------------------------------------------------------
//		� EmCFIO::StartRead
// ---------------------------------------------------------------------------
void EmCFIO::StartRead(DiskIOParams *params) {
    State.Lba = params->Lba;
    State.NumSectorsRequested = params->SectorCnt;
    State.NumSectorsCompleted = 0;
    State.SectorIndex = 0;
    State.Error = 0;
    State.Status = ReadSector();
}

// ---------------------------------------------------------------------------
//		� EmCFIO::StartWrite
// ---------------------------------------------------------------------------
void EmCFIO::StartWrite(DiskIOParams *params) {
    State.Lba = params->Lba;
    State.NumSectorsRequested = params->SectorCnt;
    State.NumSectorsCompleted = 0;
    State.SectorIndex = 0;
    State.Status = DIO_SUCCESS;
    State.Error = 0;
}

// ---------------------------------------------------------------------------
//		� EmCFIO::GetStatus
// ---------------------------------------------------------------------------
void EmCFIO::GetStatus(DiskIOStatus *status, DiskDataStatus *dataStatus) {
    *status = State.Status;
    if (State.NumSectorsCompleted == State.NumSectorsRequested)
        *dataStatus = DIO_DATA_COMPLETE;
    else
        *dataStatus = DIO_MORE_DATA;
}

// ---------------------------------------------------------------------------
//		� EmCFIO::GetError
// ---------------------------------------------------------------------------
DiskIOError EmCFIO::GetError(void) {
    if (State.Status == DIO_SUCCESS)
        return (0);
    else
        // at this point, all errors are pretty much the same ... we can't
        // create our emulation file ... we probably will need to include
        // some more refined error codes at some point
        return (State.Error);
}

void EmCFIO::SetError(DiskIOError error) {
    State.Status = DIO_ERROR;
    State.Error = error;
}

void EmCFIO::ClearError(void) {
    State.Status = DIO_SUCCESS;
    State.Error = 0;
}
