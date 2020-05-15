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

#ifndef EmRegsPrv_h
#define EmRegsPrv_h

#include "EmMemory.h"  // EmMemDoGet8, EmMemDoPut8, etc.

// Inline functions for reading/writing registers.

inline int8 _get_reg(int8* a) { return EmMemDoGet8(a); }
inline int16 _get_reg(int16* a) { return EmMemDoGet16(a); }
inline int32 _get_reg(int32* a) { return EmMemDoGet32(a); }

inline uint8 _get_reg(uint8* a) { return EmMemDoGet8(a); }
inline uint16 _get_reg(uint16* a) { return EmMemDoGet16(a); }
inline uint32 _get_reg(uint32* a) { return EmMemDoGet32(a); }

inline void _put_reg(int8* a, int8 v) { EmMemDoPut8(a, v); }
inline void _put_reg(int16* a, int16 v) { EmMemDoPut16(a, v); }
inline void _put_reg(int32* a, int32 v) { EmMemDoPut32(a, v); }

inline void _put_reg(uint8* a, uint8 v) { EmMemDoPut8(a, v); }
inline void _put_reg(uint16* a, uint16 v) { EmMemDoPut16(a, v); }
inline void _put_reg(uint32* a, uint32 v) { EmMemDoPut32(a, v); }

#endif  // EmRegsPrv_h
