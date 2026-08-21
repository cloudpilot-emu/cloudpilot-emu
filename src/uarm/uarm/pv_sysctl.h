#ifndef _PV_SYSCTL_H_
#define _PV_SYSCTL_H_

#include <cstdint>

class SoC;
struct PvSysctl;
struct ArmMem;

PvSysctl* pvSysctlInit(ArmMem* mem, SoC* soc, uint32_t ramSize);

#endif  // _PV_SYSCTL_H_
