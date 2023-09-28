//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_MMC_H_
#define _PXA_MMC_H_

#include "CPU.h"
#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"
#include "vSD.h"

struct PxaMmc;

struct PxaMmc* pxaMmcInit(struct ArmMem* physMem, struct SocIc* ic, struct SocDma* dma);

void pxaMmcInsert(struct PxaMmc* mmc, struct VSD* vsd);  // NULL also acceptable

#endif
