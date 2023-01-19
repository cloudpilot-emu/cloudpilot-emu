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

#include "EmTRGDiskIO.h"

#include <cstring>

#include "EmCommon.h"

/************************************************************************
 * This class handles the generic low level disk access.
 ************************************************************************/
#include <stdio.h>

#include "EmCommon.h"
#include "EmTRGDiskIO.h"

#define SECTOR_SIZE 512

EmTRGDiskIO::EmTRGDiskIO() {}

EmTRGDiskIO::~EmTRGDiskIO() {}

void EmTRGDiskIO::Initialize(int driveNo) {}

void EmTRGDiskIO::Dispose(void) {}

int EmTRGDiskIO::ReadSector(uint32 sectorNum, void *buffer) {
    memset(buffer, 0, SECTOR_SIZE);

    return 0;
}

int EmTRGDiskIO::WriteSector(uint32 sectorNum, void *buffer) { return 0; }
