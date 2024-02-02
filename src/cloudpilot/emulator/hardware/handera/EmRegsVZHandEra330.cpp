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

#include "EmRegsVZHandEra330.h"

#include "ChunkHelper.h"
#include "EmBankRegs.h"  // EmBankRegs::DisableSubBank
#include "EmCommon.h"
#include "EmRegs330CPLD.h"
#include "EmRegsVZPrv.h"
#include "EmSPISlave330Current.h"
#include "EmSPISlaveADS784x.h"  // EmSPISlaveADS784x
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

#pragma mark -

const int kNumButtonRows = 4;
const int kNumButtonCols = 4;

const uint16 kButtonMap[kNumButtonRows][kNumButtonCols] = {
    {keyBitHard1, keyBitHard2, keyBitHard3, keyBitHard4},
    {keyBitPageUp, keyBitPageDown, 0, keyBitThumbDown},
    {keyBitPower, 0, keyBitContrast, keyBitThumbPush},
    {0, 0, 0, keyBitThumbUp},
};

namespace {
    constexpr uint32 SAVESTATE_VERSION = 1;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::EmRegsVZHandEra330
// ---------------------------------------------------------------------------

EmRegsVZHandEra330::EmRegsVZHandEra330(HandEra330PortManager** fPortManager)
    : fSPISlaveCurrent(new EmSPISlave330Current()) {
    fSPISlaveADC = new EmSPISlaveADS784x(kChannelSet2);

    PortD = PortD_DOCK_BTN | PortD_CD_IRQ | PortD_CF_IRQ | PortD_POWER_FAIL;
    PortF = PortF_PEN_IO | PortF_CPLD_CS_F;
    PortG = PortG_DTACK | PortG_A0 | PortG_Unused |
            // PortG_LION |
            PortG_Unused2;
    PortJ = PortJ_AD_CS;
    PortK = PortK_LED_GREEN | PortK_LED_RED | PortK_CPLD_TDO | PortK_CPLD_TCK;
    PortM = PortM_CPLD_TDI;
    *fPortManager = &PortMgr;
    PortMgr.Keys.Row[0] = 1;
    PortMgr.Keys.Row[1] = 1;
    PortMgr.Keys.Row[2] = 1;
    PortMgr.Keys.Row[3] = 1;
    PortMgr.LCDOn = false;
    PortMgr.BacklightOn = false;
    PortMgr.IRPortOn = false;
    PortMgr.CFBus.bEnabled = false;
    PortMgr.CFBus.Width = kCFBusWidth16;
    PortMgr.CFBus.bSwapped = false;
    PortMgr.CFInserted = false;
    PortMgr.SDInserted = false;
    PortMgr.pendingIRQ2 = false;
    PortMgr.SDChipSelect = false;
    PortMgr.PowerConnected = false;

    // make sure SPI1 fifos are empty
    rxHead = rxTail = txHead = txTail = 0;
    txFifoEmpty = true;
    rxFifoEmpty = true;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::~EmRegsVZHandEra330
// ---------------------------------------------------------------------------

EmRegsVZHandEra330::~EmRegsVZHandEra330(void) { delete fSPISlaveADC; }

void EmRegsVZHandEra330::Load(SavestateLoader& loader) {
    EmRegsVZ::Load(loader);
    fSPISlaveCurrent->Load(loader);

    Chunk* chunk = loader.GetChunk(ChunkType::regsVZHandera330);
    if (!chunk) {
        logging::printf("unable to restore RegsVZHandEra330: missing savestate\n");
        loader.NotifyError();

        return;
    }

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore RegsVZHandEra330: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

void EmRegsVZHandEra330::Save(Savestate& savestate) { DoSave(savestate); }

void EmRegsVZHandEra330::Save(SavestateProbe& savestateProbe) { DoSave(savestateProbe); }

template <typename T>
void EmRegsVZHandEra330::DoSave(T& savestate) {
    EmRegsVZ::Save(savestate);
    fSPISlaveCurrent->Save(savestate);

    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsVZHandera330);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

template <typename T>
void EmRegsVZHandEra330::DoSaveLoad(T& helper) {
    helper.Do(typename T::Pack16() << PortD << PortF)
        .Do(typename T::Pack16() << PortG << PortJ)
        .Do(typename T::Pack16() << PortK << PortM)
        .Do(typename T::Pack16() << PortMgr.Keys.Row[0] << PortMgr.Keys.Row[1])
        .Do(typename T::Pack16() << PortMgr.Keys.Row[2] << PortMgr.Keys.Row[3])
        .Do(typename T::BoolPack()
            << PortMgr.LCDOn << PortMgr.BacklightOn << PortMgr.IRPortOn << PortMgr.SenseCurrent
            << PortMgr.pendingIRQ2 << PortMgr.CFInserted << PortMgr.SDInserted
            << PortMgr.PowerConnected << PortMgr.SDChipSelect)
        .Do32(rxHead)
        .Do32(rxTail)
        .Do32(txHead)
        .Do32(txTail)
        .Do(typename T::BoolPack() << rxFifoEmpty << txFifoEmpty);

    for (int i = 0; i < 8; i++) helper.Do(typename T::Pack16() << rxFifo[i] << txFifo[i]);
}

void EmRegsVZHandEra330::Initialize(void) { EmRegsVZ::Initialize(); }

void EmRegsVZHandEra330::Dispose(void) { EmRegsVZ::Dispose(); }

void EmRegsVZHandEra330::Reset(Bool hardwareReset) {
    EmRegsVZ::Reset(hardwareReset);
    spiSlaveSD->Reset();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsVZHandEra330::GetLCDScreenOn(void) {
    // TRG LCD on is determined by LCD contrast on in the CPLD
    return PortMgr.LCDOn;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsVZHandEra330::GetLCDBacklightOn(void) {
    // TRG CPLD controls the backlight
    return PortMgr.BacklightOn;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmRegsVZHandEra330::GetLineDriverState(EmUARTDeviceType type) {
    if (type == kUARTSerial) return (READ_REGISTER(portBData) & PortB_RS232_ON) != 0;

    if (type == kUARTIR) return PortMgr.IRPortOn;

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetUARTDevice
// ---------------------------------------------------------------------------
// Return what sort of device is hooked up to the given UART.

EmUARTDeviceType EmRegsVZHandEra330::GetUARTDevice(int /*uartNum*/) {
    Bool serEnabled = this->GetLineDriverState(kUARTSerial);
    Bool irEnabled = this->GetLineDriverState(kUARTIR);

    // !!! Which UART are they using?

    //	if (uartNum == ???)
    {
        if (serEnabled) return kUARTSerial;

        if (irEnabled) return kUARTIR;
    }

    return kUARTNone;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmRegsVZHandEra330::GetVibrateOn(void) { return false; }

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetInterruptLevel
// ---------------------------------------------------------------------------

int32 EmRegsVZHandEra330::GetInterruptLevel(void) {
    int32 retval;

    retval = EmRegsVZ::GetInterruptLevel();

    if (PortMgr.pendingIRQ2 && (retval < 2)) retval = 2;

    return retval;
}

EmSPISlaveSD* EmRegsVZHandEra330::GetSPISlaveSD() { return spiSlaveSD.get(); }

bool EmRegsVZHandEra330::SupportsImageInSlot(EmHAL::Slot slot, uint32 blocksTotal) {
    return slot == EmHAL::Slot::sdcard && EmSPISlaveSD::IsSizeRepresentable(blocksTotal);
}

void EmRegsVZHandEra330::Mount(EmHAL::Slot slot, CardImage& cardImage) {
    PortMgr.pendingIRQ2 = true;
}

void EmRegsVZHandEra330::Unmount(EmHAL::Slot slot) { PortMgr.pendingIRQ2 = true; }

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmRegsVZHandEra330::GetLEDState(void) {
    uint16 result = kLEDOff;
    uint8 portKData = READ_REGISTER(portKData);

    if ((portKData & PortK_LED_GREEN) == 0) result |= kLEDGreen;

    if ((portKData & PortK_LED_RED) == 0) result |= kLEDRed;

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetPortD
// ---------------------------------------------------------------------------

uint8 EmRegsVZHandEra330::GetPortD(uint8 result) {
    return result | PortD_DOCK_BTN | PortD_CD_IRQ | PortD_CF_IRQ | PortD_POWER_FAIL;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsVZHandEra330::GetPortInputValue(int port) {
    uint8 result = EmRegsVZ::GetPortInputValue(port);

    switch (port) {
        case 'D':
            result = GetPortD(result);
            break;
        case 'F':
            result = PortF;
            break;
        case 'G':
            result = PortG;
            break;
        case 'J':
            result = PortJ;
            break;
        case 'K':
            result = PortK;
            break;
        case 'M':
            result = PortM;
            break;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsVZHandEra330::GetPortInternalValue(int port) {
    uint8 result = EmRegsVZ::GetPortInternalValue(port);

    switch (port) {
        case 'D':
            result = GetPortD(result);
            break;
        case 'F':
            result = PortF;
            break;
        case 'G':
            result = PortG;
            break;
        case 'J':
            result = PortJ;
            break;
        case 'K':
            result = PortK;
            break;
        case 'M':
            result = PortM;
            break;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetKeyInfo
// ---------------------------------------------------------------------------

void EmRegsVZHandEra330::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    *numRows = kNumButtonRows;
    *numCols = kNumButtonCols;

    memcpy(keyMap, kButtonMap, sizeof(kButtonMap));

    // Determine what row is being asked for.
    rows[0] = PortMgr.Keys.Row[0];
    rows[1] = PortMgr.Keys.Row[1];
    rows[2] = PortMgr.Keys.Row[2];
    rows[3] = PortMgr.Keys.Row[3];
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVZHandEra330::GetSPI2Slave(void) {
    if ((READ_REGISTER(portJData) & PortJ_AD_CS) == 0) {
        if (PortMgr.SenseCurrent)
            return fSPISlaveCurrent;
        else
            return fSPISlaveADC;
    }

    return NULL;
}

EmSPISlave* EmRegsVZHandEra330::GetSPI1Slave(void) { return spiSlaveSD.get(); }

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetROMSize
// ---------------------------------------------------------------------------

int32 EmRegsVZHandEra330::GetROMSize(void) { return (2 * 1024 * 1024); }

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::ButtonToBits
// ---------------------------------------------------------------------------

uint16 EmRegsVZHandEra330::ButtonToBits(ButtonEvent::Button btn) {
    uint16 bitNumber = 0;

    switch (btn) {
        default:
            bitNumber = EmRegsVZ::ButtonToBits(btn);
            break;

            // Borrow some skin elements from Symbol for our Thumb wheel
            // NOTE: Borrowed Palm's contrast for our aux, so above case already handles it.
        case ButtonEvent::Button::wheelUp:
            bitNumber = keyBitThumbUp;
            break;
        case ButtonEvent::Button::wheelPush:
            bitNumber = keyBitThumbPush;
            break;
        case ButtonEvent::Button::wheelDown:
            bitNumber = keyBitThumbDown;
            break;
    }

    return bitNumber;
}

uint32 EmRegsVZHandEra330::GetBootKeyHoldTime() { return 25; }
