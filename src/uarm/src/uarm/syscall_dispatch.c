#include "syscall_dispatch.h"

#include <string.h>

#include "CPU.h"
#include "syscall.h"
#include "util.h"

#define MAX_NEST_LEVEL 4

struct SyscallDispatch {
    struct ArmCpu* cpu;

    struct ArmCpu* scratchStates[MAX_NEST_LEVEL];
    uint32_t nestLevel;
};

struct SyscallDispatch* initSyscallDispatch(struct ArmCpu* cpu) {
    struct SyscallDispatch* sd = malloc(sizeof(*sd));
    memset(sd, 0, sizeof(*sd));

    sd->cpu = cpu;

    return sd;
}

static struct ArmCpu* allocateScratchState(struct SyscallDispatch* sd) {
    if (sd->nestLevel >= MAX_NEST_LEVEL)
        ERR("unable to dispatch syscall: max nest level reached\n");

    const uint32_t nestLevel = sd->nestLevel++;
    return sd->scratchStates[nestLevel] =
               cpuPrepareInjectedCall(sd->cpu, sd->scratchStates[nestLevel]);
}

uint16_t syscall_SysSetAutoOffTime(struct SyscallDispatch* sd, uint32_t timeout) {
    struct ArmCpu* scratchState = allocateScratchState(sd);
    uint32_t* registers = cpuGetRegisters(scratchState);

    registers[0] = timeout;
    cpuExecuteInjectedCall(scratchState, SYSCALL_SYS_SET_AUTO_OFF_TIME);

    uint16_t err = registers[0];

    cpuFinishInjectedCall(sd->cpu, scratchState);
    sd->nestLevel--;

    return err;
}
