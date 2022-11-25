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

#ifndef EmRegs328Prv_h
#define EmRegs328Prv_h

#include "EmRegsPrv.h"

// Location and range of registers

const uint32 kMemoryStart = 0xFFFFF000;
const uint32 kMemorySize = sizeof(HwrM68328Type);

// Macro to return the Dragonball address of the specified register

#define db_addressof(x) (kMemoryStart + offsetof(HwrM68328Type, x))

// Macros for reading/writing Dragonball registers.

#define READ_REGISTER(reg) _get_reg(&f68328Regs.reg)

#define WRITE_REGISTER(reg, value) _put_reg(&f68328Regs.reg, value)

// Macro for installing Dragonball register handlers

#define INSTALL_HANDLER(read, write, reg)                                              \
    this->SetHandler((ReadFunction)&EmRegs328::read, (WriteFunction)&EmRegs328::write, \
                     db_addressof(reg), sizeof(f68328Regs.reg))

#endif /* EmRegs328Prv_h */
