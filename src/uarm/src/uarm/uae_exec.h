#ifndef _UAE_EXEC_H_
#define _UAE_EXEC_H_

#include <stdbool.h>
#include <stdint.h>

#include "MMU.h"
#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

enum uaeStatus {
    uae_status_ok = 0,
    uae_status_illegal_instr = 4,
    uae_status_division_by_zero = 7,
    uae_status_trap0 = 32,
    uae_status_trap8 = 40,
    uae_status_syscall = 47,
    uae_status_line_1111 = 0x0200,
    uae_status_line_1010 = 0x0300,
    uae_status_unimplemented_instr = 0x0100,
    uae_status_memory_fault = 0x0400,
    uae_status_return = 0x1000,
};

void uaeInit(struct ArmMem* mem, struct ArmMmu* mmu);

bool uaeLoad68kState(uint32_t addr);
bool uaeSave68kState(uint32_t addr);

uint8_t uaeGetFsr();
uint16_t readTrapWord();

void uaeSetPriviledged(bool priviledged);
enum uaeStatus uaeExecute();

#ifdef __cplusplus
}
#endif

#endif  // _UAE_EXEC_H_
