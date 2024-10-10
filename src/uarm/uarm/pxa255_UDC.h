//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA255_UDC_H_
#define _PXA255_UDC_H_

#include <stdio.h>

#include "CPU.h"
#include "mem.h"
#include "pxa_DMA.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Pxa255Udc;

struct Pxa255Udc *pxa255UdcInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma);

#ifdef __cplusplus
}
#endif

#endif
