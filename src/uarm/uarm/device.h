//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <cstdint>
#include <cstdio>

#include "SoC.h"
#include "device_type5.h"
#include "get_emu_time.h"
#include "keys.h"
#include "mem.h"
#include "nand.h"
#include "pxa_AC97.h"
#include "pxa_GPIO.h"
#include "pxa_I2C.h"
#include "pxa_I2S.h"
#include "pxa_SSP.h"
#include "pxa_UART.h"
#include "reschedule.h"
#include "vSD.h"

#define DEVICE_PERIODIC_TIER0 0
#define DEVICE_PERIODIC_TIER1 1

struct AudioQueue;

struct SocPeriphs {
    // in to deviceSetup
    struct PxaAC97 *ac97;
    struct PxaGpio *gpio;
    struct PxaI2c *i2c;
    struct PxaI2s *i2s;
    struct PxaSsp *ssp;
    struct PxaSsp *ssp2;  // assp for xscale
    struct PxaSsp *ssp3;  // nssp for scale
    struct ArmMem *mem;
    class SoC *soc;

    // PXA order: ffUart, hwUart, stUart, btUart
    struct PxaUart *uarts[4];

    void *adc;  // some cases need this
    void *kpc;  // some cases need this

    // out from deviceSetup
    struct NAND *nand;
    struct PxaUart *dbgUart;
    struct PxaUart *btUart;
};

enum RamTermination {  // what's after ram in phys map? (some devices probe)
    RamTerminationMirror,
    RamTerminationWriteIgnore,
    RamTerminationNone,
};

struct Device;
struct MemoryBuffer;

// simple queries
enum RamTermination deviceGetRamTerminationStyle(void);
uint_fast8_t deviceGetSocRev(void);

// device handling
struct Device *deviceSetup(enum DeviceType5 type, struct SocPeriphs *sp,
                           struct Reschedule reschedule, struct GetEmuTime getEmuTime,
                           struct Keypad *kp, struct VSD *vsd, uint8_t *nandContent,
                           size_t nandSize, const struct MemoryBuffer *nandPageBuffer);
void deviceKey(struct Device *dev, uint32_t key, bool down);
void devicePeriodic(struct Device *dev, uint32_t tier);
void devicePcmPeriodic(struct Device *dev);
void deviceTouch(struct Device *dev, int x, int y);

bool deviceTaskRequired(struct Device *dev, uint32_t tier);

void deviceSetAudioQueue(struct Device *dev, struct AudioQueue *audioQueue);

bool deviceI2sConnected();

void deviceSetSdCardInserted(struct Device *dev, bool inserted);

enum DeviceType5 deviceGetType(struct Device *dev);

template <typename T>
void deviceSave(struct Device *dev, T &savestate);

template <typename T>
void deviceLoad(struct Device *dev, T &loader);

#endif
