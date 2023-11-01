#include "patches.h"

#include <stdio.h>

#include "syscall.h"

static void headpatch_UIInitialize(void* ctx, uint32_t syscall, uint32_t* registers) {
    printf("headpatch UIInitialize\n");
}

static void tailpatch_UIInitialize(void* ctx, uint32_t syscall,
                                   const uint32_t* registersAtinvocation, uint32_t* registers) {
    printf("tailpatch UIInitialize\n");
}

void registerPatches(struct PatchDispatch* patchDispatch) {
    patchDispatchAddPatch(patchDispatch, SYSCALL_UI_INITIALIZE, headpatch_UIInitialize,
                          tailpatch_UIInitialize, NULL);
}
