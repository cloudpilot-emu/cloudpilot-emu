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

#ifndef _EM_REGS_SZ_PRV_H_
#define _EM_REGS_SZ_PRV_H_

#include "EmRegsPrv.h"

// Location and range of registers

const uint32 kMemoryStart = 0xFFFE0000L;  // different than in previous chips!!!
const uint32 kMemorySize = sizeof(HwrM68SZ328Type);

// Macro to return the DragonballSZ address of the specified register

#define db_addressof(x) (kMemoryStart + offsetof(HwrM68SZ328Type, x))

// Macros for reading/writing DragonballSZ registers.

#define READ_REGISTER(reg) _get_reg(&f68SZ328Regs.reg)

#define WRITE_REGISTER(reg, value) _put_reg(&f68SZ328Regs.reg, value)

// Macro for installing DragonballSZ register handlers

#define INSTALL_HANDLER(read, write, reg)                                            \
    this->SetHandler((ReadFunction)&EmRegsSZ::read, (WriteFunction)&EmRegsSZ::write, \
                     db_addressof(reg), sizeof(f68SZ328Regs.reg))

#endif  // _EM_REGS_SZ_PRV_H_
