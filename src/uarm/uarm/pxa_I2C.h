#ifndef _PXA_I2C_H_
#define _PXA_I2C_H_

#include "soc_I2C.h"

#ifdef __cplusplus

template <typename T>
void pxaI2cSave(struct SocI2c* i2c, T& savestate, uint32_t index = 0);

template <typename T>
void pxaI2cLoad(struct SocI2c* i2c, T& loader, uint32_t index = 0);

#endif

#endif  // _PXA_I2C_H_