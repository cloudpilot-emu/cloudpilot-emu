#ifndef _PATCHES_H_
#define _PATCHES_H_

#include "patch_dispatch.h"
#include "syscall_dispatch.h"

struct ArmCpu;
struct PatchContext;
struct SystemState;

struct PatchContext* registerPatches(struct PatchDispatch* patchDispatch,
                                     struct SyscallDispatch* syscallDispatch, struct ArmCpu* cpu,
                                     struct SystemState* systemState);

#endif  // _PATCHES_H_
