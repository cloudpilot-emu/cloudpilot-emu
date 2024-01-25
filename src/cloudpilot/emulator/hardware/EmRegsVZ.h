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

#ifndef EmRegsVZ_h
#define EmRegsVZ_h

#include "ButtonEvent.h"
#include "EmEvent.h"
#include "EmHAL.h"             // EmHALHandler
#include "EmRegs.h"            // EmRegs
#include "EmUARTDragonball.h"  // EmUARTDragonball::State
#include "Fifo.h"

class EmSPISlave;

class EmRegsVZ : public EmRegs, public EmHALHandler {
   public:
    EmRegsVZ(void);
    virtual ~EmRegsVZ(void);

    // EmRegs overrides
    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Dispose(void);

    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

    // EmHALHandler overrides
    virtual void CycleSlowly(Bool sleeping);

    virtual void DispatchButtonEvent(ButtonEvent evt);
    virtual void TurnSoundOff(void);
    virtual void ResetTimer(void);
    virtual void ResetRTC(void);

    virtual int32 GetInterruptLevel(void);
    virtual int32 GetInterruptBase(void);

    virtual Bool GetLCDScreenOn(void) = 0;
    virtual Bool GetLCDBacklightOn(void) = 0;
    virtual Bool GetLCDHasFrame(void);
    virtual bool CopyLCDFrame(Frame& frame, bool fullRefresh);
    virtual uint16 GetLCD2bitMapping();

    virtual int32 GetDynamicHeapSize(void);
    virtual int32 GetROMSize(void);
    virtual emuptr GetROMBaseAddress(void);
    virtual Bool ChipSelectsConfigured(void);
    virtual int32 GetSystemClockFrequency(void);
    virtual Bool GetAsleep(void);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void PortDataChanged(int, uint8, uint8);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) = 0;

    virtual uint32 CyclesToNextInterrupt(uint64 systemCycles);
    inline void Cycle(uint64 systemCycles, Bool sleeping);

   protected:
    virtual uint8 GetKeyBits(void);
    virtual uint16 ButtonToBits(ButtonEvent::Button btn);
    virtual EmSPISlave* GetSPI2Slave(void);
    virtual EmSPISlave* GetSPI1Slave(void);

    virtual void MarkScreen();
    virtual void UnmarkScreen();
    virtual void MarkScreenDirty();

    virtual void ApplySdctl();
    virtual void portDIntReqEnWrite(emuptr address, int size, uint32 value);

    void HotSyncEvent(Bool buttonIsDown);
    void UpdateInterrupts(void);
    void UpdatePortDInterrupts(void);
    void UpdateRTCInterrupts(void);
    void UpdateIRQ2(uint8 oldBit);
    void UpdateIRQ3(uint8 oldBit);

    void UARTStateChanged(Bool sendTxData, int uartNum);
    void UpdateUARTState(Bool refreshRxData, int uartNum);
    void UpdateUARTInterrupts(const EmUARTDragonball::State& state, int uartNum);
    void MarshalUARTState(EmUARTDragonball::State& state, int uartNum);
    void UnmarshalUARTState(const EmUARTDragonball::State& state, int uartNum);

    int GetPort(emuptr address);

    virtual void Spi1AssertSlaveSelect();
    virtual void Spi1DeassertSlaveSelect();

    virtual uint32 GetBootKeyHoldTime();

   private:
    uint32 pllFreqSelRead(emuptr address, int size);
    uint32 portXDataRead(emuptr address, int size);
    uint32 tmr1StatusRead(emuptr address, int size);
    uint32 tmr2StatusRead(emuptr address, int size);
    uint32 tmrRegisterRead(emuptr address, int size);
    uint32 uart1Read(emuptr address, int size);
    uint32 uart2Read(emuptr address, int size);
    uint32 rtcHourMinSecRead(emuptr address, int size);

    void sdctlWrite(emuptr address, int size, uint32 value);
    void csControl1Write(emuptr address, int size, uint32 value);
    void csASelectWrite(emuptr address, int size, uint32 value);
    void csDSelectWrite(emuptr address, int size, uint32 value);
    void intMaskHiWrite(emuptr address, int size, uint32 value);
    void intMaskLoWrite(emuptr address, int size, uint32 value);
    void intStatusHiWrite(emuptr address, int size, uint32 value);
    void portXDataWrite(emuptr address, int size, uint32 value);
    void pllRegisterWrite(emuptr address, int size, uint32 value);
    void tmr1StatusWrite(emuptr address, int size, uint32 value);
    void tmr2StatusWrite(emuptr address, int size, uint32 value);
    void tmrRegisterWrite(emuptr address, int size, uint32 value);
    void spiMasterControlWrite(emuptr address, int size, uint32 value);
    void uart1Write(emuptr address, int size, uint32 value);
    void uart2Write(emuptr address, int size, uint32 value);
    void lcdRegisterWrite(emuptr address, int size, uint32 value);
    void rtcControlWrite(emuptr address, int size, uint32 value);
    void rtcIntStatusWrite(emuptr address, int size, uint32 value);
    void rtcIntEnableWrite(emuptr address, int size, uint32 value);
    void pwmc1Write(emuptr address, int size, uint32 value);
    void pwms1Write(emuptr address, int size, uint32 value);
    void pwmp1Write(emuptr address, int size, uint32 value);

    uint32 spiRxDRead(emuptr address, int size);
    void spiTxDWrite(emuptr address, int size, uint32 value);
    void spiIntCSWrite(emuptr address, int size, uint32 value);
    uint32 spiIntCSRead(emuptr address, int size);
    void spiCont1Write(emuptr address, int size, uint32 value);
    uint32 spiTestRead(emuptr address, int size);

    void UpdateTimers();
    void DispatchPwmChange();
    void HandleDayRollover();

    void Spi1TransmitWord();
    void Spi1UpdateInterrupts();

    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper, uint32 version);

   protected:
    HwrM68VZ328Type f68VZ328Regs;
    bool fHotSyncButtonDown;
    uint16 fKeyBits;
    uint16 fLastTmr1Status;
    uint16 fLastTmr2Status;
    uint8 fPortDEdge;
    uint32 fPortDDataCount;

    bool markScreenScheduled{true};
    bool screenMarked{false};
    EmEvent<>::HandleT onMarkScreenCleanHandle;
    EmEvent<>::HandleT onDayRolloverHandle;

    EmUARTDragonball* fUART[2];

    double tmr1LastProcessedSystemCycles;
    double tmr2LastProcessedSystemCycles;
    uint64 nextTimerEventAfterCycle;
    uint64 systemCycles;

    int32 lastRtcAlarmCheck{-1};

    bool pwmActive{false};
    bool afterLoad{false};
    bool powerOffCached{false};

    Fifo<uint16> spi1RxFifo{8};
    Fifo<uint16> spi1TxFifo{8};
    int32 spi1Countdown{0};
    uint16 spi1TxWordPending{0};
    bool spi1TransferInProgress{false};

    EmSPISlave* fSPISlaveADC{nullptr};
};

class EmRegsVZNoScreen : public EmRegsVZ {
   public:
    EmRegsVZNoScreen() = default;

    virtual bool CopyLCDFrame(Frame& frame, bool fullRefresh);
    virtual uint16 GetLCD2bitMapping();

   protected:
    virtual void MarkScreen();
    virtual void UnmarkScreen();
    virtual void MarkScreenDirty();
};

#endif /* EmRegsVZ_h */
