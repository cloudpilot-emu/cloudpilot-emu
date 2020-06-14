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

#ifndef _ROMSTUBS_H_
#define _ROMSTUBS_H_

#include "EmCommon.h"

Err FtrGet(UInt32 creator, UInt16 featureNum, UInt32* valueP);
Err FtrSet(UInt32 creator, UInt16 featureNum, UInt32 newValue);

UInt16 SysSetAutoOffTime(UInt16 seconds);

Err PenScreenToRaw(PointType* penP);

Err EvtEnqueuePenPoint(PointType* ptP);

Err EvtWakeup(void);

Err EvtEnqueueKey(UInt16 ascii, UInt16 keycode, UInt16 modifiers);

#endif /* _ROMSTUBS_H_ */
