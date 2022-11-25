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

#include "EmRegsEZPalmVII.h"

#include "EmCommon.h"
#include "EmRegsEZPrv.h"

// clang-format off
#include "PalmPack.h"
// clang-format on

#define NON_PORTABLE
#include "EZSumo/IncsPrv/HardwareEZ.h"  // hwrEZPortGIRShutdown
#undef NON_PORTABLE
#include "PalmPackPop.h"

// ---------------------------------------------------------------------------
//		� EmRegsEZPalmVII::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsEZPalmVII::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial)
        // Pass this on to the PLD handler.
        return EmHALHandler::GetLineDriverState(type);

    if (type == kUARTIR) return (READ_REGISTER(portGData) & hwrEZPortGIRShutdown) == 0;

    return false;
}
