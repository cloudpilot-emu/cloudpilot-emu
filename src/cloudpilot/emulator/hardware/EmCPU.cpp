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

#include "EmCPU.h"

#include "EmCommon.h"

EmCPU* gCPU;

// ---------------------------------------------------------------------------
//		� EmCPU::EmCPU
// ---------------------------------------------------------------------------

EmCPU::EmCPU(EmSession* session) : fSession(session) {
    EmAssert(gCPU == NULL);
    gCPU = this;
}

// ---------------------------------------------------------------------------
//		� EmCPU::~EmCPU
// ---------------------------------------------------------------------------

EmCPU::~EmCPU(void) {
    EmAssert(gCPU == this);
    gCPU = NULL;
}

// ---------------------------------------------------------------------------
//		� EmCPU::Reset
// ---------------------------------------------------------------------------

void EmCPU::Reset(Bool /*hardwareReset*/) {}

void EmCPU::Save(Savestate&) {}

void EmCPU::Save(SavestateProbe&) {}

void EmCPU::Load(SavestateLoader&) {}
