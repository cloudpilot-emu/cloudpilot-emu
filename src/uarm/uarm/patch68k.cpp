#include "patch68k.h"

#include "pace.h"
#include "syscall_68k.h"

static bool patchNVFS = false;

void patch68kInit(uint32_t patches) { patchNVFS = (patches & PATCH_68K_NVFS) != 0; }

bool patch68kHandle(uint16_t trapWord) {
    // ATM this is a hack, but it currently is more efficient than a bitfield or pointer array,
    // and chances are that that's all we'll ever need.
    if (!patchNVFS) return false;
    if (trapWord < SYSCALL_68K_DM_SYNC || trapWord > SYSCALL_68K_DM_SYNC_DATABASE) return false;

    paceSetDreg(0, 0);
    return true;
}
