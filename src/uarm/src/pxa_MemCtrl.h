//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_MEM_CTRL_H_
#define _PXA_MEM_CTRL_H_

#include <stdint.h>

#include "mem.h"


struct PxaMemCtrlr;

struct PxaMemCtrlr* pxaMemCtrlrInit(struct ArmMem* physMem, uint_fast8_t socRev);

#endif
