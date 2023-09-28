//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _KEYS_H_
#define _KEYS_H_

#include <stdbool.h>
#include <stdint.h>

#include "soc_GPIO.h"

struct Keypad;

struct Keypad *keypadInit(struct SocGpio *gpio, bool matrixHasPullUps);
bool keypadDefineRow(struct Keypad *kp, unsigned rowIdx, int8_t gpio);
bool keypadDefineCol(struct Keypad *kp, unsigned colIdx, int8_t gpio);
bool keypadAddGpioKey(struct Keypad *kp, uint32_t sdlKey, int8_t gpioNum, bool activeHigh);
bool keypadAddMatrixKey(struct Keypad *kp, uint32_t sdlKey, unsigned row, unsigned col);

void keypadSdlKeyEvt(struct Keypad *kp, uint32_t sdlKey, bool wentDown);

#endif
