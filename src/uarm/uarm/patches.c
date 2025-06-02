#include "patches.h"

#include <stdio.h>

#include "CPU.h"
#include "syscall.h"

static bool headpatch_SysSetAutoOffTime(void* ctx, uint32_t syscall, uint32_t* registers) {
    fprintf(stderr, "overriding auto off %u -> 0\n", registers[0]);

    registers[0] = 0;

    return true;
}

static bool headpatch_PenRawToScreen(void* ctx, uint32_t syscall, uint32_t* registers) {
    struct ArmCpu* cpu = ctx;

    const uint32_t pointP = registers[0];
    int16_t x, y;

    cpuMemOpExternal(cpu, &x, pointP, 2, false);
    cpuMemOpExternal(cpu, &y, pointP + 2, 2, false);

    if (x != 0x3fff) x = (x - 280) / 10;
    if (y != 0x3fff) y = (y - 210) / 7;

    cpuMemOpExternal(cpu, &x, pointP, 2, true);
    cpuMemOpExternal(cpu, &y, pointP + 2, 2, true);

    return false;
}

void registerPatches(struct PatchDispatch* patchDispatch, struct SyscallDispatch* syscallDispatch,
                     struct ArmCpu* cpu) {
    patchDispatchAddPatch(patchDispatch, SYSCALL_SYS_SET_AUTO_OFF_TIME, headpatch_SysSetAutoOffTime,
                          NULL, NULL);

    patchDispatchAddPatch(patchDispatch, SYSCALL_HAL_PEN_RAW_TO_SCREEN, headpatch_PenRawToScreen,
                          NULL, cpu);
}
