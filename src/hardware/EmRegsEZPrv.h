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

#ifndef EmRegsEZPrv_h
#define EmRegsEZPrv_h

#include "EmRegsPrv.h"
#include "Palm.h"

// Location and range of registers

const uint32 kMemoryStart = 0xFFFFF000;
const uint32 kMemorySize = sizeof(HwrM68EZ328Type);

// Macro to return the DragonballEZ address of the specified register

#define addressof(x) (kMemoryStart + offsetof(HwrM68EZ328Type, x))

// Macros for reading/writing DragonballEZ registers.

#define READ_REGISTER(reg) _get_reg(&f68EZ328Regs.reg)

#define WRITE_REGISTER(reg, value) _put_reg(&f68EZ328Regs.reg, value)

// Macro for installing DragonballEZ register handlers

#define INSTALL_HANDLER(read, write, reg)                                            \
    this->SetHandler((ReadFunction)&EmRegsEZ::read, (WriteFunction)&EmRegsEZ::write, \
                     addressof(reg), sizeof(f68EZ328Regs.reg))

#endif /* EmRegsEZPrv_h */
