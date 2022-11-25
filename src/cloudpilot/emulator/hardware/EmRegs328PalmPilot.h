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

#ifndef EmRegs328PalmPilot_h
#define EmRegs328PalmPilot_h

#include "EmRegs328.h"  // EmRegs328

class EmRegs328PalmPilot : public EmRegs328 {
   public:
    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLineDriverState(EmUARTDeviceType);

    virtual uint8 GetPortInputValue(int);
};

#endif /* EmRegs328PalmPilot_h */
