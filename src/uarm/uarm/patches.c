#include "patches.h"

#include <stdio.h>

#include "syscall.h"

static void tailpatch_UIInitialize(void* ctx, uint32_t syscall,
                                   const uint32_t* registersAtinvocation, uint32_t* registers) {
    fprintf(stderr, "UI initialized\n");

    struct SyscallDispatch* sd = (struct SyscallDispatch*)ctx;
    syscall_SysSetAutoOffTime(sd, 0);
}

static void headpatch_SysSetAutoOffTime(void* ctx, uint32_t syscall, uint32_t* registers) {
    registers[0] = 0;
}

void registerPatches(struct PatchDispatch* patchDispatch, struct SyscallDispatch* syscallDispatch) {
    patchDispatchAddPatch(patchDispatch, SYSCALL_UI_INITIALIZE, NULL, tailpatch_UIInitialize,
                          syscallDispatch);

    patchDispatchAddPatch(patchDispatch, SYSCALL_SYS_SET_AUTO_OFF_TIME, headpatch_SysSetAutoOffTime,
                          NULL, NULL);
}
