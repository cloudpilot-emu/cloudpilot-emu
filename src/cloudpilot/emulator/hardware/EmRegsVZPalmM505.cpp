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

#include "EmRegsVZPalmM505.h"

#include "EmBankRegs.h"  // EmBankRegs::DisableSubBank
#include "EmCommon.h"
#include "EmRegsSED1376.h"  // sed1376RegsAddr
#include "EmRegsVZPrv.h"

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM505::EmRegsVZPalmM505
// ---------------------------------------------------------------------------

EmRegsVZPalmM505::EmRegsVZPalmM505(void) : EmRegsVZPalmM500() {}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM505::~EmRegsVZPalmM505
// ---------------------------------------------------------------------------

EmRegsVZPalmM505::~EmRegsVZPalmM505(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM505::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsVZPalmM505::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegsVZ::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    this->SetHandler((ReadFunction)&EmRegsVZPalmM505::StdRead,
                     (WriteFunction)&EmRegsVZPalmM505::portFSelectWrite, db_addressof(portFSelect),
                     sizeof(f68VZ328Regs.portFSelect));
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::portFSelectWrite
// ---------------------------------------------------------------------------

void EmRegsVZPalmM505::portFSelectWrite(emuptr address, int size, uint32 value) {
    uint8 oldPortFSelect = READ_REGISTER(portFSelect);

    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    uint8 portFSelect = READ_REGISTER(portFSelect);

#define hwrVZPortFBusClock 0x04  // (P) Bus Clock

    if (((portFSelect ^ oldPortFSelect) & hwrVZPortFBusClock) != 0) {
        if ((portFSelect & hwrVZPortFBusClock) != 0) {
            EmBankRegs::DisableSubBank(sed1376RegsAddr);
            EmBankRegs::DisableSubBank(sed1376VideoMemStart);
        } else {
            EmBankRegs::EnableSubBank(sed1376RegsAddr);
            EmBankRegs::EnableSubBank(sed1376VideoMemStart);
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM505::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM505::GetLCDScreenOn(void) {
    // Override the Dragonball version and let the SED 1376 handle it.

    return EmHALHandler::GetLCDScreenOn();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM505::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM505::GetLCDBacklightOn(void) {
    // Override the Dragonball version and let the SED 1376 handle it.

    return EmHALHandler::GetLCDBacklightOn();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZPalmM505::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmRegsVZPalmM505::GetLCDHasFrame(void) {
    // Override the Dragonball version and let the SED 1376 handle it.

    return EmHALHandler::GetLCDHasFrame();
}

bool EmRegsVZPalmM505::CopyLCDFrame(Frame& frame, bool fullRefresh) {
    return EmHALHandler::CopyLCDFrame(frame, fullRefresh);
}

uint16 EmRegsVZPalmM505::GetLCD2bitMapping() { return EmHALHandler::GetLCD2bitMapping(); }

void EmRegsVZPalmM505::MarkScreen() { return; }

void EmRegsVZPalmM505::UnmarkScreen() { return; }

void EmRegsVZPalmM505::MarkScreenDirty() { return; }
