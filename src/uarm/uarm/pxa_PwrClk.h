//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_PWR_CLK_H_
#define _PXA_PWR_CLK_H_

#include "CPU.h"
#include "SoC_type.h"
#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PxaPwrClk;

struct PxaPwrClk* pxaPwrClkInit(struct ArmCpu* cpu, struct ArmMem* physMem, struct SoC* soc,
                                bool isPXA270);

#ifdef __cplusplus
}
#endif

#endif
