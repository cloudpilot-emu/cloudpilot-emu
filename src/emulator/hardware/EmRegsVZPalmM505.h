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

#ifndef EmRegsVZPalmM505_h
#define EmRegsVZPalmM505_h

#include "EmRegsVZPalmM500.h"

class EmRegsVZPalmM505 : public EmRegsVZPalmM500 {
   public:
    EmRegsVZPalmM505(void);
    virtual ~EmRegsVZPalmM505(void);

    virtual void SetSubBankHandlers(void);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLCDHasFrame(void);
    virtual void GetLCDBeginEnd(emuptr& begin, emuptr& end);
    virtual bool CopyLCDFrame(Frame& frame);
    virtual uint16 GetLCD2bitMapping();

    void portFSelectWrite(emuptr address, int size, uint32 value);

   protected:
    virtual void MarkScreen();
    virtual void UnmarkScreen();
    virtual void MarkScreenDirty();
};

#endif /* EmRegsVZPalmM505_h */
