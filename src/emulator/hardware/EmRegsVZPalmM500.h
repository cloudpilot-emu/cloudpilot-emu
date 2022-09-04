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

#ifndef EmRegsVZPalmM500_h
#define EmRegsVZPalmM500_h

#include "EmRegsVZ.h"

class EmRegsVZPalmM500 : public EmRegsVZ {
   public:
    EmRegsVZPalmM500(void);
    virtual ~EmRegsVZPalmM500(void);

    Bool GetLCDScreenOn(void) override;
    Bool GetLCDBacklightOn(void) override;
    Bool GetLineDriverState(EmUARTDeviceType type) override;
    EmUARTDeviceType GetUARTDevice(int uartNum) override;
    Bool GetVibrateOn(void) override;
    uint16 GetLEDState(void) override;

    uint8 GetPortInputValue(int) override;
    uint8 GetPortInternalValue(int) override;
    void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) override;

    bool SupportsSlot(EmHAL::Slot slot) override;
    void Mount(EmHAL::Slot slot, const string& key, CardImage& cardImage) override;
    void Unmount(EmHAL::Slot slot) override;

   protected:
    EmSPISlave* GetSPISlave(void) override;
};

#endif /* EmRegsVZPalmM500_h */
