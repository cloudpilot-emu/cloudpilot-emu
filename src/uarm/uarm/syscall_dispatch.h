#ifndef _SYSCALL_DISPATCH_H_
#define _SYSCALL_DISPATCH_H_

#include <stdint.h>

#include "SoC.h"

#ifdef __cplusplus

    #include <functional>

extern "C" {
#endif

#define SC_EXECUTE_PURE 0
#define SC_EXECUTE_FULL 1

struct SyscallDispatch;
struct ArmCpu;

struct SyscallDispatch* initSyscallDispatch(struct SoC* cpu);

bool syscallDispatchInProgress(struct SyscallDispatch* sd);

bool syscallDispatchM68kSupport(struct SyscallDispatch* sd);
bool syscallDispatchPrepare(struct SyscallDispatch* sd);

bool syscallDispatch_strncpy_toHost(struct SyscallDispatch* sd, void* dest, uint32_t src,
                                    size_t maxlen);
bool syscallDispatch_memcpy_fromHost(struct SyscallDispatch* sd, uint32_t dest, const void* src,
                                     size_t size);
bool syscallDispatch_memcpy_toHost(struct SyscallDispatch* sd, void* dest, uint32_t src,
                                   size_t size);

uint16_t syscall_SysSetAutoOffTime(struct SyscallDispatch* sd, uint32_t flags, uint32_t timeout);

uint16_t syscall_MemPtrNew(struct SyscallDispatch* sd, uint32_t flags, uint32_t size);

uint16_t syscall_MemPtrFree(struct SyscallDispatch* sd, uint32_t flags, uint32_t ptr);

uint16_t syscall_FtrGet(struct SyscallDispatch* sd, uint32_t flags, uint32_t creator,
                        uint16_t ftrNum, uint32_t valueP);

uint32_t syscall68k_SysGetOsVersionString(struct SyscallDispatch* sd, uint32_t flags);
uint32_t syscall68k_MemPtrNew(struct SyscallDispatch* sd, uint32_t flags, uint32_t size);
uint16_t syscall68k_MemPtrFree(struct SyscallDispatch* sd, uint32_t flags, uint32_t memPtr);
uint16_t syscall68k_MemNumCards(struct SyscallDispatch* sd, uint32_t flags);
uint16_t syscall68k_ExgDBRead(struct SyscallDispatch* sd, uint32_t flags, uint32_t readProcP,
                              uint32_t deleteProcP, uint32_t userDataP, uint32_t dbIDP,
                              uint16_t cardNo, uint32_t needsResetP, bool keepDates);
uint16_t syscall68k_ExgDBWrite(struct SyscallDispatch* sd, uint32_t flags, uint32_t writeProcP,
                               uint32_t userDataP, uint32_t nameP, uint32_t dbID, uint16_t cardNo);
uint16_t syscall68k_DmNumDatabases(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo);
uint32_t syscall68k_DmGetDatabase(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                  uint16_t index);
uint16_t syscall68k_DmDatabaseInfo(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                   uint32_t dbId, uint32_t nameP, uint32_t attributesP,
                                   uint32_t versionP, uint32_t crDateP, uint32_t modDateP,
                                   uint32_t bcpUpDateP, uint32_t modNumP, uint32_t appInfoIDP,
                                   uint32_t sortInfoIDP, uint32_t typeP, uint32_t creatorP);
uint16_t syscall68k_DmDatabaseProtect(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                      uint32_t dbID, uint8_t protect);
uint16_t syscall68k_DmDeleteDatabase(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                     uint32_t dbID);
uint32_t syscall68k_DmFindDatabase(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                   uint32_t nameP);
uint16_t syscall68k_SysUIAppSwitch(struct SyscallDispatch* sd, uint32_t flags, uint16_t cardNo,
                                   uint32_t dbID, uint16_t cmd, uint32_t cmdPBP);

#ifdef __cplusplus
}

using M68kStub = std::function<void(struct ArmCpu*, uint32_t parameterBase,
                                    std::function<void()> deadMansSwitch)>;
void syscallDispatchRegisterM68kStub(struct SyscallDispatch* sd, uint32_t trampoline,
                                     M68kStub stub);
void syscallDispatchUnregisterM68kStub(struct SyscallDispatch* sd, uint32_t trampoline);

#endif

#endif  // _SYSCALL_DISPATCH_H_
