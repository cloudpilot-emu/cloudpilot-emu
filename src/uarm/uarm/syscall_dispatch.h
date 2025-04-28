#ifndef _SYSCALL_DISPATCH_H_
#define _SYSCALL_DISPATCH_H_

#include <stdint.h>

#include "SoC.h"

#ifdef __cplusplus

    #include <functional>

extern "C" {
#endif

struct SyscallDispatch;
struct ArmCpu;

struct SyscallDispatch* initSyscallDispatch(struct SoC* cpu);

bool syscallDispatchInProgress(struct SyscallDispatch* sd);

bool syscallDispatchM68kSupport(struct SyscallDispatch* sd);
bool syscallDispatchPrepare(struct SyscallDispatch* sd);

bool syscallDispatch_strncpy_toHost(struct SyscallDispatch* sd, void* dest, uint32_t src,
                                    size_t maxlen);
bool syscallDispatch_memcpy_fromHost(struct SyscallDispatch* sd, uint32_t dest, void* src,
                                     size_t size);

uint16_t syscall_SysSetAutoOffTime(struct SyscallDispatch* sd, uint32_t timeout);

uint32_t syscall68k_SysGetOsVersionString(struct SyscallDispatch* sd);
uint32_t syscall68k_MemPtrNew(struct SyscallDispatch* sd, uint32_t size);
uint16_t syscall68k_MemPtrFree(struct SyscallDispatch* sd, uint32_t memPtr);
uint16_t syscall68k_ExgDBRead(struct SyscallDispatch* sd, uint32_t readProcP, uint32_t deleteProcP,
                              uint32_t userDataP, uint32_t dbIDP, uint16_t cardNo,
                              uint32_t needsResetP, bool keepDates);
uint16_t syscall68k_DmDeleteDatabase(struct SyscallDispatch* sd, uint16_t cardNo, uint32_t dbID);

#ifdef __cplusplus
}

using M68kStub = std::function<void(struct ArmCpu*, uint32_t parameterBase,
                                    std::function<void()> deadMansSwitch)>;
void syscallDispatchRegisterM68Stub(struct SyscallDispatch* sd, uint32_t trampoline, M68kStub stub);
void syscallDispatchUnregisterM68Stub(struct SyscallDispatch* sd, uint32_t trampoline);

#endif

#endif  // _SYSCALL_DISPATCH_H_
