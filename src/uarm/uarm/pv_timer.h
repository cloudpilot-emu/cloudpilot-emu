#ifndef _PV_TIMER_H_
#define _PV_TIMER_H_

struct PvTimer;
struct ArmMem;
struct PvIc;

PvTimer* pvTimerInit(ArmMem* mem, PvIc* ic);

void pvTimerTick(PvTimer* timer);

void pvTimerSuspendInterrupts(PvTimer* timer, bool suspend);

template <typename T>
void pvTimerSave(struct PvTimer* timer, T& savestate);

template <typename T>
void pvTimerLoad(struct PvTimer* timer, T& loader);

#endif  // _PV_TIMER_H_
