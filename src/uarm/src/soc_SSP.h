//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_SSP_H_
#define _SOC_SSP_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "CPU.h"
#include "mem.h"
#include "soc_DMA.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SocSsp;

typedef uint_fast16_t (*SspClientProcF)(
    void* userData, uint_fast8_t nBits,
    uint_fast16_t sent);  // return result byte. 0 if nothing to say (so that replies can be ORRed)

struct SocSsp* socSspInit(struct ArmMem* physMem, struct SocIc* ic, struct SocDma* dma,
                          uint32_t base, uint_fast8_t irqNo, uint_fast8_t dmaReqNoBase);
void socSspPeriodic(struct SocSsp* ssp);
bool socSspAddClient(struct SocSsp* ssp, SspClientProcF procF, void* userData);

#ifdef __cplusplus
}
#endif

#endif
