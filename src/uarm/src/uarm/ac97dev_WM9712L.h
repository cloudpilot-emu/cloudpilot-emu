//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _AC97_WM9712L_H_
#define _AC97_WM9712L_H_

#include <stdbool.h>
#include <stdint.h>

#include "soc_AC97.h"
#include "soc_GPIO.h"

#ifdef __cplusplus
extern "C" {
#endif

struct WM9712L;

struct AudioQueue;

enum WM9712LauxPin {  // Vref is 3.3v,
    WM9712LauxPinAux1 =
        0,  // keep in mind this is divided by 3, so if battery is 3V, pass 1V to this
    WM9712LauxPinAux2,
    WM9712LauxPinBmon,
    WM9712LauxPinAux4,
};

struct WM9712L *wm9712LInit(struct SocAC97 *ac97, struct SocGpio *gpio, int8_t penDownPin);
void wm9712Lperiodic(struct WM9712L *wm);

void wm9712LsetAuxVoltage(struct WM9712L *wm, enum WM9712LauxPin which, uint32_t mV);
void wm9712LsetPen(struct WM9712L *wm, int16_t x, int16_t y,
                   int16_t press);  // raw ADC values, negative for pen up

void wm9712LsetAudioQueue(struct WM9712L *wm, struct AudioQueue *audioQueue);

#ifdef __cplusplus
}
#endif

#endif
