#ifndef _UART_BCM2035_H_
#define _UART_BCM2035_H_

#include <stdint.h>

#include "get_emu_time.h"
#include "soc_UART.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Bcm2035;

struct Bcm2035* bcm2035Init(GetEmuTime getTime);

void bcm2035RegisterWithUart(struct Bcm2035* bcm2035, SocUart* uart);

#ifdef __cplusplus
}

template <typename T>
void bcm2035Save(struct Bcm2035* bcm2035, T& savestate);

template <typename T>
void bcm2035Load(struct Bcm2035* bcm2035, T& loader);

#endif

#endif  // _UART_BCM2035_H_
