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

#ifndef EmSPISlaveADS784x_h
#define EmSPISlaveADS784x_h

#include "EmSPISlave.h"  // EmSPISlave

enum EmADSChannelType {
    kChannelUnused,
    kChannelPenX,
    kChannelPenY,
    kChannelBattery7843,
    kChannelBattery7846,
    kChannelDockSerial,
    kChannelDockTwister,
    kChannelTemp0,
    kChannelTemp1
};

#define kChannelSet1                                                                  \
    kChannelTemp0, kChannelPenY, kChannelBattery7846, kChannelUnused, kChannelUnused, \
        kChannelPenX, kChannelDockTwister, kChannelTemp1

#define kChannelSet2                                                                   \
    kChannelUnused, kChannelPenY, kChannelBattery7843, kChannelUnused, kChannelUnused, \
        kChannelPenX, kChannelDockSerial, kChannelUnused

#define kChannelSet3                                                                   \
    kChannelUnused, kChannelPenY, kChannelBattery7843, kChannelUnused, kChannelUnused, \
        kChannelPenX, kChannelDockTwister, kChannelUnused

class EmSPISlaveADS784x : public EmSPISlave {
   public:
    EmSPISlaveADS784x(EmADSChannelType ch0, EmADSChannelType ch1, EmADSChannelType ch2,
                      EmADSChannelType ch3, EmADSChannelType ch4, EmADSChannelType ch5,
                      EmADSChannelType ch6, EmADSChannelType ch7);
    virtual ~EmSPISlaveADS784x(void);

    virtual uint16 DoExchange(uint16 control, uint16 data);

   protected:
    void ProcessCommand(uint8);
    void LoadPendingConversion(void);

   private:
    EmADSChannelType fChannelUse[8];

    uint32 fBitBufferIn;
    uint16 fBitBufferOut;
    int fNumBitsIn;
    uint16 fPendingResult;
    Bool fHavePending;

    int fCommandBitsSeen;
};

#endif  // EmSPISlaveADS784x_h
