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

#ifndef EmRegsVZTemp_h
#define EmRegsVZTemp_h

#include "EmPalmStructs.h"  // EmProxyHwrPalmI705PLDType
#include "EmRegsVZ.h"

class EmRegsVZPalmI705 : public EmRegsVZ {
   public:
    EmRegsVZPalmI705(void);
    virtual ~EmRegsVZPalmI705(void);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLineDriverState(EmUARTDeviceType type);
    virtual EmUARTDeviceType GetUARTDevice(int uartNum);
    virtual Bool GetVibrateOn(void);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

   private:
    EmSPISlave* fSPISlaveADC;
};

class EmRegsVZPalmM125 : public EmRegsVZ {
   public:
    EmRegsVZPalmM125(void);
    virtual ~EmRegsVZPalmM125(void);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLineDriverState(EmUARTDeviceType type);
    virtual EmUARTDeviceType GetUARTDevice(int uartNum);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

   private:
    EmSPISlave* fSPISlaveADC;
};

class EmRegsVZPalmM130 : public EmRegsVZ {
   public:
    EmRegsVZPalmM130(void);
    virtual ~EmRegsVZPalmM130(void);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLineDriverState(EmUARTDeviceType type);
    virtual EmUARTDeviceType GetUARTDevice(int uartNum);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

   protected:
    virtual EmSPISlave* GetSPISlave(void);

   private:
    EmSPISlave* fSPISlaveADC;
};

#if 0
class EmRegsPLDPalmI705 : public EmRegs, public EmHALHandler {
   public:
    EmRegsPLDPalmI705(emuptr);
    virtual ~EmRegsPLDPalmI705(void);

    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Save(SessionFile&);
    virtual void Load(SessionFile&);
    virtual void Dispose(void);

    virtual uint16 GetLEDState(void);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

   private:
    uint32 low_battRead(emuptr address, int size);
    uint32 usb_intRead(emuptr address, int size);
    uint32 bullet_detRead(emuptr address, int size);

    emuptr fBaseAddr;
    EmProxyHwrPalmI705PLDType fRegs;
};

typedef EmRegsPLDPalmI705 EmRegsPLDJabba;
#endif

#endif /* EmRegsVZTemp_h */
