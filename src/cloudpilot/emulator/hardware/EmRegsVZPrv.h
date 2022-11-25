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

#ifndef EmRegsVZPrv_h
#define EmRegsVZPrv_h

#include "EmRegsPrv.h"

// Location and range of registers

const uint32 kMemoryStart = 0xFFFFF000;
const uint32 kMemorySize = sizeof(HwrM68VZ328Type);

// Macro to return the DragonballVZ address of the specified register

#define db_addressof(x) (kMemoryStart + offsetof(HwrM68VZ328Type, x))

// Macros for reading/writing DragonballVZ registers.

#define READ_REGISTER(reg) _get_reg(&f68VZ328Regs.reg)

#define WRITE_REGISTER(reg, value) _put_reg(&f68VZ328Regs.reg, value)

// Macro for installing DragonballVZ register handlers

#define INSTALL_HANDLER(read, write, reg)                                            \
    this->SetHandler((ReadFunction)&EmRegsVZ::read, (WriteFunction)&EmRegsVZ::write, \
                     db_addressof(reg), sizeof(f68VZ328Regs.reg))

#endif /* EmRegsVZPrv_h */
