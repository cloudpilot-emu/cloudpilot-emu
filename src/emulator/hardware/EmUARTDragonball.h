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

#ifndef EmUARTDragonball_h
#define EmUARTDragonball_h

#include "EmThreadSafeQueue.h"  // EmByteQueue

class SessionFile;
class EmTransport;

class EmUARTDragonball {
   public:
    enum UART_Type { kUART_Dragonball, kUART_DragonballEZ, kUART_DragonballVZ, kUART_DragonballSZ };

    struct State {
        State(UART_Type type) { UART_TYPE = type; }

        UART_Type UART_TYPE;

        // +	= Supported
        //		= Unsupported
        // ->	= Input value
        // <-	= Output value

        // UART control register bits
        // These are all values the user sets; we just look at them.

        /*  +  ->  */ unsigned int UART_ENABLE : 1;
        /*  +  ->  */ unsigned int RX_ENABLE : 1;
        /*  +  ->  */ unsigned int TX_ENABLE : 1;
        /*     ->  */ unsigned int RX_CLK_CONT : 1;
        /*  +  ->  */ unsigned int PARITY_EN : 1;
        /*  +  ->  */ unsigned int ODD_EVEN : 1;
        /*  +  ->  */ unsigned int STOP_BITS : 1;
        /*  +  ->  */ unsigned int CHAR8_7 : 1;
        /*     ->  */ unsigned int GPIO_DELTA_ENABLE : 1;  // 68328 only
        /*     ->  */ unsigned int OLD_ENABLE : 1;         // 68EZ328 only
        /*     ->  */ unsigned int CTS_DELTA_ENABLE : 1;
        /*  +  ->  */ unsigned int RX_FULL_ENABLE : 1;
        /*  +  ->  */ unsigned int RX_HALF_ENABLE : 1;
        /*  +  ->  */ unsigned int RX_RDY_ENABLE : 1;
        /*  +  ->  */ unsigned int TX_EMPTY_ENABLE : 1;
        /*  +  ->  */ unsigned int TX_HALF_ENABLE : 1;
        /*  +  ->  */ unsigned int TX_AVAIL_ENABLE : 1;

        // Baud control register bits
        // These are all values the user sets; we just look at them.

        /*     ->  */ unsigned int GPIO_DELTA : 1;  // 68328 only
        /*     ->  */ unsigned int GPIO : 1;        // 68328 only
        /*     ->  */ unsigned int GPIO_DIR : 1;    // 68328 only
        /*     ->  */ unsigned int GPIO_SRC : 1;    // 68328 only
        /*     ->  */ unsigned int UCLK_DIR : 1;    // 68EZ328 only
        /*     ->  */ unsigned int BAUD_SRC : 1;
        /*  +  ->  */ unsigned int DIVIDE;
        /*  +  ->  */ unsigned int PRESCALER;

        // Receive register bits
        // These are all input bits; we set them, not the user.

        /*  +  <-  */ unsigned int RX_FIFO_FULL : 1;
        /*  +  <-  */ unsigned int RX_FIFO_HALF : 1;
        /*  +  <-  */ unsigned int DATA_READY : 1;
        /*     <-  */ unsigned int OLD_DATA : 1;  // 68EZ328 only
        /*     <-  */ unsigned int OVRUN : 1;
        /*     <-  */ unsigned int FRAME_ERROR : 1;
        /*     <-  */ unsigned int BREAK : 1;
        /*     <-  */ unsigned int PARITY_ERROR : 1;
        /*  +  <-  */ unsigned int RX_DATA;

        // Transmitter register bits
        // The user sets TX_DATA, IGNORE_CTS, and SEND_BREAK.
        // We set the rest.

        /*  +  <-  */ unsigned int TX_FIFO_EMPTY : 1;
        /*  +  <-  */ unsigned int TX_FIFO_HALF : 1;
        /*  +  <-  */ unsigned int TX_AVAIL : 1;
        /*     ->  */ unsigned int SEND_BREAK : 1;
        /*  +  ->  */ unsigned int IGNORE_CTS : 1;
        /*  +  <-  */ unsigned int BUSY : 1;  // 68EZ328 only
        /*  +  <-  */ unsigned int CTS_STATUS : 1;
        /*     <-  */ unsigned int CTS_DELTA : 1;
        /*  +  ->  */ unsigned int TX_DATA;

        // Misc register bits
        // These are all values the user sets; we just look at them.

        /*     ->  */ unsigned int BAUD_TEST : 1;  // 68EZ328 only
        /*     ->  */ unsigned int CLK_SRC : 1;
        /*     ->  */ unsigned int FORCE_PERR : 1;
        /*  +  ->  */ unsigned int LOOP : 1;
        /*     ->  */ unsigned int BAUD_RESET : 1;  // 68EZ328 only
        /*     ->  */ unsigned int IR_TEST : 1;     // 68EZ328 only
        /*  +  ->  */ unsigned int RTS_CONT : 1;
        /*  +  ->  */ unsigned int RTS : 1;
        /*  +  ->  */ unsigned int IRDA_ENABLE : 1;
        /*     ->  */ unsigned int IRDA_LOOP : 1;
        /*     ->  */ unsigned int RX_POL : 1;  // 68EZ328 only
        /*     ->  */ unsigned int TX_POL : 1;  // 68EZ328 only

        // Level Marker Interrupt

        /*  +  ->  */ unsigned int TXFIFO_LEVEL_MARKER : 4;  // 68SZ328 only
        /*  +  ->  */ unsigned int RXFIFO_LEVEL_MARKER : 4;  // 68SZ328 only
    };

    EmUARTDragonball(UART_Type, int uartNum);
    ~EmUARTDragonball(void);

    void StateChanged(State&, Bool sendRxData);
    void UpdateState(State&, Bool refreshRxData);

    void TransmitTxFIFO(EmTransport*);
    void ReceiveRxFIFO(EmTransport*);

    EmTransport* GetTransport(void);

   private:
    int PrvFIFOSize(Bool forRX);
    int PrvLevelMarker(Bool forRX);

   private:
    int fUARTNum;
    State fState;
    EmByteQueue fRxFIFO;
    EmByteQueue fTxFIFO;
};

#endif /* EmUARTDragonball_h */
