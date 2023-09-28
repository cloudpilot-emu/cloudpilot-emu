//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_DMA_H_
#define _SOC_DMA_H_

#include "CPU.h"
#include "mem.h"
#include "soc_IC.h"

struct SocDma;

struct SocDma* socDmaInit(struct ArmMem* physMem, struct SocIc* ic);
void socDmaPeriodic(struct SocDma* dma);
void socDmaExternalReq(struct SocDma* dma, uint_fast8_t chNum,
                       bool requested);  // request a transfer burst

#endif
