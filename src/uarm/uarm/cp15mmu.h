//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _CP15_MMU_H_
#define _CP15_MMU_H_

#include <cstdint>

#include "CPU.h"
#include "MMU.h"
#include "icache.h"

struct ArmCP15MMU;

struct ArmCP15MMU* cp15MMUInit(struct ArmCpu* cpu, struct ArmMmu* mmu, struct icache* ic,
                               uint32_t cpuid, uint32_t cacheId, bool xscale, bool omap);
void cp15MMUSetFaultStatus(struct ArmCP15MMU* cp15, uint32_t addr, uint_fast8_t faultStatus);
void cp15MMUCycle(struct ArmCP15MMU* cp15);

template <typename T>
void cp15MMUSave(struct ArmCP15MMU* cp15, T& savestate);

template <typename T>
void cp15MMULoad(struct ArmCP15MMU* cp15, T& loader);

#endif  // _CP15_MMU_H_
