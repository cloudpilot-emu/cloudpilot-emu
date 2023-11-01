#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SYSCALL_UI_INITIALIZE 0xc55c
#define SYSCALL_SYS_SET_AUTO_OFF_TIME 0x88c8

#define packSyscall(table, offset) ((table << 12) | offset)

const char* getSyscallName(uint32_t syscall);

#ifdef __cplusplus
}
#endif

#endif  // _SYSCALLS_H_
