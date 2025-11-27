#include "patches.h"

#include <stdio.h>

#include "CPU.h"
#include "stdlib.h"
#include "syscall.h"

struct PatchContext {
    struct ArmCpu* cpu;
    struct SyscallDispatch* sd;
};

static bool headpatch_SysSetAutoOffTime(void* ctx, uint32_t syscall, uint32_t* registers) {
    fprintf(stderr, "overriding auto off %u -> 0\n", registers[0]);
    registers[0] = 0;

    return true;
}

static bool headpatch_PenRawToScreen(void* ctx, uint32_t syscall, uint32_t* registers) {
    struct PatchContext* patchContext = ctx;

    const uint32_t pointP = registers[0];
    int16_t x, y;

    cpuMemOpExternal(patchContext->cpu, &x, pointP, 2, false);
    cpuMemOpExternal(patchContext->cpu, &y, pointP + 2, 2, false);

    if (x != 0x3fff) x = (x - 280) / 10;
    if (y != 0x3fff) y = (y - 210) / 7;

    cpuMemOpExternal(patchContext->cpu, &x, pointP, 2, true);
    cpuMemOpExternal(patchContext->cpu, &y, pointP + 2, 2, true);

    return false;
}

static bool headpatch_PenScreenToRaw(void* ctx, uint32_t syscall, uint32_t* registers) {
    struct PatchContext* patchContext = ctx;

    const uint32_t pointP = registers[0];
    int16_t x, y;

    cpuMemOpExternal(patchContext->cpu, &x, pointP, 2, false);
    cpuMemOpExternal(patchContext->cpu, &y, pointP + 2, 2, false);

    if (x != 0x3fff) x = 280 + x * 10;
    if (y != 0x3fff) y = 210 + y * 7;

    cpuMemOpExternal(patchContext->cpu, &x, pointP, 2, true);
    cpuMemOpExternal(patchContext->cpu, &y, pointP + 2, 2, true);

    return false;
}

static bool headpatch_SysSleep(void* ctx, uint32_t syscall, uint32_t* registers) {
    printf("sys sleep aborted\n");
    registers[0] = 0;

    return false;
}

static void tailpatch_uiInitialize(void* ctx, uint32_t syscall,
                                   const uint32_t* registersAtinvocation, uint32_t* registers) {
    printf("ui initialized \n");

    struct PatchContext* patchContext = ctx;

    registers[13] -= 4;
    const uint32_t versionP = registers[13];

    if (syscall_FtrGet(patchContext->sd, SC_EXECUTE_PURE, 'psys', 1, versionP) != 0) {
        printf("failed to get OS version\n");
    } else {
        uint32_t version;

        if (!cpuMemOpExternal(patchContext->cpu, &version, versionP, 4, false)) {
            printf("failed to dereference OS version pointer\n");
        } else {
            printf("OS version: 0x%08x\n", version);
        }
    }

    registers[13] += 4;
}

struct PatchContext* registerPatches(struct PatchDispatch* patchDispatch,
                                     struct SyscallDispatch* syscallDispatch, struct ArmCpu* cpu) {
    struct PatchContext* ctx = (struct PatchContext*)malloc(sizeof(struct PatchContext));
    ctx->cpu = cpu;
    ctx->sd = syscallDispatch;

    patchDispatchAddPatch(patchDispatch, SYSCALL_SYS_SET_AUTO_OFF_TIME, headpatch_SysSetAutoOffTime,
                          NULL, ctx);

    patchDispatchAddPatch(patchDispatch, SYSCALL_HAL_PEN_RAW_TO_SCREEN, headpatch_PenRawToScreen,
                          NULL, ctx);

    patchDispatchAddPatch(patchDispatch, SYSCALL_HAL_PEN_SCREEN_TO_RAW, headpatch_PenScreenToRaw,
                          NULL, ctx);

    patchDispatchAddPatch(patchDispatch, SYSCALL_SYS_SLEEP, headpatch_SysSleep, NULL, ctx);

    patchDispatchAddPatch(patchDispatch, SYSCALL_UI_INITIALIZE, NULL, tailpatch_uiInitialize, ctx);

    return ctx;
}  //
