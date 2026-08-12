//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_PWR_CLK_H_
#define _PXA_PWR_CLK_H_

#include "CPU.h"
#include "mem.h"

class SoC;
struct PxaPwrClk;

struct PxaPwrClk* pxaPwrClkInit(struct ArmCpu* cpu, struct ArmMem* physMem, class SoC* soc,
                                bool isPXA270);

void pxaPwrClkReset(struct PxaPwrClk* pc);

template <typename T>
void pxaPwrClkSave(struct PxaPwrClk* pc, T& savestate);

template <typename T>
void pxaPwrClkLoad(struct PxaPwrClk* pc, T& loader);

#endif
