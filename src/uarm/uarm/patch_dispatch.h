#ifndef _PATCH_DISPATCH_H_
#define _PATCH_DISPATCH_H_

#include <stdint.h>

#include "CPU.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*HeadpatchF)(void* ctx, uint32_t syscall, uint32_t* registers);
typedef void (*TailpatchF)(void* ctx, uint32_t syscall, const uint32_t* registersAtinvocation,
                           uint32_t* registers);

struct PatchDispatch;
struct ArmCpu;

struct PatchDispatch* initPatchDispatch();
void destroyPatchDispatch(struct PatchDispatch* pd);

void patchDispatchOnLoadR12FromR9(struct PatchDispatch* pd, int32_t offset);
void patchDispatchOnLoadPcFromR12(struct PatchDispatch* pd, int32_t offset, uint32_t* registers);
void patchOnBeforeExecute(struct PatchDispatch* pd, uint32_t* registers);

void patchDispatchAddPatch(struct PatchDispatch* pd, uint32_t syscall, HeadpatchF headpatch,
                           TailpatchF tailpatch, void* ctx);

void patchDispatchSetCpu(struct PatchDispatch* pd, struct ArmCpu* cpu);

#ifdef __cplusplus
}

template <typename T>
void patchDispatchSave(struct PatchDispatch* pd, T& savestate);

template <typename T>
void patchDispatchLoad(struct PatchDispatch* pd, T& savestate);
#endif

#endif  // _PATCH_DISPATCH_H_
