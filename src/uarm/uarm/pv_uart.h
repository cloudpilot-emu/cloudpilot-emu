#ifndef _PV_UART_H_
#define _PV_UART_H_

#include <cstdint>

struct PvUart;
struct ArmMem;

typedef void (*PvUartWriteF)(uint8_t chr, void* ctx);

PvUart* pvUartInit(ArmMem* mem, uint32_t base);
void pvUartSetWriteF(PvUart* uart, PvUartWriteF writeF, void* ctx);

#endif  //  _PV_UART_H_
