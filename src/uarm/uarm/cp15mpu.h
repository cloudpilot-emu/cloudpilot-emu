#ifndef _CP15_MPU_H_
#define _CP15_MPU_H_

#include <cstdint>

struct ArmCP15MPU;
struct ArmCpu;
struct ArmMpu;
struct icache;

struct ArmCP15MPU* cp15MPUInit(struct ArmCpu* cpu, struct ArmMpu* mpu, struct icache* ic,
                               uint32_t cpuid, uint32_t cacheId);

template <typename T>
void cp15MPUSave(struct ArmCP15MMU* cp15, T& savestate);

template <typename T>
void cp15MPULoad(struct ArmCP15MPU* cp15, T& loader);

#endif  // _CP15_MPU_H_
