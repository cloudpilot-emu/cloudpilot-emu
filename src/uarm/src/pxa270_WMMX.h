//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA270_WMMX_H_
#define _PXA270_WMMX_H_

#include "CPU.h"
#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Pxa270wmmx;

struct Pxa270wmmx* pxa270wmmxInit(struct ArmCpu* cpu);

#ifdef __cplusplus
}
#endif

#endif
