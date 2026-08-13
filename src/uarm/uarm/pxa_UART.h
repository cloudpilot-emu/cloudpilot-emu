//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_UART_H_
#define _PXA_UART_H_

#include <cstdint>

#include "CPU.h"
#include "mem.h"
#include "pxa_IC.h"
#include "reschedule.h"

/*
        PXA255 UARTs

        They are identical, but at diff base addresses. this implements one. instanciate more than
   one to make all 3 work if needed.

        by default we read nothing and write nowhere (buffer drains fast into nothingness)
        this can be changed by addidng appropriate callbacks

*/

#define PXA_FFUART_BASE 0x40100000UL
#define PXA_BTUART_BASE 0x40200000UL
#define PXA_STUART_BASE 0x40700000UL
#define PXA_HWUART_BASE 0x41600000UL  // PXA25x/PXA26x only

#define UART_CHAR_BREAK 0x800
#define UART_CHAR_FRAME_ERR 0x400
#define UART_CHAR_PAR_ERR 0x200
#define UART_CHAR_NONE 0x100

struct PxaUart;

typedef uint_fast16_t (*PxaUartReadF)(void *userData);
typedef void (*PxaUartWriteF)(uint_fast16_t chr, void *userData);
typedef bool (*PxaUartClientIsActive)(void *userData);
typedef void (*PxaUartClientTick)(void *userData);

struct PxaUart *pxaUartInit(struct ArmMem *physMem, struct Reschedule reschedule, struct PxaIc *ic,
                            uint32_t baseAddr, uint8_t irq);
void pxaUartProcess(struct PxaUart *uart);  // write out data in TX fifo and read data into RX fifo

void pxaUartSetFuncs(struct PxaUart *uart, PxaUartReadF readF, PxaUartWriteF writeF,
                     PxaUartClientIsActive clientIsActiveF, PxaUartClientTick clientTickF,
                     void *userData);

bool pxaUartTaskRequired(struct PxaUart *uart);

template <typename T>
void pxaUartSave(struct PxaUart *uart, T &savestate, uint32_t index = 0);

template <typename T>
void pxaUartLoad(struct PxaUart *uart, T &loader, uint32_t index = 0);

#endif
