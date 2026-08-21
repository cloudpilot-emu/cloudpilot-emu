#ifndef _PV_TIMER_H_
#define _PV_TIMER_H_

#include <cstdint>

struct PvTimer;
struct ArmMem;
struct PvIc;

PvTimer* pvTimerInit(ArmMem* mem, PvIc* ic);

void pvTimerTick(PvTimer* timer, uint32_t ticks);

void pvTimerSuspendInterrupts(PvTimer* timer, bool suspend);

uint32_t pvTimerTicksToNextInterrupt(PvTimer* timer);

template <typename T>
void pvTimerSave(struct PvTimer* timer, T& savestate);

template <typename T>
void pvTimerLoad(struct PvTimer* timer, T& loader);

#endif  // _PV_TIMER_H_
