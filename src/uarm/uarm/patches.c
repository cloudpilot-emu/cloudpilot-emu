#include "patches.h"

#include <stdio.h>

#include "syscall.h"

static void headpatch_SysSetAutoOffTime(void* ctx, uint32_t syscall, uint32_t* registers) {
    fprintf(stderr, "overriding auto off %u -> 0\n", registers[0]);

    registers[0] = 0;
}

void registerPatches(struct PatchDispatch* patchDispatch, struct SyscallDispatch* syscallDispatch) {
    patchDispatchAddPatch(patchDispatch, SYSCALL_SYS_SET_AUTO_OFF_TIME, headpatch_SysSetAutoOffTime,
                          NULL, NULL);
}
