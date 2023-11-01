#ifndef _PATCHES_H_
#define _PATCHES_H_

#include "patch_dispatch.h"
#include "syscall_dispatch.h"

#ifdef __cplusplus
extern "C" {
#endif

void registerPatches(struct PatchDispatch* patchDispatch, struct SyscallDispatch* syscallDispatch);

#ifdef __cplusplus
}
#endif

#endif  // _PATCHES_H_
