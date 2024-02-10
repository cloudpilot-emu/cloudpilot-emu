/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmUARTDragonball.h"

#include "EmCommon.h"
#include "EmHAL.h"
#include "EmSession.h"
#include "EmTransportSerial.h"
#include "Logging.h"
#include "Preferences.h"

/*
        This module contains the routines for handling serial I/O.  It
        is responsible for responding to changes in state enacted by
        software (from either the OS or user), and for dealing with
        the actual transmission and reception of serial data.

        There are four ways in which serial activity could occur: something
        could write to a UART register, something could read from a UART
        register, a byte could be received from the host serial port, or
        a byte could be sent out the host serial port.  Here is what
        happens on each of those events.

        Something reads a UART register:
                - If the register is the RX_DATA register, clear the DATA_READY bit
                - Make sure the state is up-to-date (including interrupts)
                - Return the register contents

        Something writes to a UART register:
                - Update the writable parts of the register
                - React to any changes
                - Make sure the state is up-to-date (including interrupts)

        Data appears at the host serial port:
                - Post the byte to the RX FIFO (if there is room)
                - Make sure the state is up-to-date (including interrupts)

        Data needs to be sent to the host serial port:
                - Send the first byte in the TX FIFO
                - Make sure the state is up-to-date (including interrupts)
*/

// ======================================================================
//	Private functions
// ======================================================================

static const int kMaxFifoSize = 64;

static Bool PrvPinBaud(EmTransportSerial::Baud& newBaud);
static Bool PrvPinBaud(EmTransportSerial::Baud& newBaud, EmTransportSerial::Baud testBaud);

// #define LOGGING 0
#ifdef LOGGING
    #define PRINTF logging::printf
#else
    #define PRINTF(...) ;
#endif

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::EmUARTDragonball
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	type - the type of UART to emulate.  The Dragonball
 *					and DragonballEZ UARTs are similar enough that we
 *					can handle them both here with just a few tests in
 *					the places where they differ.
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

EmUARTDragonball::EmUARTDragonball(UART_Type type, int uartNum)
    : fUARTNum(uartNum),
      fState(type),
      fRxFIFO(this->PrvFIFOSize(true)),
      fTxFIFO(this->PrvFIFOSize(false)) {}

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::~EmUARTDragonball
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

EmUARTDragonball::~EmUARTDragonball(void) {
    for (auto uart : {kUARTSerial, kUARTIR}) {
        EmTransportSerial* transport = gSession->GetTransportSerial(uart);
        if (transport) transport->Close();
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::StateChanged
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmUARTDragonball::StateChanged(State& newState, Bool sendTxData) {
    EmAssert(fState.UART_TYPE == newState.UART_TYPE);

    // (Changing the configuration is the only place where we assume that
    // the transport we're using is a serial transport.)

    EmTransportSerial* transport = this->GetTransport();

    EmTransportSerial::Config config;
    if (transport) config = transport->GetConfig();

    // ========== RX_ENABLE ==========
    //
    // This bit enables the receiver block. While this bit is low, the receiver is disabled and the
    // receive FIFO is flushed. This bit resets to 0.

    if (fState.RX_ENABLE != newState.RX_ENABLE) {
        if (newState.RX_ENABLE == 0) {
            fRxFIFO.Clear();
        }
    }

    // ========== TX_ENABLE ==========
    //
    // This bit enables the transmitter block. While this bit is low, the transmitter is disabled
    // and the transmit FIFO is flushed. This bit resets to 0.

    if (fState.TX_ENABLE != newState.TX_ENABLE) {
        if (newState.TX_ENABLE == 0) {
            fTxFIFO.Clear();
        }
    }

    // ========== PARITY_EN ==========
    //
    // This bit controls the parity generator in the transmitter and parity checker in the receiver.
    // When this bit is high, they are enabled. When it is low, they are disabled.
    //
    // ========== ODD_EVEN ==========
    //
    // This bit controls the sense of the parity generator and checker. When this bit is high, odd
    // parity is generated and expected. When this bit is low, even parity is generated and
    // expected. This bit has no function if PARITY EN is low.

    if (newState.PARITY_EN == 0) {
        config.fParity = EmTransportSerial::kNoParity;
    } else if (newState.ODD_EVEN) {
        config.fParity = EmTransportSerial::kOddParity;
    } else {
        config.fParity = EmTransportSerial::kEvenParity;
    }

    // ========== STOP_BITS =========
    //
    // This bit controls the number of stop bits transmitted after a character. When this bit is
    // high, two stop bits are sent. When this bit is low, one stop bit is sent. This bit has no
    // effect on the receiver, which expects one or more stop bits.

    config.fStopBits = newState.STOP_BITS ? 2 : 1;

    // ========== CHAR8_7 ==========
    //
    // This bit controls the character length. While high, the transmitter and receiver are in 8-bit
    // mode. While low, they are in 7-bit mode. The transmitter then ignores B7 and the receiver
    // sets B7 to 0.

    config.fDataBits = newState.CHAR8_7 ? 8 : 7;

    // ========== DIVIDE ==========
    //
    // These bits control the clock frequency produced by the baud rate generator.
    //
    //		000 = Divide by 1.
    //		001 = Divide by 2.
    //		010 = Divide by 4.
    //		011 = Divide by 8.
    //		100 = Divide by 16.
    //		101 = Divide by 32.
    //		110 = Divide by 64.
    //		111 = Divide by 128.
    //
    // ========== PRESCALER ==========
    //
    // These bits control the division value of the baud generator prescaler. The division value is
    // determined by the following formula:
    //
    //		Prescaler division value = 65 (decimal) - PRESCALER

    // Baud rate is sysClockFreq / preScaler / divider / 16
    //
    // (Using sysClockFreq / (preScaler * divider * 16) might get closer to the
    //  intended baud (as would using floating point), but does it mirror what
    //  the hardware actually does?)

    int32 sysClockFreq = EmHAL::GetSystemClockFrequency();
    config.fBaud = sysClockFreq / (65 - newState.PRESCALER) / (1 << newState.DIVIDE) / 16;

    // "newRate" is only approximate to within 0.1%. Pin the value to an
    // exact value.
    //
    // !!! What to do if we can't pin to a valid value?

    (void)PrvPinBaud(config.fBaud);

    // ========== IGNORE_CTS ==========
    //
    // When this bit is high, it forces the CTS signal that is presented to the transmitter to
    // always be asserted, which effectively ignores the external pin.

    config.fHwrHandshake = newState.IGNORE_CTS == 0;

    // ========== RTS_CONT ==========
    //
    // This bit selects the function of the RTS pin.
    //
    //		0 = RTS pin is controlled by the RTS bit.
    //		1 = RTS pin is controlled by the receiver FIFO. When the FIFO is full (one slot is
    //			remaining), RTS is negated.
    //
    // ========== RTS ==========
    //
    // This bit controls the RTS pin while the RTS CONT bit is 0.
    //
    //		0 = RTS pin is 1.
    //		1 = RTS pin is 0.

    if ((fState.RTS_CONT != newState.RTS_CONT || fState.RTS != newState.RTS) && transport) {
        if (newState.RTS_CONT) {
            transport->SetRTS(EmTransportSerial::kRTSAuto);
        } else {
            if (newState.RTS) {
                transport->SetRTS(EmTransportSerial::kRTSOn);
            } else {
                transport->SetRTS(EmTransportSerial::kRTSOff);
            }
        }
    }

    // ========== UART_ENABLE ==========
    //
    // This bit enables the UART module. When this bit is low, the UART module is disabled and
    // in low-power mode. While this bit is high, the UART module is active. This bit resets to 0.

    // ========== IRDA_ENABLE ==========
    //
    // This bit enables the IrDA interface.
    //
    //		0 = Normal NRZ operation.
    //		1 = IRDA operation.

    if (fState.UART_ENABLE != newState.UART_ENABLE || fState.IRDA_ENABLE != newState.IRDA_ENABLE) {
        // Nothing to do here.
    }

    // Establish the new settings.  Do this only when the UART is
    // enabled, to help reduce the thrashing on the host serial port,
    // and to help prevent the installation of invalid settings (which
    // could appear in the UART registers as it's being configured).

    if (newState.UART_ENABLE && transport) {
        transport->SetConfig(config);
    }

    // ========== SEND_BREAK ==========
    //
    // This bit forces the transmitter to immediately send continuous zeros creating a break
    // character.

    if (fState.SEND_BREAK != newState.SEND_BREAK && transport) {
        transport->SetBreak(newState.SEND_BREAK);
    }

    // ========== TX_DATA ==========
    //
    // These bits are the parallel transmit-data inputs. In 7-bit mode, D7 is ignored and in 8-bit
    // mode, all of the bits are used. Data is transmitted LSB first. A new character is transmitted
    // when these bits are written and have passed through the FIFO.

    // ========== LOOP ==========
    //
    // This bit controls loopback for system testing purposes. When this bit is high, the receiver
    // input is internally connected to the transmitter and ignores the RXD pin. The TXD pin is
    // unaffected by this bit.

    if (sendTxData && newState.UART_ENABLE && newState.TX_ENABLE) {
        if (newState.LOOP == 0) {
            if (transport && transport->CanWrite())  // The host serial port is open
            {
                // With or without hardware handshaking, we'll put data
                // in the FIFO, and let the host's handshaking take care
                // of when the data is removed from the FIFO.

                if (fTxFIFO.GetFree() > 0)  // There's room in the FIFO
                {
                    fTxFIFO.Put(newState.TX_DATA);  // so add the data

                    // Call TransmitTxFIFO here to send the data we
                    // just queued up.  Doing this is important on the Mac in
                    // order to send out the data quickly instead of later at
                    // idle time.

                    this->TransmitTxFIFO(transport);
                }
            } else  // The host serial port is NOT open
            {
                if (config.fHwrHandshake)  // Reflects the state of the IGNORE_CTS bit.
                {
                    // With hardware handshaking, data is sent only when CTS
                    // is asserted.  With no host serial port, we define that
                    // CTS is never asserted, so the data clogs up the FIFO.

                    if (fTxFIFO.GetFree() > 0)  // There's room in the FIFO
                    {
                        fTxFIFO.Put(newState.TX_DATA);  // so add the data

                        // Serial port is closed, so don't call Platform::TransmitTxFIFO.
                    }
                } else {
                    // With no hardware handshaking, data is sent whenever it's
                    // ready.  With nowhere to go, we can drop it on the floor.
                }
            }
        } else  // We're in loopback mode.
        {
            if (fRxFIFO.GetFree() > 0) {
                fRxFIFO.Put(newState.TX_DATA);
            }
        }
    }

    // Update the state in case any of the above operations have side-effects.

    UpdateState(newState, false);

    // Remember this for next time.

    fState = newState;
}

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::UpdateState
 *
 * DESCRIPTION:	Receive any data, and update the FIFO state registers.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmUARTDragonball::UpdateState(State& state, Bool refreshRxData) {
    EmAssert(fState.UART_TYPE == state.UART_TYPE);

    // Update the RxFIFO if there's been any buffered data.

    bool receiveWasInProgress = receiveInProgress;

    EmTransportSerial* transport = this->GetTransport();
    if (transport) {
        this->ReceiveRxFIFO(transport);
    }

    if (receiveInProgress != receiveWasInProgress && !receiveInProgress) {
        state.OLD_DATA = true;
    }

    // === RX_FIFO_FULL ===
    //
    // This read-only bit indicates that the receiver FIFO is full and may generate an overrun. This
    // bit generates a maskables interrupt.
    //
    // Further, from the overview section of the manual:
    //
    // If your software has a short interrupt
    // latency time, the FIFO FULL interrupt in the Receiver register can be enabled. The FIFO has
    // one remaining space available when this interrupt is generated.

    state.RX_FIFO_FULL = fRxFIFO.GetFree() == 0;

    // === RX_FIFO_HALF ===
    //
    // This read-only bit indicates that the receiver FIFO has four or fewer slots remaining in the
    // FIFO. This bit generates a maskable interrupt.

    state.RX_FIFO_HALF = fRxFIFO.GetFree() <= this->PrvLevelMarker(true);

    // === DATA_READY ===
    //
    // This read-only bit indicates that at least one byte is present in the receive FIFO. The
    // character bits are valid only while this bit is set. This bit generates a maskable interrupt.

    state.DATA_READY = fRxFIFO.GetUsed() > 0;

    // === OLD_DATA ===	// non-68328 only
    //
    // This read-only bit indicates that data in the FIFO is older than 30 bit times. It is useful
    // in situations where the FIFO FULL or FIFO HALF interrupts are used. If there is data in the
    // FIFO, but below the interrupt threshold, a maskable interrupt can be generated to alert the
    // software that unread data is present. This bit clears when the character bits are read.

    // Not supported right now.

    // === OVRUN ===
    //
    // When this read-only bit is high, it indicates that the receiver overwrote data in the FIFO.
    // The character with this bit set is valid, but at least one previous character was lost. In
    // normal circumstances, this bit should never be set. It indicates that your software is not
    // keeping up with the incoming data rate. This bit is updated and valid for each received
    // character.

    // !!! TBD

    // === FRAME_ERROR ===
    //
    // While high, this read-only bit indicates that the current character had a framing error
    // (missing stop bit), indicating the possibility of corrupted data. This bit is updated for
    // each character read from the FIFO.

    // !!! TBD

    // === BREAK ===
    //
    // When this read-only bit is high, it indicates that the current character was detected as a
    // BREAK. The data bits are all 0 and the stop bit was also 0. The FRAME ERROR bit will
    // always be set when this bit is set. If odd parity is selected, PARITY ERROR will also be set
    // along with this bit. This bit is updated and valid with each character read from the FIFO.

    // !!! TBD

    // === PARITY_ERROR ===
    //
    // When this read-only bit is high, it indicates that the current character was detected with a
    // parity error, indicating the possibility of corrupted data. This bit is updated and valid
    // with each character read from the FIFO. While parity is disabled, this bit always reads zero.

    // !!! TBD

    // === RX_DATA ===
    //
    // These read-only bits are the top receive character in the FIFO. They have no meaning if the
    // DATA READY bit is 0. In 7-bit mode, the MSB is forced to 0 and in 8-bit mode, all bits are
    // active.

    if (state.DATA_READY && state.UART_ENABLE && state.RX_ENABLE && refreshRxData) {
        state.RX_DATA = fRxFIFO.Get();

        if (state.CHAR8_7 == 0) {
            state.RX_DATA &= 0x07F;
        }

        PRINTF("UART: Put 0x%02X into RX_DATA.", (uint32)(uint8)state.RX_DATA);
    }

    // === TX_FIFO_EMPTY ===
    //
    // This read-only bit indicates that the transmit FIFO is empty. This bit generates a maskable
    // interrupt.

    state.TX_FIFO_EMPTY = fTxFIFO.GetUsed() == 0;

    // === TX_FIFO_HALF ===
    //
    // This read-only bit indicates that the transmit FIFO is less than half full. This bit
    // generates a maskable interrupt.

    state.TX_FIFO_HALF = fTxFIFO.GetUsed() < this->PrvLevelMarker(false);

    // === TX_AVAIL ===
    //
    // This read-only bit indicates that the transmit FIFO has at least one slot available for data.
    // This bit generates a maskable interrupt.

    state.TX_AVAIL = fTxFIFO.GetFree() > 0;

    // === BUSY ===	// non-68328 only
    //
    // When this bit is high, it indicates that the transmitter is busy sending a character. This
    // signal is asserted while the transmitter state machine is not idle or the FIFO has data in
    // it.

    state.BUSY = !state.TX_FIFO_EMPTY;

    // === CTS_STATUS ===
    //
    // This bit indicates the current status of the CTS pin. A "snapshot" of the pin is taken
    // immediately before this bit is presented to the data bus. While the IGNORE CTS bit is high,
    // this bit can serve as a general-purpose input.
    //
    // Note that this pin is ACTIVE LOW!  That's why the Boolean expression is negated below.
    //
    // For now, say that it's clear to send if the FIFO is empty
    //
    // !!! TBD - could be better?

    state.CTS_STATUS = !(fTxFIFO.GetUsed() == 0);

    // === CTS_DELTA ===
    //
    // When this bit is high, it indicates that the CTS pin changed state and generates a maskable
    // interrupt. The current state of the CTS pin is available on the CTS STATUS bit. You can
    // generate an immediate interrupt by setting this bit high. The CTS interrupt is cleared by
    // writing 0 to this bit.

    // Not supported right now.

    // Remember this for next time.

    fState = state;
}

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::TransmitTxFIFO
 *
 * DESCRIPTION:	Transmit any bytes in the TX FIFO out the serial port.
 *				Assumes that the serial port is open.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	Nothing
 *
 ***********************************************************************/

void EmUARTDragonball::TransmitTxFIFO(EmTransportSerial* transport) {
    EmAssert(transport);

    if (transport->CanWrite()) {
        // Write out any outgoing bytes.

        char buffer[kMaxFifoSize];
        long spaceInTxFIFO = fTxFIFO.GetUsed();

        if (spaceInTxFIFO > 0) {
            for (long ii = 0; ii < spaceInTxFIFO; ++ii) {
                buffer[ii] = fTxFIFO.Get();
            }
#if 0  // CSTODO
            if (LogSerialData())
                LogAppendData(buffer, spaceInTxFIFO, "UART: Transmitted data:");
            else
#endif
            PRINTF("UART: Transmitted %ld serial bytes.", spaceInTxFIFO);

            transport->Write(spaceInTxFIFO, buffer);
        }
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::ReceiveRxFIFO
 *
 * DESCRIPTION:	Fills up the RX FIFO with as many bytes as it can from
 *				the host serial port.  Assumes that the serial port is
 *				open.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	Nothing
 *
 ***********************************************************************/

void EmUARTDragonball::ReceiveRxFIFO(EmTransportSerial* transport) {
    EmAssert(transport);

    if (transport->CanRead()) {
        receiveInProgress = true;

        // Buffer up any incoming bytes.

        char buffer[kMaxFifoSize];
        long spaceInRxFIFO = fRxFIFO.GetFree();

        // See how many bytes are waiting.

        long bytesToBuffer = transport->BytesPending();

        // See if we have that much room in the FIFO. If not, limit our read
        // to that many bytes.

        if (bytesToBuffer > spaceInRxFIFO) {
            bytesToBuffer = spaceInRxFIFO;
        }

        // If there is data waiting in the wings, and the hardware says it's
        // OK to receive data, and there's room in the RxFIFO, then receive it.
        //
        // It's OK to receive data if RTS_CONT is 1.  That means that the FIFO
        // controls whether or not it's OK to receive data, and that check
        // is taken care of with "bytesToBuffer > 0".
        //
        // If RTS_CONT is zero, then it's OK to receive data if RTS is 1.  That
        // mean that the /RTS pin is zero, which means OK to receive.

        if (bytesToBuffer > 0 && (fState.RTS_CONT == 1 || fState.RTS == 1)) {
            // If there are still bytes to be read, read them in and insert them
            // into the RX FIFO.  If the buffer was previously empty, Hardware::Cycle
            // will notice that there are now bytes in there and update the
            // UART registers.
            //
            // !!! TBD: Hardware::Cycle is called after every opcode execution.
            // Since it is very rare that serial bytes are coming in, the overhead
            // incurred for checking to see if there are serial bytes does not pay
            // off.  We may want to figure out a way to update the UART registers
            // here, and take the check out of Hardware::Cycle.  I'd do that here,
            // but (a) Hardware::Cycle takes a "sleeping" parameter that I'd have
            // to determine here, and (b) the similar solution on Windows is more
            // difficult, as I'd have to stop the CPU thread, update the registers,
            // and restart the thread.

            if (transport->Read(bytesToBuffer, buffer)) {
#if 0  // CSTODO
       // not quite the correct phrase for IR over serial (over TCP)
                if (LogSerialData())
                    LogAppendData(buffer, bytesToBuffer, "UART: Received data:");
                else
#endif
                PRINTF("UART: Received %ld serial bytes.", bytesToBuffer);

                for (long ii = 0; ii < bytesToBuffer; ++ii) {
                    fRxFIFO.Put(buffer[ii]);
                }  // end loop that puts bytes into FIFO
            }      // end no-error-from-EmTransport::Read
        }          // end BytesInBuffer-returned-non-zero
    }              // end is-serial-port-open
    else {
        receiveInProgress = false;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::GetTransport
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	.
 *
 * RETURNED:	.
 *
 ***********************************************************************/

EmTransportSerial* EmUARTDragonball::GetTransport(void) {
    EmUARTDeviceType type = EmHAL::GetUARTDevice(fUARTNum);

    return gSession->GetTransportSerial(type);
}

void EmUARTDragonball::Cycle(uint64 systemCycles) {}

void EmUARTDragonball::SetModeSync(bool sync) { this->sync = sync; }

bool EmUARTDragonball::GetModeSync() const { return sync; }

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::PrvFIFOSize
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	.
 *
 * RETURNED:	.
 *
 ***********************************************************************/

int EmUARTDragonball::PrvFIFOSize(Bool forRX) {
    int size;

    switch (fState.UART_TYPE) {
        case kUART_Dragonball:
            size = 8;
            break;

        case kUART_DragonballEZ:
            if (forRX) {
                size = 12;
            } else {
                size = 8;
            }
            break;

        case kUART_DragonballVZ:
            if (this->fUARTNum == 0) {
                if (forRX) {
                    size = 12;
                } else {
                    size = 8;
                }
            } else {
                size = 64;
            }
            break;

        default:
            EmAssert(false);
            size = 8;
            break;
    }

    return size;
}

/***********************************************************************
 *
 * FUNCTION:	EmUARTDragonball::PrvLevelMarker
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	.
 *
 * RETURNED:	.
 *
 ***********************************************************************/

int EmUARTDragonball::PrvLevelMarker(Bool forRX) {
    int level = (forRX ? fRxFIFO.GetMaxSize() : fTxFIFO.GetMaxSize()) / 2;

    switch (fState.UART_TYPE) {
        case kUART_Dragonball:
            break;

        case kUART_DragonballEZ:
            break;

        case kUART_DragonballVZ:
            if (this->fUARTNum == 1) {
                int marker = forRX ? fState.TXFIFO_LEVEL_MARKER : fState.RXFIFO_LEVEL_MARKER;

                if (marker != 0) {
                    level = marker * 4;
                }
            }
            break;

        default:
            EmAssert(false);
            break;
    }

    return level;
}

/***********************************************************************
 *
 * FUNCTION:	PrvPinBaud
 *
 * DESCRIPTION:	Pins the given baud value to the test baud value if the
 *				former is sufficiently close to the latter.
 *
 * PARAMETERS:	newBaud - the value to possibly alter.
 *
 *				testBaud - the value to pin to.
 *
 * RETURNED:	newBaud is changed in place.  If it is changed, the
 *				function returns true.  Otherwise, it returns false.
 *
 ***********************************************************************/

Bool PrvPinBaud(EmTransportSerial::Baud& newBaud) {
    Bool pinned = false;

    if (!pinned) pinned = PrvPinBaud(newBaud, 150);
    if (!pinned) pinned = PrvPinBaud(newBaud, 300);
    if (!pinned) pinned = PrvPinBaud(newBaud, 600);
    if (!pinned) pinned = PrvPinBaud(newBaud, 1200);
    if (!pinned) pinned = PrvPinBaud(newBaud, 1800);
    if (!pinned) pinned = PrvPinBaud(newBaud, 2400);
    if (!pinned) pinned = PrvPinBaud(newBaud, 3600);
    if (!pinned) pinned = PrvPinBaud(newBaud, 4800);
    if (!pinned) pinned = PrvPinBaud(newBaud, 7200);
    if (!pinned) pinned = PrvPinBaud(newBaud, 9600);
    if (!pinned) pinned = PrvPinBaud(newBaud, 14400);
    if (!pinned) pinned = PrvPinBaud(newBaud, 19200);
    if (!pinned) pinned = PrvPinBaud(newBaud, 28800);
    if (!pinned) pinned = PrvPinBaud(newBaud, 38400);
    if (!pinned) pinned = PrvPinBaud(newBaud, 57600);
    if (!pinned) pinned = PrvPinBaud(newBaud, 115200);
    if (!pinned) pinned = PrvPinBaud(newBaud, 230400);

    return pinned;
}

/***********************************************************************
 *
 * FUNCTION:	PrvPinBaud
 *
 * DESCRIPTION:	Pins the given baud value to the test baud value if the
 *				former is sufficiently close to the latter.
 *
 * PARAMETERS:	newBaud - the value to possibly alter.
 *
 *				testBaud - the value to pin to.
 *
 * RETURNED:	newBaud is changed in place.  If it is changed, the
 *				function returns true.  Otherwise, it returns false.
 *
 ***********************************************************************/

Bool PrvPinBaud(EmTransportSerial::Baud& newBaud, EmTransportSerial::Baud testBaud) {
    // Pin to within 2%.  The Dragonball reference says that the uBaud
    // register should be accurate to within 0.1%, but let's give it
    // some slop.

    if (newBaud > (testBaud - (testBaud / 50)) && newBaud < (testBaud + (testBaud / 50))) {
        newBaud = testBaud;
        return true;
    }

    return false;
}
