#ifndef _PXA_GPIO_H_
#define _PXA_GPIO_H_

#include "CPU.h"
#include "mem.h"
#include "pxa_IC.h"

struct PxaGpio;

typedef void (*GpioChangedNotifF)(void* userData, uint32_t gpio, bool oldState, bool newState);
typedef void (*GpioDirsChangedF)(void* userData);

enum PxaGpioState {  // these values make it look like all HiZ, AFR, and nonexistent pins have
                     // pullups to those who dumbly assume pxaGpioGetState returns a boolean

    PxaGpioStateLow,
    PxaGpioStateHigh,
    PxaGpioStateHiZ,
    PxaGpioStateAFR0,  // AFR values must be in order
    PxaGpioStateAFR1,
    PxaGpioStateAFR2,
    PxaGpioStateAFR3,
    PxaGpioStateNoSuchGpio,
};

struct PxaGpio* pxaGpioInit(struct ArmMem* physMem, struct PxaIc* ic, uint_fast8_t socRev);

// for external use :)
enum PxaGpioState pxaGpioGetState(struct PxaGpio* gpio, uint_fast8_t gpioNum);
void pxaGpioSetState(struct PxaGpio* gpio, uint_fast8_t gpioNum,
                     bool on);  // we can only set value (and only of input pins), not direction

// only for output pins!
void pxaGpioSetNotif(struct PxaGpio* gpio, uint_fast8_t gpioNum, GpioChangedNotifF notifF,
                     void* userData);  // one per pin. set ot NULL to disable

// for all (but only one notifier)
void pxaGpioSetDirsChangedNotif(struct PxaGpio* gpio, GpioDirsChangedF notifF, void* userData);

template <typename T>
void pxaGpioSave(struct PxaGpio* gpio, T& savestate);

template <typename T>
void pxaGpioLoad(struct PxaGpio* gpio, T& loader);

#endif  // _PXA_GPIO_H_
