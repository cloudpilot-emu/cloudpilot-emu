#ifndef _PATCH_68K_H_
#define _PATCH_68K_H_

#include <stdbool.h>
#include <stdint.h>

#include "device_type5.h"

#define PATCH_68K_NVFS 1

#ifdef __cplusplus
extern "C" {
#endif

void patch68kInit(uint32_t patches);

bool patch68kHandle(uint16_t trapWord);

#ifdef __cplusplus
}
#endif

#endif  // _PATCH_68K_H_