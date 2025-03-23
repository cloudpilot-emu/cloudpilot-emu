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

#ifndef EmRegsVZHandEra330_h
#define EmRegsVZHandEra330_h

#include <memory>

#include "EmHandEra330Defs.h"
#include "EmPalmStructs.h"
#include "EmRegsVZ.h"
#include "EmSPISlaveSD.h"
#include "ExternalStorage.h"

#define keyBitThumbDown 0x1000
#define keyBitThumbUp 0x2000
#define keyBitThumbPush 0x4000

#define PortB_CF_CS 0x0001
#define PortB_RS232_ON 0x0040

#define PortD_KBD_COL0 0x0001
#define PortD_KBD_COL1 0x0002
#define PortD_KBD_COL2 0x0004
#define PortD_KBD_COL3 0x0008
#define PortD_DOCK_BTN 0x0010
#define PortD_CD_IRQ 0x0020
#define PortD_CF_IRQ 0x0040
#define PortD_POWER_FAIL 0x0080

#define PortF_LCD_PWM 0x0001
#define PortF_PEN_IO 0x0002
#define PortF_CLK_OUT 0x0004
#define PortF_A20 0x0008
#define PortF_A21 0x0010
#define PortF_A22 0x0020
#define PortF_A23 0x0040
#define PortF_CPLD_CS_F 0x0080

#define PortG_DTACK 0x0001
#define PortG_A0 0x0002
#define PortG_Unused 0x0004
#define PortG_CPLD_TMS 0x0008
#define PortG_LION 0x0010
#define PortG_Unused2 0x0020

#define PortJ_SPI_TX 0x0001
#define PortJ_SPI_RX 0x0002
#define PortJ_SPI_CLK 0x0004
#define PortJ_AD_CS 0x0008
#define PortJ_232_RX2 0x0010
#define PortJ_232_TX2 0x0020
#define PortJ_232_RTS2 0x0040
#define PortJ_232_CTS2 0x0080

#define PortK_VOL_PWM 0x0001
#define PortK_GP_IN_F 0x0002
#define PortK_LED_GREEN 0x0004
#define PortK_LED_RED 0x0008
#define PortK_CPLD_TDO 0x0010
#define PortK_Unused 0x0020
#define PortK_CPLD_TCK 0x0040
#define PortK_Unused2 0x0020

#define PortM_SD_CLK 0x0001
#define PortM_SD_CE 0x0002
#define PortM_SD_DQMH 0x0004
#define PortM_SD_DQML 0x0008
#define PortM_SD_A10 0x0010
#define PortM_CPLD_TDI 0x0020

class EmRegsVZHandEra330 : public EmRegsVZ {
   public:
    EmRegsVZHandEra330(HandEra330PortManager** fPortManager);
    virtual ~EmRegsVZHandEra330(void);

    virtual void Save(Savestate<ChunkType>&);
    virtual void Save(SavestateProbe<ChunkType>&);
    virtual void Load(SavestateLoader<ChunkType>&);

    virtual void Initialize(void);
    virtual void Dispose(void);
    virtual void Reset(Bool hardwareReset);

    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLineDriverState(EmUARTDeviceType type);
    virtual EmUARTDeviceType GetUARTDevice(int uartNum);
    virtual Bool GetVibrateOn(void);
    virtual uint16 GetLEDState(void);
    virtual int32 GetROMSize(void);

    virtual uint8 GetPortInputValue(int);
    virtual uint8 GetPortInternalValue(int);
    virtual void GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows);
    virtual int32 GetInterruptLevel(void);

    EmSPISlaveSD* GetSPISlaveSD();

    virtual bool SupportsImageInSlot(EmHAL::Slot slot, uint32 blocksTotal);
    virtual void Mount(EmHAL::Slot slot, CardImage& cardImage);
    virtual void Unmount(EmHAL::Slot slot);

   protected:
    virtual EmSPISlave* GetSPI2Slave(void);
    virtual EmSPISlave* GetSPI1Slave(void);
    virtual uint16 ButtonToBits(ButtonEvent::Button btn);

    virtual uint32 GetBootKeyHoldTime();

   private:
    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper);

   private:
    uint8 GetPortD(uint8 result);
    UInt16 PortD;
    UInt16 PortF;
    UInt16 PortG;
    UInt16 PortJ;
    UInt16 PortK;
    UInt16 PortM;
    EmSPISlave* fSPISlaveCurrent;
    HandEra330PortManager PortMgr;

    unique_ptr<EmSPISlaveSD> spiSlaveSD = make_unique<EmSPISlaveSD>();

    uint16 rxFifo[8];
    bool rxFifoEmpty;
    int rxHead;
    int rxTail;
    uint16 txFifo[8];
    bool txFifoEmpty;
    int txHead;
    int txTail;
};

#endif /* EmRegsVZTemp_h */
