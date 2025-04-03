#ifndef _PXA_AC97_H_
#define _PXA_AC97_H_

#include "soc_AC97.h"

#ifdef __cplusplus
template <typename T>
void pxaAC97Save(struct SocAC97* ac97, T& savestate);

template <typename T>
void pxaAC97Load(struct SocAC97* ac97, T& loader);
#endif

#endif  // _PXA_AC97_H_