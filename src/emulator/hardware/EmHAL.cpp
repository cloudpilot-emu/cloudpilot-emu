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

#include "EmHAL.h"

#include "EmCommon.h"
#include "Logging.h"

EmHALHandler* EmHAL::fgRootHandler;

#define PRINTF \
    if (!0)    \
        ;      \
    else       \
        logging::printf

using ButtonEventT = ButtonEvent;

/***********************************************************************
 *
 * FUNCTION:	PrvHandlePortOpenErrors
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	nothing, but displays one of two warnings if its cases
 *				are tripped.
 *
 ***********************************************************************/
#if 0  // CSTODO
namespace {
	void PrvHandlePortOpenErrors(ErrCode err, string errString) {
		switch (err) {
			// access denied, comm port, on Win32
			case 5:
				Errors::ReportErrCommPort(errString);
				break;

			// comm port error on Mac
			case -97:
				Errors::ReportErrCommPort(errString);
				break;
		}
	}
}
#endif

EmEvent<> EmHAL::onSystemClockChange{};
EmEvent<double, double> EmHAL::onPwmChange{};

vector<EmHAL::CycleConsumer> EmHAL::cycleConsumers;

// ---------------------------------------------------------------------------
//		� EmHAL::AddHandler
// ---------------------------------------------------------------------------

void EmHAL::AddHandler(EmHALHandler* handler) {
    EmAssert(handler->fNextHandler == NULL);
    EmAssert(handler->fPrevHandler == NULL);
    EmAssert(fgRootHandler == NULL || fgRootHandler->fPrevHandler == NULL);

    if (fgRootHandler != NULL) {
        fgRootHandler->fPrevHandler = handler;
        handler->fNextHandler = fgRootHandler;
    }

    fgRootHandler = handler;
}

// ---------------------------------------------------------------------------
//		� EmHAL::RemoveHandler
// ---------------------------------------------------------------------------

void EmHAL::RemoveHandler(EmHALHandler* handler) {
    if (handler->fNextHandler) {
        handler->fNextHandler->fPrevHandler = handler->fPrevHandler;
    }

    if (handler->fPrevHandler) {
        handler->fPrevHandler->fNextHandler = handler->fNextHandler;
    } else {
        fgRootHandler = handler->fNextHandler;
    }

    handler->fNextHandler = NULL;
    handler->fPrevHandler = NULL;

    EmAssert(handler->fNextHandler == NULL);
    EmAssert(handler->fPrevHandler == NULL);
    EmAssert(fgRootHandler == NULL || fgRootHandler->fPrevHandler == NULL);
}

// ---------------------------------------------------------------------------
//		� EmHAL::RemoveHandler
// ---------------------------------------------------------------------------

void EmHAL::EnsureCoverage(void) {
#if 0
	// Rats...can't get this work...

	Bool			isHandled[20];
	EmHALHandler*	thisHandler = fgRootHandler;
	EmHALHandler	baseHandler;

	while (thisHandler)
	{
		typedef void (EmHALHandler::*CycleHandler)(Bool);
		CycleHandler	p1 = (thisHandler->Cycle);
		CycleHandler	p2 = (baseHandler.Cycle);

		if (p1 != p2)
		{
			isHandled[0] = true;
		}

		thisHandler = thisHandler->fNextHandler;
	}

	for (int ii = 0; ii < sizeof (isHandled); ++ii)
	{
		EmAssert (isHandled[ii]);
	}
#endif
}

// ---------------------------------------------------------------------------
//		� EmHAL::Cycle
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		� EmHAL::CycleSlowly
// ---------------------------------------------------------------------------

void EmHAL::CycleSlowly(Bool sleeping) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->CycleSlowly(sleeping);
}

// ---------------------------------------------------------------------------
//		� EmHAL::ButtonEvent
// ---------------------------------------------------------------------------

void EmHAL::ButtonEvent(ButtonEventT evt) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->ButtonEvent(evt);
}

// ---------------------------------------------------------------------------
//		� EmHAL::TurnSoundOff
// ---------------------------------------------------------------------------

void EmHAL::TurnSoundOff(void) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->TurnSoundOff();
}

// ---------------------------------------------------------------------------
//		� EmHAL::ResetTimer
// ---------------------------------------------------------------------------

void EmHAL::ResetTimer(void) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->ResetTimer();
}

// ---------------------------------------------------------------------------
//		� EmHAL::ResetRTC
// ---------------------------------------------------------------------------

void EmHAL::ResetRTC(void) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->ResetRTC();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetInterruptLevel
// ---------------------------------------------------------------------------

int32 EmHAL::GetInterruptLevel(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetInterruptLevel();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetInterruptBase
// ---------------------------------------------------------------------------

int32 EmHAL::GetInterruptBase(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetInterruptBase();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetLCDScreenOn
// ---------------------------------------------------------------------------
// Called in various portXDataWrite methods to determine if Screen::InvalidateAll
// needs to be called.  Typically implemented in the EmRegs<Product> or
// EmRegs<LCDDriver> subclass.

Bool EmHAL::GetLCDScreenOn(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetLCDScreenOn();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetLCDBacklightOn
// ---------------------------------------------------------------------------
// Called in various portXDataWrite methods to determine if Screen::InvalidateAll
// needs to be called.  Typically implemented in the EmRegs<Product> or
// EmRegs<LCDDriver> subclass.

Bool EmHAL::GetLCDBacklightOn(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetLCDBacklightOn();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetLCDHasFrame
// ---------------------------------------------------------------------------
// Called by host LCD code to know if it needs to draw a 2-pixel white frame.
// Typically implemented in the EmRegs<Processor> or EmRegs<LCDDriver> subclass.

Bool EmHAL::GetLCDHasFrame(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetLCDHasFrame();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetLCDBeginEnd
// ---------------------------------------------------------------------------
// Called by Screen class in order to mark the memory used for the framebuffer.
// Typically implemented in the EmRegs<Processor> or EmRegs<LCDDriver> subclass.

void EmHAL::GetLCDBeginEnd(emuptr& begin, emuptr& end) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->GetLCDBeginEnd(begin, end);
}

bool EmHAL::CopyLCDFrame(Frame& frame) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->CopyLCDFrame(frame);
}

uint16 EmHAL::GetLCD2bitMapping() {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetLCD2bitMapping();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetDynamicHeapSize
// ---------------------------------------------------------------------------

int32 EmHAL::GetDynamicHeapSize(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetDynamicHeapSize();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetROMSize
// ---------------------------------------------------------------------------

int32 EmHAL::GetROMSize(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetROMSize();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetROMBaseAddress
// ---------------------------------------------------------------------------

emuptr EmHAL::GetROMBaseAddress(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetROMBaseAddress();
}

// ---------------------------------------------------------------------------
//		� EmHAL::ChipSelectsConfigured
// ---------------------------------------------------------------------------

Bool EmHAL::ChipSelectsConfigured(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->ChipSelectsConfigured();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetSystemClockFrequency
// ---------------------------------------------------------------------------

int32 EmHAL::GetSystemClockFrequency(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetSystemClockFrequency();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetAsleep
// ---------------------------------------------------------------------------

Bool EmHAL::GetAsleep(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetAsleep();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetPortInputValue
// ---------------------------------------------------------------------------

uint8 EmHAL::GetPortInputValue(int port) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetPortInputValue(port);
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetPortInternalValue
// ---------------------------------------------------------------------------

uint8 EmHAL::GetPortInternalValue(int port) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetPortInternalValue(port);
}

// ---------------------------------------------------------------------------
//		� EmHAL::PortDataChanged
// ---------------------------------------------------------------------------

void EmHAL::PortDataChanged(int port, uint8 oldVal, uint8 newVal) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->PortDataChanged(port, oldVal, newVal);
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetKeyInfo
// ---------------------------------------------------------------------------

void EmHAL::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    EmAssert(EmHAL::GetRootHandler());
    EmHAL::GetRootHandler()->GetKeyInfo(numRows, numCols, keyMap, rows);
}

// ---------------------------------------------------------------------------
//		� EmHAL::LineDriverChanged
// ---------------------------------------------------------------------------
// Open or close the transports in response to their line drivers being
// enabled or disabled.

void EmHAL::LineDriverChanged(EmUARTDeviceType type) {
#if 0  // CSTODO
    ErrCode err = errNone;
    EmTransport* transport = gEmuPrefs->GetTransportForDevice(type);

    if (transport) {
        if (EmHAL::GetLineDriverState(type)) {
            err = transport->Open();
        } else {
            /* err = */ transport->Close();
        }

        if (err != errNone) {
            string errString(transport->GetSpecificName());
            ::PrvHandlePortOpenErrors(err, errString);
        }
    }

#endif
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmHAL::GetLineDriverState(EmUARTDeviceType type) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetLineDriverState(type);
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetUARTDevice
// ---------------------------------------------------------------------------
// Return what sort of device is hooked up to the given UART.

EmUARTDeviceType EmHAL::GetUARTDevice(int uartNum) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetUARTDevice(uartNum);
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetLineDriverStates
// ---------------------------------------------------------------------------
// Collect all the states of all the driver types we know about.  Pass in to
// this routine a variable of type Bool[kUARTEnd].

void EmHAL::GetLineDriverStates(Bool* states) {
    for (EmUARTDeviceType ii = kUARTBegin; ii < kUARTEnd; ++ii) {
        states[ii] = EmHAL::GetLineDriverState(ii);
    }
}

// ---------------------------------------------------------------------------
//		� EmHAL::CompareLineDriverStates
// ---------------------------------------------------------------------------
// Collect the current state of all the line drivers, and compare them to
// a previously-saved snapshot.  For any differences, call LineDriverChanged.

void EmHAL::CompareLineDriverStates(const Bool* oldStates) {
    Bool newStates[kUARTEnd];
    EmHAL::GetLineDriverStates(newStates);

    for (EmUARTDeviceType ii = kUARTBegin; ii < kUARTEnd; ++ii) {
        if (newStates[ii] != oldStates[ii]) {
            EmHAL::LineDriverChanged(ii);
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetDTR
// ---------------------------------------------------------------------------
// DTR is "Data Terminal Ready".  In the same way that the RTS signal is
// typically hooked up to the external device's CTS signal, our DTR pin is
// hooked up to the external device's DSR pin.  It can be modified in order
// to tell the external device if we can accept any data.

Bool EmHAL::GetDTR(int uartNum) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetDTR(uartNum);
}

// ---------------------------------------------------------------------------
//		� EmHAL::DTRChanged
// ---------------------------------------------------------------------------
// Called when the Palm OS changes the setting of the DTR pin.  We respond
// to this change by reflecting the setting in the host's DTR pin.

void EmHAL::DTRChanged(int uartNum) {
#if 0  // CSTODO
    EmUARTDeviceType type = EmHAL::GetUARTDevice(uartNum);
    EmTransport* transport = gEmuPrefs->GetTransportForDevice(type);
    EmTransportSerial* serTransport = dynamic_cast<EmTransportSerial*>(transport);

    if (serTransport) {
        Bool state = EmHAL::GetDTR(uartNum);
        PRINTF("EmHAL::DTRChanged: DTR changed in emulated port to %d.", (int)state);
        serTransport->SetDTR(state);
    }
#endif
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmHAL::GetVibrateOn(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetVibrateOn();
}

// ---------------------------------------------------------------------------
//		� EmHAL::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmHAL::GetLEDState(void) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->GetLEDState();
}

uint32 EmHAL::CyclesToNextInterrupt(uint64 systemCycles) {
    EmAssert(EmHAL::GetRootHandler());
    return EmHAL::GetRootHandler()->CyclesToNextInterrupt(systemCycles);
}

void EmHAL::AddCycleConsumer(CycleHandler handler, void* context) {
    for (const auto consumer : cycleConsumers) {
        if (consumer.handler == handler && consumer.context == context) return;
    }

    cycleConsumers.push_back({handler, context});
}

void EmHAL::RemoveCycleConsumer(CycleHandler handler, void* context) {
    typename vector<CycleConsumer*>::size_type j = 0;

    for (typename vector<CycleConsumer*>::size_type i = 0; i < cycleConsumers.size(); i++)
        if (cycleConsumers[i].handler != handler && cycleConsumers[i].context != context) {
            if (j != i) cycleConsumers[j] = cycleConsumers[i];
            j++;
        }

    if (j < cycleConsumers.size()) cycleConsumers.resize(j);
}

void EmHAL::DispatchCycle(uint64 cycles, bool sleeping) {
    for (auto consumer : cycleConsumers) consumer.handler(consumer.context, cycles, sleeping);
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmHALHandler::EmHALHandler
// ---------------------------------------------------------------------------

EmHALHandler::EmHALHandler(void) : fNextHandler(NULL), fPrevHandler(NULL) {
    EmHAL::AddHandler(this);
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::~EmHALHandler
// ---------------------------------------------------------------------------

EmHALHandler::~EmHALHandler(void) { EmHAL::RemoveHandler(this); }

// ---------------------------------------------------------------------------
//		� EmHALHandler::Cycle
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		� EmHALHandler::CycleSlowly
// ---------------------------------------------------------------------------

void EmHALHandler::CycleSlowly(Bool sleeping) {
    EmAssert(this->GetNextHandler());
    this->GetNextHandler()->CycleSlowly(sleeping);
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::ButtonEvent
// ---------------------------------------------------------------------------

void EmHALHandler::ButtonEvent(ButtonEventT evt) {
    EmAssert(this->GetNextHandler());
    this->GetNextHandler()->ButtonEvent(evt);
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::TurnSoundOff
// ---------------------------------------------------------------------------

void EmHALHandler::TurnSoundOff(void) {
    EmAssert(this->GetNextHandler());
    this->GetNextHandler()->TurnSoundOff();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::ResetTimer
// ---------------------------------------------------------------------------

void EmHALHandler::ResetTimer(void) {
    EmAssert(this->GetNextHandler());
    this->GetNextHandler()->ResetTimer();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::ResetRTC
// ---------------------------------------------------------------------------

void EmHALHandler::ResetRTC(void) {
    EmAssert(this->GetNextHandler());
    this->GetNextHandler()->ResetRTC();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetInterruptLevel
// ---------------------------------------------------------------------------

int32 EmHALHandler::GetInterruptLevel(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetInterruptLevel();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetInterruptBase
// ---------------------------------------------------------------------------

int32 EmHALHandler::GetInterruptBase(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetInterruptBase();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmHALHandler::GetLCDScreenOn(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetLCDScreenOn();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmHALHandler::GetLCDBacklightOn(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetLCDBacklightOn();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmHALHandler::GetLCDHasFrame(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetLCDHasFrame();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetLCDBeginEnd
// ---------------------------------------------------------------------------

void EmHALHandler::GetLCDBeginEnd(emuptr& begin, emuptr& end) {
    EmAssert(this->GetNextHandler());
    this->GetNextHandler()->GetLCDBeginEnd(begin, end);
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetLCDScanlines
// ---------------------------------------------------------------------------

bool EmHALHandler::CopyLCDFrame(Frame& frame) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->CopyLCDFrame(frame);
}

uint16 EmHALHandler::GetLCD2bitMapping() {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetLCD2bitMapping();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetDynamicHeapSize
// ---------------------------------------------------------------------------

int32 EmHALHandler::GetDynamicHeapSize(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetDynamicHeapSize();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetROMSize
// ---------------------------------------------------------------------------

int32 EmHALHandler::GetROMSize(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetROMSize();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetROMBaseAddress
// ---------------------------------------------------------------------------

emuptr EmHALHandler::GetROMBaseAddress(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetROMBaseAddress();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::ChipSelectsConfigured
// ---------------------------------------------------------------------------

Bool EmHALHandler::ChipSelectsConfigured(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->ChipSelectsConfigured();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetSystemClockFrequency
// ---------------------------------------------------------------------------

int32 EmHALHandler::GetSystemClockFrequency(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetSystemClockFrequency();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetAsleep
// ---------------------------------------------------------------------------

Bool EmHALHandler::GetAsleep(void) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetAsleep();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetPortInputValue
// ---------------------------------------------------------------------------

uint8 EmHALHandler::GetPortInputValue(int port) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetPortInputValue(port);
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetPortInternalValue
// ---------------------------------------------------------------------------

uint8 EmHALHandler::GetPortInternalValue(int port) {
    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetPortInternalValue(port);
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::PortDataChanged
// ---------------------------------------------------------------------------

void EmHALHandler::PortDataChanged(int port, uint8 oldVal, uint8 newVal) {
    EmAssert(this->GetNextHandler());
    this->GetNextHandler()->PortDataChanged(port, oldVal, newVal);
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetKeyInfo
// ---------------------------------------------------------------------------

void EmHALHandler::GetKeyInfo(int* numRows, int* numCols, uint16* keyMap, Bool* rows) {
    EmAssert(this->GetNextHandler());
    this->GetNextHandler()->GetKeyInfo(numRows, numCols, keyMap, rows);
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetLineDriverState
// ---------------------------------------------------------------------------
// Return whether or not the line drivers for the given object are open or
// closed.

Bool EmHALHandler::GetLineDriverState(EmUARTDeviceType type) {
    if (EmHALHandler::GetNextHandler()) {
        return EmHALHandler::GetNextHandler()->GetLineDriverState(type);
    }

    return false;
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetUARTDevice
// ---------------------------------------------------------------------------
// Return what sort of device is hooked up to the given UART.

EmUARTDeviceType EmHALHandler::GetUARTDevice(int uartNum) {
    if (EmHALHandler::GetNextHandler()) {
        return EmHALHandler::GetNextHandler()->GetUARTDevice(uartNum);
    }

    return kUARTNone;
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetDTR
// ---------------------------------------------------------------------------
// DTR is "Data Terminal Ready".  In the same way that the RTS signal is
// typically hooked up to the external device's CTS signal, our DTR pin is
// hooked up to the external device's DSR pin.  It can be modified in order
// to tell the external device if we can accept any data.

Bool EmHALHandler::GetDTR(int uartNum) {
    if (EmHALHandler::GetNextHandler()) {
        return EmHALHandler::GetNextHandler()->GetDTR(uartNum);
    }

    return false;
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetVibrateOn
// ---------------------------------------------------------------------------

Bool EmHALHandler::GetVibrateOn(void) {
    if (!this->GetNextHandler()) return false;

    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetVibrateOn();
}

// ---------------------------------------------------------------------------
//		� EmHALHandler::GetLEDState
// ---------------------------------------------------------------------------

uint16 EmHALHandler::GetLEDState(void) {
    if (!this->GetNextHandler()) return 0;

    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->GetLEDState();
}

uint32 EmHALHandler::CyclesToNextInterrupt(uint64 systemCycles) {
    if (!this->GetNextHandler()) return 0;

    EmAssert(this->GetNextHandler());
    return this->GetNextHandler()->CyclesToNextInterrupt(systemCycles);
}
