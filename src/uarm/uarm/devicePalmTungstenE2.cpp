//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <stdlib.h>

#include "ac97dev_WM9712L.h"
#include "cputil.h"
#include "device.h"
#include "mmiodev_DirectNAND.h"
#include "savestate/savestateAll.h"

// clang-format off
/*
		GPIO	DIR	VAL	AFR	EDGE	NOTES
		0		IN		0
		1		IN		1			gpio reset
		2		IN		0
		3		IN		0
		4		IN		0	FE 		checked in FIQ handler of all places.... if high, ignored. probbly cradle detect (low if cradled)
		5		IN		0
		6		OUT	LO	1			MMC clock
		7		IN		0
		8		OUT	LO	1			MMC CS0
		9		IN		0
		10		IN		0			sd card detect (Active low)
		11		IN		0
		12		IN		0	RE FE 	headphone detect?		[GUESS]
		13		IN		0
		14		IN		0
		15		OUT	LO	2			nCS1
		16		OUT	LO	2			PWM0
		17		OUT	HI	0
		18		OUT	LO	0
		19		IN		0
		20		IN		0
		21		IN		0
		22		IN		0
		23		OUT	LO	0
		24		OUT	LO	0
		25		IN		0
		26		OUT	HI	0			WM9712L power? enabled when enabling pen. hi for power
		27		OUT	LO	0
		28		IN		1			AC97 bit_clk
		29		IN		1			AC97 Sdata_in0
		30		OUT	LO	2			AC97 Sdata_out
		31		OUT	LO	2			AC97 sync
		32		OUT	HI	0
		33		OUT	LO	0
		34		IN		1			FFUART RX
		35		IN		0
		36		OUT	LO	0
		37		OUT	HI	0
		38		OUT	LO	0
		39		OUT	LO	2			FFUART TX
		40		IN		0	FE 		hotsync button?		[GUESS]
		41		OUT	LO	0
		42		IN		3			HWUART RX
		43		OUT	LO	3			HWUART TX
		44		IN		3			HWUART CTS
		45		OUT	LO	3			HWUART RTS
		46		IN		2			STUART RX
		47		OUT	LO	1			STUART TX
		48		OUT	LO	0
		49		IN		0
		50		IN		0	RE 		probably pen detect
		51		IN		0			card write protect switch (active high)
		52		IN		0			low on boot causes manufacturing test to be entered
		53		IN		0
		54		OUT	LO	0
		55		OUT	LO	0			SD power (active high)
		56		OUT	HI	0
		57		OUT	HI	0
		58		OUT	LO	2			LDD[0]
		59		OUT	LO	2			LDD[1]
		60		OUT	LO	2			LDD[2]
		61		OUT	LO	2			LDD[3]
		62		OUT	LO	2			LDD[4]
		63		OUT	LO	2			LDD[5]
		64		OUT	LO	2			LDD[6]
		65		OUT	LO	2			LDD[7]
		66		OUT	LO	2			LDD[8]
		67		OUT	LO	2			LDD[9]
		68		OUT	LO	2			LDD[10]
		69		OUT	LO	2			LDD[11]
		70		OUT	LO	2			LDD[12]
		71		OUT	LO	2			LDD[13]
		72		OUT	LO	2			LDD[14]
		73		OUT	LO	2			LDD[15]
		74		OUT	LO	2			LCD FCLK
		75		OUT	LO	2			LCD LCLK
		76		OUT	LO	2			LCD PCLK
		77		OUT	LO	2			LCD AC BIAS
		78		OUT	LO	0
		79		IN		0			NAND nRDY
		80		OUT	LO	0
		81		OUT	LO	0
		82		OUT	LO	0
		83		OUT	LO	0
		84		OUT	LO	0
		85		IN		0
*/
// clang-format on

struct Device {
    struct WM9712L *wm9712L;
    struct DirectNAND *nand;
    struct SocGpio *gpio;

    struct Reschedule reschedule;

    enum DeviceType5 type;
};

uint32_t deviceGetDefaultRamSize(void) { return 16UL << 20; }

bool deviceSupportsRamSize(uint32_t size) { return size == (16ul << 20) || size == (32ul << 20); }

enum RamTermination deviceGetRamTerminationStyle(void) { return RamTerminationMirror; }

uint_fast8_t deviceGetSocRev(void) {
    return 0;  // PXA25x
}

static void devicePrvReschedule(void *ctx, uint32_t task) {
    struct Device *dev = (struct Device *)ctx;

    if (task == RESCHEDULE_TASK_NAND)
        dev->reschedule.rescheduleCb(dev->reschedule.ctx, RESCHEDULE_TASK_DEVICE_TIER0);
}

struct Device *deviceSetup(enum DeviceType5 type, struct SocPeriphs *sp,
                           struct Reschedule reschedule, struct Keypad *kp, struct VSD *vsd,
                           uint8_t *nandContent, size_t nandSize,
                           const struct MemoryBuffer *pageBuffer) {
    static const struct NandSpecs nandSpecs = {
        .bytesPerPage = 528,
        .blocksPerDevice = 2048,
        .pagesPerBlockLg2 = 5,
        .flags = NAND_FLAG_SAMSUNG_ADDRESSED_VIA_AREAS,
        .devIdLen = 2,
        .devId = {0xec, 0x75},
    };

    struct Device *dev;

    dev = (struct Device *)malloc(sizeof(*dev));
    if (!dev) ERR("cannot alloc device");

    dev->type = type;

    dev->gpio = sp->gpio;

    struct Reschedule deviceReschedule = {.rescheduleCb = devicePrvReschedule, .ctx = dev};

    dev->reschedule = reschedule;

    dev->wm9712L = wm9712LInit(sp->ac97, sp->gpio, 50);
    if (!dev->wm9712L) ERR("Cannot init WM9712L");

    dev->nand =
        directNandInit(sp->mem, deviceReschedule, 0x04000002UL, 0x04000004UL, 0x04000000UL,
                       0x00fffff9ul, sp->gpio, 79, &nandSpecs, nandContent, nandSize, pageBuffer);
    if (!dev->nand) ERR("Cannot init NAND");

    if (!keypadAddGpioKey(kp, keyIdHard1, 11, false)) ERR("Cannot init hardkey1 (datebook)\n");

    if (!keypadAddGpioKey(kp, keyIdHard2, 13, false)) ERR("Cannot init hardkey2 (address)\n");

    if (!keypadAddGpioKey(kp, keyIdHard3, 7, false)) ERR("Cannot init hardkey3 (ToDo)\n");

    if (!keypadAddGpioKey(kp, keyIdHard4, 5, false)) ERR("Cannot init hardkey4 (Memos)\n");

    if (!keypadAddGpioKey(kp, keyIdDown, 21, false)) ERR("Cannot init down key\n");

    if (!keypadAddGpioKey(kp, keyIdUp, 22, false)) ERR("Cannot init up key\n");

    if (!keypadAddGpioKey(kp, keyIdLeft, 19, false)) ERR("Cannot init left key\n");

    if (!keypadAddGpioKey(kp, keyIdRight, 20, false)) ERR("Cannot init right key\n");

    if (!keypadAddGpioKey(kp, keyIdSelect, 14, false)) ERR("Cannot init select key\n");

    if (!keypadAddGpioKey(kp, keyIdPower, 2, false))  // XXX: change this to tru eot boot with
                                                      // poewr key inverted. press ESC to release.
        ERR("Cannot init power key\n");

    sp->gpio = sp->gpio;

    socGpioSetState(sp->gpio, 1, true);  // reset button

    socGpioSetState(sp->gpio, 4,
                    true);  // not in cradle (also prevents FIQ wtchdog from firing after 5 sec)

    socGpioSetState(sp->gpio, 40, true);  // no hotsync button pressed [??]

    socGpioSetState(sp->gpio, 12, false);  // no headphones [??]

    socGpioSetState(sp->gpio, 52, true);  // no manufacturing test mode please

    wm9712LsetAuxVoltage(dev->wm9712L, WM9712LauxPinBmon, 4200 / 3);  // main battery is 4.2V

    deviceSetSdCardInserted(dev, false);

    sp->dbgUart = sp->uarts[1];  // HWUART

    sp->nand = directNandGetNand(dev->nand);

    struct DeviceDisplayConfiguration displayConfiguration;
    deviceGetDisplayConfiguration(type, &displayConfiguration);

    return dev;
}

void devicePeriodic(struct Device *dev, uint32_t tier) {
    if (tier == DEVICE_PERIODIC_TIER0) directNandPeriodic(dev->nand);
}

void devicePcmPeriodic(struct Device *dev) { wm9712Lperiodic(dev->wm9712L); }

void deviceTouch(struct Device *dev, int x, int y) {
    wm9712LsetPen(dev->wm9712L, (x >= 0 && y >= 0) ? 280 + 10 * x : -1,
                  (x >= 0 && y >= 0) ? 210 + 7 * y : -1, 1000);
}

void deviceKey(struct Device *dev, uint32_t key, bool down) {
    // nothing
}

void deviceGetDisplayConfiguration(enum DeviceType5 deviceType,
                                   struct DeviceDisplayConfiguration *displayConfiguration) {
    displayConfiguration->width = 320;

    if (deviceType == deviceTypeFrankenE2) {
        displayConfiguration->height = 480;
        displayConfiguration->graffitiHeight = 0;
    } else {
        displayConfiguration->height = 320;
        displayConfiguration->graffitiHeight = 120;
    }
}

bool deviceTaskRequired(struct Device *dev, uint32_t tier) {
    if (tier == DEVICE_PERIODIC_TIER0) {
        return directNandTaskRequired(dev->nand);
    } else {
        return true;
    }
}

void deviceSetAudioQueue(struct Device *dev, struct AudioQueue *audioQueue) {
    wm9712LsetAudioQueue(dev->wm9712L, audioQueue);
}

bool deviceI2sConnected() { return false; }

void deviceSetSdCardInserted(struct Device *dev, bool inserted) {
    socGpioSetState(dev->gpio, 10, !inserted);
}

enum DeviceType5 deviceGetType(struct Device *dev) { return dev->type; }

template <typename T>
void deviceSave(struct Device *dev, T &savestate) {
    wm9712Lsave(dev->wm9712L, savestate);
    directNandSave(dev->nand, savestate);
}

template <typename T>
void deviceLoad(struct Device *dev, T &loader) {
    wm9712Load(dev->wm9712L, loader);
    directNandLoad(dev->nand, loader);
}

template void deviceSave<Savestate<ChunkType>>(Device *dev, Savestate<ChunkType> &savestate);
template void deviceSave<SavestateProbe<ChunkType>>(Device *dev,
                                                    SavestateProbe<ChunkType> &savestate);
template void deviceLoad<SavestateLoader<ChunkType>>(Device *dev,
                                                     SavestateLoader<ChunkType> &loader);