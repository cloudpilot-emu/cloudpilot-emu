//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_GPIO_H_
#define _SOC_GPIO_H_

#include "CPU.h"
#include "mem.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SocGpio;

typedef void (*GpioChangedNotifF)(void* userData, uint32_t gpio, bool oldState, bool newState);
typedef void (*GpioDirsChangedF)(void* userData);

enum SocGpioState {  // these values make it look like all HiZ, AFR, and nonexistent pins have
                     // pullups to those who dumbly assume socGpioGetState returns a boolean

    SocGpioStateLow,
    SocGpioStateHigh,
    SocGpioStateHiZ,
    SocGpioStateAFR0,  // AFR values must be in order
    SocGpioStateAFR1,
    SocGpioStateAFR2,
    SocGpioStateAFR3,
    SocGpioStateNoSuchGpio,
};

struct SocGpio* socGpioInit(struct ArmMem* physMem, struct SocIc* ic, uint_fast8_t socRev);

// for external use :)
enum SocGpioState socGpioGetState(struct SocGpio* gpio, uint_fast8_t gpioNum);
void socGpioSetState(struct SocGpio* gpio, uint_fast8_t gpioNum,
                     bool on);  // we can only set value (and only of input pins), not direction

// only for output pins!
void socGpioSetNotif(struct SocGpio* gpio, uint_fast8_t gpioNum, GpioChangedNotifF notifF,
                     void* userData);  // one per pin. set ot NULL to disable

// for all (but only one notifier)
void socGpioSetDirsChangedNotif(struct SocGpio* gpio, GpioDirsChangedF notifF, void* userData);

#ifdef __cplusplus
}
#endif

#endif
