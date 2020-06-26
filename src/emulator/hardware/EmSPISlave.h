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

#ifndef EmSPISlave_h
#define EmSPISlave_h

#include "EmCommon.h"

class EmSPISlave {
   public:
    EmSPISlave(void);
    virtual ~EmSPISlave(void);

    virtual uint16 DoExchange(uint16 control, uint16 data) = 0;
    virtual void Enable(void);
    virtual void Disable(void);
};

#endif  // EmSPISlave_h
