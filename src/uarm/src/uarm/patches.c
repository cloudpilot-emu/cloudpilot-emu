#include "patches.h"

#include <stdio.h>

#include "syscall.h"

static void tailpatch_UIInitialize(void* ctx, uint32_t syscall,
                                   const uint32_t* registersAtinvocation, uint32_t* registers) {
    fprintf(stderr, "UI initialized\n");

    struct SyscallDispatch* sd = (struct SyscallDispatch*)ctx;
    syscall_SysSetAutoOffTime(sd, 0);
}

void registerPatches(struct PatchDispatch* patchDispatch, struct SyscallDispatch* syscallDispatch) {
    patchDispatchAddPatch(patchDispatch, SYSCALL_UI_INITIALIZE, NULL, tailpatch_UIInitialize,
                          syscallDispatch);
}
