//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_I2S_H_
#define _SOC_I2S_H_

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

struct SocI2s;

struct SocI2s *socI2sInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma);
void socI2sPeriodic(struct SocI2s *i2s);

#ifdef __cplusplus
}
#endif

#endif
