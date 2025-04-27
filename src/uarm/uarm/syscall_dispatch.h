#ifndef _SYSCALL_DISPATCH_H_
#define _SYSCALL_DISPATCH_H_

#include <stdint.h>

#include "SoC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SyscallDispatch;

struct SyscallDispatch* initSyscallDispatch(struct SoC* cpu);

bool syscallDispatchM68kSupport(struct SyscallDispatch* sd);
bool syscallDispatchPrepare(struct SyscallDispatch* sd);

bool syscallDispatch_strncpy_toHost(struct SyscallDispatch* sd, void* dest, uint32_t src,
                                    size_t maxlen);

uint16_t syscall_SysSetAutoOffTime(struct SyscallDispatch* sd, uint32_t timeout);

uint32_t syscall68k_SysGetOsVersionString(struct SyscallDispatch* sd);

uint16_t syscall68k_MemPtrFree(struct SyscallDispatch* sd, uint32_t memPtr);

#ifdef __cplusplus
}
#endif

#endif  // _SYSCALL_DISPATCH_H_
