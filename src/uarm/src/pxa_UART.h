//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_UART_H_
#define _PXA_UART_H_

#include "CPU.h"
#include "mem.h"
#include "soc_UART.h"

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

#endif
