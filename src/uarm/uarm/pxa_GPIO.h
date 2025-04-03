#ifndef _PXA_GPIO_H_
#define _PXA_GPIO_H_

#include "soc_GPIO.h"

#ifdef __cplusplus
template <typename T>
void pxaGpioSave(struct SocGpio* gpio, T& savestate);

template <typename T>
void pxaGpioLoad(struct SocGpio* gpio, T& loader);
#endif

#endif  // _PXA_GPIO_H_