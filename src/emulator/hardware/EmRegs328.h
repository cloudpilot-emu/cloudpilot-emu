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

#ifndef EmRegs328_h
#define EmRegs328_h

#include "ButtonEvent.h"
#include "EmEvent.h"
#include "EmHAL.h"             // EmHALHandler
#include "EmRegs.h"            // EmRegs
#include "EmUARTDragonball.h"  // EmUARTDragonball::State

class EmRegs328 : public EmRegs, public EmHALHandler {
    using ButtonEventT = class ButtonEvent;

   public:
    EmRegs328(void);
    virtual ~EmRegs328(void);

    // EmRegs overrides
    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);
    virtual void Dispose(void);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

    // EmHALHandler overrides
    virtual void CycleSlowly(Bool sleeping);

    virtual void ButtonEvent(ButtonEventT evt);
    virtual void TurnSoundOff(void);
    virtual void ResetTimer(void);
    virtual void ResetRTC(void);

    virtual int32 GetInterruptLevel(void);
    virtual int32 GetInterruptBase(void);

    virtual Bool GetLCDScreenOn(void) = 0;
    virtual Bool GetLCDBacklightOn(void) = 0;
    virtual Bool GetLCDHasFrame(void);
    virtual void GetLCDBeginEnd(emuptr&, emuptr&);
    virtual bool CopyLCDFrame(Frame& frame);
    virtual uint16 GetLCD2bitMapping();

    virtual EmUARTDeviceType GetUARTDevice(int uartNum);
    virtual int32 GetDynamicHeapSize(void);
    virtual int32 GetROMSize(void);
    virtual emuptr GetROMBaseAddress(void);
    virtual Bool ChipSelectsConfigured(void);
    virtual int32 GetSystemClockFrequency(void);
    virtual Bool GetAsleep(void);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void PortDataChanged(int, uint8, uint8);

    virtual uint32 CyclesToNextInterrupt(uint64 systemCycles);
    void Cycle(uint64 systemCycles, Bool sleeping);

   private:
    uint32 pllFreqSelRead(emuptr address, int size);
    uint32 portXDataRead(emuptr address, int size);
    uint32 tmr1StatusRead(emuptr address, int size);
    uint32 tmr2StatusRead(emuptr address, int size);
    uint32 uartRead(emuptr address, int size);
    uint32 rtcHourMinSecRead(emuptr address, int size);
    uint32 rtcDayRead(emuptr address, int size);

    void csASelect1Write(emuptr address, int size, uint32 value);
    void csCSelect0Write(emuptr address, int size, uint32 value);
    void csCSelect1Write(emuptr address, int size, uint32 value);
    void intMaskHiWrite(emuptr address, int size, uint32 value);
    void intMaskLoWrite(emuptr address, int size, uint32 value);
    void intStatusHiWrite(emuptr address, int size, uint32 value);
    void portXDataWrite(emuptr address, int size, uint32 value);
    void portDIntReqEnWrite(emuptr address, int size, uint32 value);
    void tmr1StatusWrite(emuptr address, int size, uint32 value);
    void tmr2StatusWrite(emuptr address, int size, uint32 value);
    void wdCounterWrite(emuptr address, int size, uint32 value);
    void spiMasterControlWrite(emuptr address, int size, uint32 value);
    void uartWrite(emuptr address, int size, uint32 value);
    void lcdRegisterWrite(emuptr address, int size, uint32 value);
    void pllRegisterWrite(emuptr address, int size, uint32 value);
    void tmr1RegisterWrite(emuptr address, int size, uint32 value);
    void tmr2RegisterWrite(emuptr address, int size, uint32 value);
    void rtcControlWrite(emuptr address, int size, uint32 value);
    void rtcIntStatusWrite(emuptr address, int size, uint32 value);
    void rtcIntEnableWrite(emuptr address, int size, uint32 value);
    void rtcDayWrite(emuptr address, int size, uint32 value);
    void pwmcWrite(emuptr address, int size, uint32 value);
    void pwmwWrite(emuptr address, int size, uint32 value);
    void pwmpWrite(emuptr address, int size, uint32 value);

   protected:
    void HotSyncEvent(Bool buttonIsDown);

    virtual uint8 GetKeyBits(void);
    virtual uint16 ButtonToBits(ButtonEventT::Button btn);

    virtual void MarkScreen();
    virtual void UnmarkScreen();

   protected:
    void UpdateInterrupts(void);
    void UpdatePortDInterrupts(void);
    void UpdateRTCInterrupts(void);

   protected:
    Bool IDDetectAsserted(void);
    UInt8 GetHardwareID(void);

   protected:
    void UARTStateChanged(Bool sendTxData);
    void UpdateUARTState(Bool refreshRxData);
    void UpdateUARTInterrupts(const EmUARTDragonball::State& state);
    void MarshalUARTState(EmUARTDragonball::State& state);
    void UnmarshalUARTState(const EmUARTDragonball::State& state);

   protected:
    int GetPort(emuptr address);

   private:
    void UpdateTimerTicksPerSecond();
    uint32 Tmr1CyclesToNextInterrupt(uint64 systemCycles);
    uint32 Tmr2CyclesToNextInterrupt(uint64 systemCycles);

    void DispatchPwmChange();

    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper, uint32 version);

   protected:
    HwrM68328Type f68328Regs;
    bool fHotSyncButtonDown;
    uint16 fKeyBits;
    uint16 fLastTmr1Status;
    uint16 fLastTmr2Status;
    uint8 fPortDEdge;
    uint32 fPortDDataCount;

    double tmr1LastProcessedSystemCycles;
    double tmr2LastProcessedSystemCycles;
    double timer1TicksPerSecond;
    double timer2TicksPerSecond;

    int32 lastRtcAlarmCheck;

    bool markScreen{true};
    EmEvent<>::HandleT onMarkScreenCleanHandle;

    EmUARTDragonball* fUART;

    bool pwmActive{false};
    bool afterLoad{false};
};

#endif /* EmRegs328_h */
