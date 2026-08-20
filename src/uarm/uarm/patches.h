#ifndef _PATCHES_H_
#define _PATCHES_H_

#include "patch_dispatch.h"
#include "syscall_dispatch.h"

struct ArmCpu;
struct PatchContext;
struct SystemState;

#define PATCH_FEATURE_TOUCH_TRANSLATION 0x01
#define PATCH_FEATURE_NO_SLEEP 0x02
#define PATCH_FEATURE_ALL 0xffffffff

struct PatchContext* registerPatches(struct PatchDispatch* patchDispatch,
                                     struct SyscallDispatch* syscallDispatch, struct ArmCpu* cpu,
                                     struct SystemState* systemState,
                                     uint32_t features = PATCH_FEATURE_ALL);

#endif  // _PATCHES_H_
