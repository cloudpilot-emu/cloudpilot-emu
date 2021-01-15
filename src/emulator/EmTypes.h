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

#ifndef EmTypes_h
#define EmTypes_h

#include <stdint.h>

// =====================================
// ========== SIMPLE TYPEDEFS ==========
// =====================================

// Standard integral types

typedef int8_t int8;
typedef uint8_t uint8;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int32_t int32;
typedef uint32_t uint32;

typedef int64_t int64;
typedef uint64_t uint64;

typedef uint32 emuptr;

typedef bool Bool;

typedef int32 ErrCode;
typedef int32 StrCode;

typedef uint8 ScaleType;
typedef int32 RAMSizeType;
typedef int32 EmCoord;

#define EmMemNULL ((emuptr)0)
#define EmMemEOM ((emuptr)0xFFFFFFFF)

#endif  // EmTypes_h
