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

#ifndef EmHAL_h
#define EmHAL_h

#include "ButtonEvent.h"
#include "EmCommon.h"

class EmHAL;
class EmPixMap;
struct Frame;

enum { kLEDOff = 0x00, kLEDGreen = 0x01, kLEDRed = 0x02 };

enum EmUARTDeviceType {
    kUARTBegin = 0,

    kUARTSerial = kUARTBegin,
    kUARTIR,
    kUARTBluetooth,
    kUARTMystery,
    kUARTNone,

    kUARTEnd
};

DEFINE_SCALAR_MODIFIERS(EmUARTDeviceType)

class EmHALHandler {
    using ButtonEventT = ButtonEvent;

   public:
    EmHALHandler(void);
    virtual ~EmHALHandler(void);

    virtual void Cycle(Bool sleeping);
    virtual void CycleSlowly(Bool sleeping);

    virtual void ButtonEvent(ButtonEventT evt);
    virtual void TurnSoundOff(void);
    virtual void ResetTimer(void);
    virtual void ResetRTC(void);

    virtual int32 GetInterruptLevel(void);
    virtual int32 GetInterruptBase(void);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLCDHasFrame(void);
    virtual void GetLCDBeginEnd(emuptr&, emuptr&);
    virtual bool CopyLCDFrame(Frame& frame);

    virtual int32 GetDynamicHeapSize(void);
    virtual int32 GetROMSize(void);
    virtual emuptr GetROMBaseAddress(void);
    virtual Bool ChipSelectsConfigured(void);
    virtual int32 GetSystemClockFrequency(void);
    virtual Bool GetCanStop(void);
    virtual Bool GetAsleep(void);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void PortDataChanged(int, uint8, uint8);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

    virtual Bool GetLineDriverState(EmUARTDeviceType);
    virtual EmUARTDeviceType GetUARTDevice(int uartNum);

    virtual Bool GetDTR(int uartNum);

    virtual Bool GetVibrateOn(void);
    virtual uint16 GetLEDState(void);

   protected:
    EmHALHandler* GetNextHandler(void) { return fNextHandler; }

   private:
    EmHALHandler* fNextHandler;
    EmHALHandler* fPrevHandler;

    friend class EmHAL;
};

class EmHAL {
    using ButtonEventT = ButtonEvent;

   public:
    static void AddHandler(EmHALHandler*);
    static void RemoveHandler(EmHALHandler*);
    static void EnsureCoverage(void);

    static void Cycle(Bool sleeping);
    static void CycleSlowly(Bool sleeping);

    static void ButtonEvent(ButtonEventT evt);
    static void TurnSoundOff(void);
    static void ResetTimer(void);
    static void ResetRTC(void);

    static int32 GetInterruptLevel(void);
    static int32 GetInterruptBase(void);

    static Bool GetLCDScreenOn(void);
    static Bool GetLCDBacklightOn(void);
    static Bool GetLCDHasFrame(void);
    static void GetLCDBeginEnd(emuptr&, emuptr&);
    static bool CopyLCDFrame(Frame& frame);

    static int32 GetDynamicHeapSize(void);
    static int32 GetROMSize(void);
    static emuptr GetROMBaseAddress(void);
    static Bool ChipSelectsConfigured(void);
    static int32 GetSystemClockFrequency(void);
    static Bool GetCanStop(void);
    static Bool GetAsleep(void);

    static uint8 GetPortInputValue(int);
    static uint8 GetPortInternalValue(int);
    static void PortDataChanged(int, uint8, uint8);
    static void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);

    static void LineDriverChanged(EmUARTDeviceType);
    static Bool GetLineDriverState(EmUARTDeviceType);
    static EmUARTDeviceType GetUARTDevice(int uartNum);

    static void GetLineDriverStates(Bool* states);
    static void CompareLineDriverStates(const Bool* oldStates);

    static Bool GetDTR(int uartNum);
    static void DTRChanged(int uartNum);

    static Bool GetVibrateOn(void);
    static uint16 GetLEDState(void);

   private:
    static EmHALHandler* GetRootHandler(void) { return fgRootHandler; }
    static EmHALHandler* fgRootHandler;
};

inline void EmHAL::Cycle(Bool sleeping) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->Cycle(sleeping);
}

#endif /* EmHAL_h */
