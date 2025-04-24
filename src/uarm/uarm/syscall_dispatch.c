#include "syscall_dispatch.h"

#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "cputil.h"
#include "pace.h"
#include "syscall.h"

#define MAX_NEST_LEVEL 4

struct SyscallDispatch {
    struct ArmCpu* cpu;

    struct ArmCpu* scratchStates[MAX_NEST_LEVEL];
    struct PaceScratchState* scratchStatesPace[MAX_NEST_LEVEL];
    size_t nestLevel;
};

struct SyscallDispatch* initSyscallDispatch(struct ArmCpu* cpu) {
    struct SyscallDispatch* sd = malloc(sizeof(*sd));
    memset(sd, 0, sizeof(*sd));

    sd->cpu = cpu;

    return sd;
}

static size_t allocateScratchState(struct SyscallDispatch* sd) {
    if (sd->nestLevel >= MAX_NEST_LEVEL)
        ERR("unable to dispatch syscall: max nest level reached\n");

    const uint32_t nestLevel = sd->nestLevel++;

    sd->scratchStates[nestLevel] = cpuPrepareInjectedCall(sd->cpu, sd->scratchStates[nestLevel]);
    sd->scratchStatesPace[nestLevel] = pacePrepareInjectedCall(sd->scratchStatesPace[nestLevel]);

    return nestLevel;
}

uint16_t syscall_SysSetAutoOffTime(struct SyscallDispatch* sd, uint32_t timeout) {
    const size_t nestLevel = allocateScratchState(sd);
    uint32_t* registers = cpuGetRegisters(sd->scratchStates[nestLevel]);

    registers[0] = timeout;
    cpuExecuteInjectedCall(sd->scratchStates[nestLevel], SYSCALL_SYS_SET_AUTO_OFF_TIME);

    uint16_t err = registers[0];

    cpuFinishInjectedCall(sd->cpu, sd->scratchStates[nestLevel]);
    paceFinishInjectedCall(sd->scratchStatesPace[nestLevel]);
    sd->nestLevel--;

    return err;
}
