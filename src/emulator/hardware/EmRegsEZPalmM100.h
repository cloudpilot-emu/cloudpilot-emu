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

#ifndef EmRegsEZm100_h
#define EmRegsEZm100_h

#include "EmRegsEZ.h"

class EmRegsEZPalmM100 : public EmRegsEZ {
   public:
    EmRegsEZPalmM100(void);
    virtual ~EmRegsEZPalmM100(void);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLineDriverState(EmUARTDeviceType type);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

   private:
    EmSPISlave* fSPISlaveADC;
};

#endif /* EmRegsEZm100_h */
