#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SYSCALL_UI_INITIALIZE 0xc55c
#define SYSCALL_SYS_SLEEP 0x88d8
#define SYSCALL_SYS_SET_AUTO_OFF_TIME 0x88c8
#define SYSCALL_MEM_PTR_NEW 0x8584
#define SYSCALL_MEM_CHUNK_FREE 0x84b8
#define SYSCALL_FTR_GET 0x8424
#define SYSCALL_HAL_PEN_RAW_TO_SCREEN 0x415c
#define SYSCALL_HAL_PEN_SCREEN_TO_RAW 0x4168

#define packSyscall(table, offset) ((table << 12) | offset)

const char* getSyscallName(uint32_t syscall);

#ifdef __cplusplus
}
#endif

#endif  // _SYSCALLS_H_
