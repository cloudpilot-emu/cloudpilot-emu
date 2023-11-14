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
    uae_status_ok,
    uae_status_return,
    uae_status_trap0,
    uae_status_trap8,
    uae_status_syscall,
    uae_status_line_1111,
    uae_status_line_1010,
    uae_status_illegal_instr,
    uae_status_unimplemented_instr,
    uae_status_division_by_zero,
    uae_memory_fault
};

void uaeInit(struct ArmMem* mem, struct ArmMmu* mmu);

bool uaeLoad68kState(uint32_t addr);
bool uaeSave68kState(uint32_t addr);

uint8_t uaeGetFsr();

enum uaeStatus uaeExecute();

#ifdef __cplusplus
}
#endif

#endif  // _UAE_EXEC_H_
