#ifndef _PATCHES_H_
#define _PATCHES_H_

#include "patch_dispatch.h"

#ifdef __cplusplus
extern "C" {
#endif

void registerPatches(struct PatchDispatch* patchDispatch);

#ifdef __cplusplus
}
#endif

#endif  // _PATCHES_H_
