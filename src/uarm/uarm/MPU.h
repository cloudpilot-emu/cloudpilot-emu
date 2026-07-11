#ifndef _MPU_H_
#define _MPU_H_

#include <cstdint>

struct ArmMpu;

typedef uint32_t MPUTestResult;

#define MPU_TEST_RESULT_OK(x) (x & 0x01)
#define MPU_TEST_RESULT_CACHEABLE(x) (x & 0x02)

ArmMpu* mpuCreate();

void mpuReset(ArmMpu* mpu);

MPUTestResult mpuTestAddress(ArmMpu* mpu, uint32_t pa, bool write, bool privileged);

void mpuSetEnabled(ArmMpu* mpu, bool enabled);
void mpuSetCacheable(ArmMpu* mpu, uint8_t cacheable);
void mpuSetBufferable(ArmMpu* mpu, uint8_t bufferable);
void mpuSetAP(ArmMpu* mpu, uint16_t ap);
void mpuSetRegionConfig(ArmMpu* mpu, uint8_t region, uint32_t config);

bool mpuIsEnabled(ArmMpu* mpu);
uint8_t mpuGetCacheable(ArmMpu* mpu);
uint8_t mpuGetBufferable(ArmMpu* mpu);
uint16_t mpuGetAP(ArmMpu* mpu);
uint32_t mpuGetRegionConfig(ArmMpu* mpu, uint8_t region);

#endif  // _MPU_H_
