#include "app_launcher.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cputil.h"
#include "syscall_dispatch.h"

#define MAX_NAME_LEN 31
#define sysAppLaunchCmdNormalLaunch 0

bool launchAppByName(struct SyscallDispatch* sd, const char* name) {
    const size_t nameLen = strlen(name);
    if (nameLen > MAX_NAME_LEN) {
        fprintf(stderr, "failed to launch app: name length exceeds limit\n");
        return false;
    }

    if (!syscallDispatchM68kSupport(sd)) {
        fprintf(stderr, "failed to launch app: m68k syscalls not supported\n");
        return false;
    }

    if (!syscallDispatchPrepare(sd)) {
        fprintf(stderr, "failed to launch app: unable to prepare syscall\n");
        return false;
    }

    const uint32_t scratch = syscall68k_MemPtrNew(sd, SC_EXECUTE_PURE, MAX_NAME_LEN + 1);
    if (!scratch) {
        fprintf(stderr, "failed to launch app: unable to allocate scratch space\n");
        return false;
    }

    if (!syscallDispatch_memcpy_fromHost(sd, scratch, name, nameLen + 1)) {
        ERR("failed to copy application name to memory");
    }

    const uint32_t dbID = syscall68k_DmFindDatabase(sd, SC_EXECUTE_FULL, 0, scratch);
    bool result = false;

    if (!dbID) {
        fprintf(stderr, "failed to launch app: %s not found\n", name);
    } else {
        result = syscall68k_SysUIAppSwitch(sd, SC_EXECUTE_FULL, 0, dbID,
                                           sysAppLaunchCmdNormalLaunch, 0) == 0;
    }

    if (syscall68k_MemPtrFree(sd, SC_EXECUTE_PURE, scratch) != 0)
        ERR("failed to release scratch memory");

    return result;
}
