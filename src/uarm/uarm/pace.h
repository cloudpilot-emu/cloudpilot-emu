#ifndef _PACE_H_
#define _PACE_H_

#include <stdbool.h>
#include <stdint.h>

#include "MMU.h"
#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PaceScratchState;

enum paceStatus {
    pace_status_ok = 0,
    pace_status_illegal_instr = 4,
    pace_status_division_by_zero = 5,
    pace_status_trap0 = 32,
    pace_status_trap8 = 40,
    pace_status_syscall = 47,
    pace_status_line_1111 = 11,
    pace_status_line_1010 = 10,
    // There are reserved on 68k
    pace_status_unimplemented_instr = 48,
    pace_status_memory_fault = 49,
    pace_status_return = 50,
};

void paceInit(struct ArmMem* mem, struct ArmMmu* mmu);

void paceSetStatePtr(uint32_t addr);
void paceResetFsr();
uint8_t paceGetFsr();
uint16_t paceGetLastOpcode();

uint8_t paceGet8(uint32_t addr);
uint16_t paceGet16(uint32_t addr);
uint32_t paceGet32(uint32_t addr);

void paceSet8(uint32_t addr, uint8_t value);
void paceSet16(uint32_t addr, uint16_t value);
void paceSet32(uint32_t addr, uint32_t value);

void pacePush8(uint8_t value);
void pacePush16(uint16_t value);
void pacePush32(uint32_t value);

void paceSetPriviledged(bool priviledged);

bool paceLoad68kState();
bool paceSave68kState();

void paceGetMemeryFault(uint32_t* addr, bool* wasWrite, uint_fast8_t* fsr);
uint16_t paceReadTrapWord();

uint32_t paceGetPC();
void paceSetPC(uint32_t pc);

uint32_t paceGetDreg(uint8_t index);
void paceSetDreg(uint8_t index, uint32_t value);

uint32_t paceGetAreg(uint8_t index);
void paceSetAreg(uint8_t index, uint32_t value);

enum paceStatus paceExecute();

struct PaceScratchState* pacePrepareInjectedCall(struct PaceScratchState* scratchState);
void paceFinishInjectedCall(struct PaceScratchState* scratchState);

#ifdef __cplusplus
}

template <typename T>
void paceSave(T& savestate);

template <typename T>
void paceLoad(T& loader);
#endif

#endif  // _PACE_H_
