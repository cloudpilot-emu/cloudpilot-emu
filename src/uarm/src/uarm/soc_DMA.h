//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_DMA_H_
#define _SOC_DMA_H_

#include "CPU.h"
#include "mem.h"
#include "reschedule.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SocDma;

struct SocDma* socDmaInit(struct ArmMem* physMem, struct Reschedule reschedule, struct SocIc* ic);
void socDmaPeriodic(struct SocDma* dma);
void socDmaExternalReq(struct SocDma* dma, uint_fast8_t chNum,
                       bool requested);  // request a transfer burst

bool socDmaTaskRequired(struct SocDma* dma);

#ifdef __cplusplus
}
#endif

#endif
