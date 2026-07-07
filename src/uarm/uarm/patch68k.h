#ifndef _PATCH_68K_H_
#define _PATCH_68K_H_

#include <cstdbool>
#include <cstdint>

#include "device_type5.h"

#define PATCH_68K_NVFS 1

void patch68kInit(uint32_t patches);

bool patch68kHandle(uint16_t trapWord);

#endif  // _PATCH_68K_H_
