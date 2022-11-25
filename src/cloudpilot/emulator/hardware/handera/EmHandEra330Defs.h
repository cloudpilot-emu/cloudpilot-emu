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
#ifndef EmHandEra330Defs_h
#define EmHandEra330Defs_h

#include "EmCommon.h"
#include "EmHandEraCFBus.h"
#include "EmHandEraSDBus.h"

#define HE330_NUM_KEY_ROWS 4

typedef struct {
    uint16 Row[HE330_NUM_KEY_ROWS];
} HandEra330Keys;

typedef struct {
    HandEra330Keys Keys;
    CFBusManager CFBus;
    SDBusManager SDBus;
    Bool LCDOn;
    Bool BacklightOn;
    Bool IRPortOn;
    Bool SenseCurrent;
    Bool pendingIRQ2;
    Bool CFInserted;
    Bool SDInserted;
    Bool PowerConnected;
    Bool SDChipSelect;
} HandEra330PortManager;

#endif  // EmHandEra330Defs_h
