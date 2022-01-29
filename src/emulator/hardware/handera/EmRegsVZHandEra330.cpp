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
#include "EmTRGSD.h"
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
    : EmRegsVZ(),

      fSPISlaveCurrent(new EmSPISlave330Current()) {
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

void EmRegsVZHandEra330::Initialize(void) {
    EmRegsVZ::Initialize();

    SD.Initialize();
}

void EmRegsVZHandEra330::Dispose(void) {
    EmRegsVZ::Dispose();

    SD.Dispose();
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

    // It's probably an error to have them both enabled at the same
    // time.  !!! TBD: make this an error message.

    EmAssert(!(serEnabled && irEnabled));

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

EmSPISlave* EmRegsVZHandEra330::GetSPISlave(void) {
    if ((READ_REGISTER(portJData) & PortJ_AD_CS) == 0) {
        if (PortMgr.SenseCurrent)
            return fSPISlaveCurrent;
        else
            return fSPISlaveADC;
    }

    return NULL;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::GetROMSize
// ---------------------------------------------------------------------------

int32 EmRegsVZHandEra330::GetROMSize(void) { return (2 * 1024 * 1024); }

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::ButtonToBits
// ---------------------------------------------------------------------------

uint16 EmRegsVZHandEra330::ButtonToBits(ButtonEventT::Button btn) {
    uint16 bitNumber = 0;

    switch (btn) {
        default:
            bitNumber = EmRegsVZ::ButtonToBits(btn);
            break;

            // Borrow some skin elements from Symbol for our Thumb wheel
            // NOTE: Borrowed Palm's contrast for our aux, so above case already handles it.
        case ButtonEventT::Button::wheelUp:
            bitNumber = keyBitThumbUp;
            break;
        case ButtonEventT::Button::wheelPush:
            bitNumber = keyBitThumbPush;
            break;
        case ButtonEventT::Button::wheelDown:
            bitNumber = keyBitThumbDown;
            break;
    }

    return bitNumber;
}

/**********************************************************************************
 * SD support:
 * HandEra 330 SD is attached to the DragonballVZ SPI1 which is otherwise unused.
 **********************************************************************************/
uint32 EmRegsVZHandEra330::spiRxDRead(emuptr /* address */, int /* size */) {
    uint32 retval;

    // there is an 8 word fifo here, read back the first in.

    if ((rxHead == rxTail) && rxFifoEmpty) {
        // invalid read, fifo empty
        return 0;
    }

    retval = rxFifo[rxTail++];
    if (rxTail == 8) rxTail = 0;
    if (rxTail == rxHead) txFifoEmpty = true;
    return retval;
}

void EmRegsVZHandEra330::spiTxDWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register. (so reading the last value back works)
    EmRegsVZ::StdWrite(address, size, value);

    if (!txFifoEmpty && (txHead == txTail)) {
        // fifo full, do nothing
        return;
    }

    txFifoEmpty = false;

    // there is an 8 word fifo here.
    txFifo[txHead++] = value;
    if (txHead == 8) txHead = 0;
}

void EmRegsVZHandEra330::spiCont1Write(emuptr address, int size, uint32 value) {
    // if we were not enabled before, flush fifos
    if ((value & hwrVZ328SPIMControlEnable) == 0) {
        txTail = txHead;
        rxTail = rxHead;
        txFifoEmpty = rxFifoEmpty = true;
    }

    // Do a standard update of the register.
    EmRegsVZ::StdWrite(address, size, value);

    // Get the current value.
    uint16 spiCont1 = READ_REGISTER(spiCont1);

// Check to see if data exchange and enable are enabled.
#define BIT_MASK (hwrVZ328SPIMControlExchange | hwrVZ328SPIMControlEnable)
    if ((spiCont1 & BIT_MASK) == BIT_MASK) {
        // do the exchange
        if (!txFifoEmpty) {
            // is SD chip selected?
            if (PortMgr.SDChipSelect) {
                uint16 rxData, txData;

                do {
                    txData = txFifo[txTail++];
                    if (txTail == 8) txTail = 0;
                    SD.ExchangeBits(txData, &rxData, (spiCont1 & 0x000f) + 1);
                    rxFifo[rxHead++] = rxData;
                    if (rxHead == 8) rxHead = 0;
                } while (txTail != txHead);
                txFifoEmpty = true;
                rxFifoEmpty = false;
            } else {
                // nothing else is connected here, just stuff the rx fifo and flush the tx fifo
                do {
                    txTail++;
                    if (txTail == 8) txTail = 0;
                    rxFifo[rxHead++] = 0xff;
                    if (rxHead == 8) rxHead = 0;
                } while (txTail != txHead);
                rxFifoEmpty = false;
                txFifoEmpty = true;
            }
        }

        // Clear the exchange bit.
        spiCont1 &= ~hwrVZ328SPIMControlExchange;
        WRITE_REGISTER(spiCont1, spiCont1);
    }
}

uint32 EmRegsVZHandEra330::spiCont1Read(emuptr /* address */, int /* size */) { return 0; }

void EmRegsVZHandEra330::spiIntCSWrite(emuptr /* address */, int /* size */, uint32 /* value */) {}

uint32 EmRegsVZHandEra330::spiIntCSRead(emuptr /* address */, int /* size */) { return 0; }

// ---------------------------------------------------------------------------
//		� EmRegsVZHandEra330::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsVZHandEra330::SetSubBankHandlers(void) {
    // HwrM68VZ328Type   regs;

    EmRegsVZ::SetSubBankHandlers();

    // SD support
    this->SetHandler((ReadFunction)&EmRegsVZHandEra330::spiRxDRead,
                     (WriteFunction)&EmRegsVZHandEra330::StdWrite, db_addressof(spiRxD),
                     sizeof(UInt16));
    this->SetHandler((ReadFunction)&EmRegsVZHandEra330::StdRead,
                     (WriteFunction)&EmRegsVZHandEra330::spiTxDWrite, db_addressof(spiTxD),
                     sizeof(UInt16));
    this->SetHandler((ReadFunction)&EmRegsVZHandEra330::StdRead,
                     (WriteFunction)&EmRegsVZHandEra330::spiCont1Write, db_addressof(spiCont1),
                     sizeof(UInt16));
}
