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
#ifndef _EmTRGSD_h_
#define _EmTRGSD_h_

#include "EmTRGDiskIO.h"

/************************************************************************
 * This class handles the SPI SD controller interface used in the
 * HandEra 330.
 ************************************************************************/

typedef enum {
    sdBusGetCmdState,
    sdBusSendResponseState,
    sdBusGetDataState,
    sdBusSendDataState
} sdBusStateType;

class EmTRGSD {
   private:
    uint8 cmdBuffer[6];
    int cmdIndex;
    uint8 responseBuffer[2];
    int responseIndex;
    int responseSize;
    uint8 dataBuffer[520];
    int dataIndex;
    int dataSize;
    uint32 sectorNum;
    sdBusStateType sdBusState;
    sdBusStateType sdBusNextState;  // next state after sending response

    EmTRGDiskIO DiskIO;

    void ProcessCommand(void);
    void CompleteCommand(void);

   public:
    EmTRGSD();
    ~EmTRGSD();

    void Initialize(void);
    void Dispose(void);

    void ExchangeBits(uint16 txData, uint16 *rxData, uint16 Bits);
};

#endif
