//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "ROM.h"
#include "SoC.h"
#include "keys.h"
#include "mem.h"
#include "nand.h"
#include "reschedule.h"
#include "soc_AC97.h"
#include "soc_GPIO.h"
#include "soc_I2C.h"
#include "soc_I2S.h"
#include "soc_SSP.h"
#include "soc_UART.h"
#include "soc_uWire.h"
#include "vSD.h"

#define DEVICE_PERIODIC_TIER0 0
#define DEVICE_PERIODIC_TIER1 1

#ifdef __cplusplus
extern "C" {
#endif

struct AudioQueue;

struct DeviceDisplayConfiguration {
    uint16_t width;
    uint16_t height;
    uint16_t graffitiHeight;
};

struct SocPeriphs {
    // in to deviceSetup
    struct SocAC97 *ac97;
    struct SocGpio *gpio;
    struct SocUwire *uw;
    struct SocI2c *i2c;
    struct SocI2s *i2s;
    struct SocSsp *ssp;
    struct SocSsp *ssp2;  // assp for xscale
    struct SocSsp *ssp3;  // nssp for scale
    struct ArmMem *mem;
    struct SoC *soc;

    // PXA order: ffUart, hwUart, stUart, btUart
    struct SocUart *uarts[4];

    void *adc;  // some cases need this
    void *kpc;  // some cases need this

    // out from deviceSetup
    struct NAND *nand;
    struct SocUart *dbgUart;
};

enum RamTermination {  // what's after ram in phys map? (some devices probe)
    RamTerminationMirror,
    RamTerminationWriteIgnore,
    RamTerminationNone,
};

struct Device;

// simple queries
uint32_t deviceGetRamSize(void);
enum RamTermination deviceGetRamTerminationStyle(void);
uint_fast8_t deviceGetSocRev(void);

// device handling
struct Device *deviceSetup(struct SocPeriphs *sp, struct Reschedule reschedule, struct Keypad *kp,
                           struct VSD *vsd, uint8_t *nandContent, size_t nandSize);
void deviceKey(struct Device *dev, uint32_t key, bool down);
void devicePeriodic(struct Device *dev, uint32_t tier);
void devicePcmPeriodic(struct Device *dev);
void deviceTouch(struct Device *dev, int x, int y);

void deviceGetDisplayConfiguration(struct DeviceDisplayConfiguration *displayConfiguration);

bool deviceTaskRequired(struct Device *dev, uint32_t tier);

void deviceSetAudioQueue(struct Device *dev, struct AudioQueue *audioQueue);

bool deviceI2sConnected();

#ifdef __cplusplus
}
#endif

#endif
