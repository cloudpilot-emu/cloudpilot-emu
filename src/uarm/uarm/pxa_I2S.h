#ifndef _PXA_I2S_H_
#define _PXA_I2S_H_

#include "CPU.h"
#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"

struct PxaI2s;

struct PxaI2s *pxaI2sInit(struct ArmMem *physMem, struct PxaIc *ic, struct PxaDma *dma);
void pxaI2sPeriodic(struct PxaI2s *i2s);

template <typename T>
void pxaI2sSave(struct PxaI2s *i2s, T &savestate);

template <typename T>
void pxaI2sLoad(struct PxaI2s *i2s, T &loader);

#endif  // _PXA_I2S_H_
