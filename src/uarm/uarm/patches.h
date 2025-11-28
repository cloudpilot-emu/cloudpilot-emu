#ifndef _PATCHES_H_
#define _PATCHES_H_

#include "patch_dispatch.h"
#include "syscall_dispatch.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ArmCpu;
struct PatchContext;
struct SystemState;

struct PatchContext* registerPatches(struct PatchDispatch* patchDispatch,
                                     struct SyscallDispatch* syscallDispatch, struct ArmCpu* cpu,
                                     struct SystemState* systemState);

#ifdef __cplusplus
}
#endif

#endif  // _PATCHES_H_
