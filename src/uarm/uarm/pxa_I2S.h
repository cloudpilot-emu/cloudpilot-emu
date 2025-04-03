#ifndef _PXA_I2S_H_
#define _PXA_I2S_H_

#include "soc_I2S.h"

#ifdef __cplusplus
template <typename T>
void pxaI2sSave(struct SocI2s* i2s, T& savestate);

template <typename T>
void pxaI2sLoad(struct SocI2s* i2s, T& loader);
#endif

#endif  // _PXA_I2S_H_