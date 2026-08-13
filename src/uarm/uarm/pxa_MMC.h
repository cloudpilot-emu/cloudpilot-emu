//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_MMC_H_
#define _PXA_MMC_H_

#include "CPU.h"
#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"
#include "vSD.h"

struct PxaMmc;

struct PxaMmc* pxaMmcInit(struct ArmMem* physMem, struct PxaIc* ic, struct PxaDma* dma);

void pxaMmcInsert(struct PxaMmc* mmc, struct VSD* vsd);  // NULL also acceptable

template <typename T>
void pxaMmcSave(struct PxaMmc* mmc, T& savestate);

template <typename T>
void pxaMmcLoad(struct PxaMmc* mmc, T& loader);

#endif
