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

#ifndef EmHandEraCFBus_h
#define EmHandEraCFBus_h

enum CFBusWidth { kCFBusWidth8, kCFBusWidth16 };

typedef struct {
    int bEnabled;
    enum CFBusWidth Width;
    int bSwapped;
} CFBusManager;

#endif  // EmHandEraBusDefs_h
