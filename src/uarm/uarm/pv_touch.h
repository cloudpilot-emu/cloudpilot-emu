#ifndef _PV_TOUCH_H_
#define _PV_TOUCH_H_

#include <cstdint>

struct PvTouch;
struct PvIc;
struct ArmMem;

PvTouch* pvTouchInit(ArmMem* mem, PvIc* ic);

void pvTouchUpdate(PvTouch* touch, int16_t x, int16_t y);

template <typename T>
void pvTouchSave(struct PvTouch* touch, T& savestate);

template <typename T>
void pvTouchLoad(struct PvTouch* touch, T& loader);

#endif  // _PV_TOUCH_H_
