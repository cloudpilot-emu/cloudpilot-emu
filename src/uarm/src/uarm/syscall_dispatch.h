#ifndef _SYSCALL_DISPATCH_H_
#define _SYSCALL_DISPATCH_H_

#include <stdint.h>

#include "CPU.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SyscallDispatch;

struct SyscallDispatch* initSyscallDispatch(struct ArmCpu* cpu);

uint16_t syscall_SysSetAutoOffTime(struct SyscallDispatch* sd, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif  // _SYSCALL_DISPATCH_H_
