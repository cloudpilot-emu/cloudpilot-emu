//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_UART_H_
#define _SOC_UART_H_

#include "CPU.h"
#include "mem.h"
#include "soc_IC.h"

struct SocUart;

#define UART_CHAR_BREAK 0x800
#define UART_CHAR_FRAME_ERR 0x400
#define UART_CHAR_PAR_ERR 0x200
#define UART_CHAR_NONE 0x100

typedef uint_fast16_t (*SocUartReadF)(void *userData);
typedef void (*SocUartWriteF)(uint_fast16_t chr, void *userData);

struct SocUart *socUartInit(struct ArmMem *physMem, struct SocIc *ic, uint32_t baseAddr,
                            uint8_t irq);
void socUartProcess(struct SocUart *uart);  // write out data in TX fifo and read data into RX fifo

void socUartSetFuncs(struct SocUart *uart, SocUartReadF readF, SocUartWriteF writeF,
                     void *userData);

#endif
