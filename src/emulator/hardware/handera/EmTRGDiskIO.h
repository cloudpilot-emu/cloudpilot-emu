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

#ifndef EmTRGDiskIO_h
#define EmTRGDiskIO_h

/************************************************************************
 * This class handles the generic low level disk access.
 ************************************************************************/
#include "EmTRGCFDefs.h"

#define UNKNOWN_DRIVE 0
#define CF_DRIVE 1
#define SD_DRIVE 2

#define SECTOR_SIZE 512

class EmTRGDiskIO {
   public:
    EmTRGDiskIO(void);
    ~EmTRGDiskIO();

    void Initialize(int driveNo);
    void Dispose(void);

    int ReadSector(uint32 sectorNum, void *buffer);
    int WriteSector(uint32 sectorNum, void *buffer);
};

#endif /* EmTRGDiskIO_h */
