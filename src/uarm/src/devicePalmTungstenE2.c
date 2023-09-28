//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "SDL.h"
#include "ac97dev_WM9712L.h"
#include "device.h"
#include "mmiodev_DirectNAND.h"
#include "util.h"

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
};

bool deviceHasGrafArea(void) { return true; }

enum RomChipType deviceGetRomMemType(void) { return RomStrataFlash16x; }

uint32_t deviceGetRamSize(void) { return 16UL << 20; }

uint_fast8_t deviceGetSocRev(void) {
    return 0;  // PXA25x
}

struct Device *deviceSetup(struct SocPeriphs *sp, struct Keypad *kp, struct VSD *vsd,
                           FILE *nandFile) {
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

    dev->wm9712L = wm9712LInit(sp->ac97, sp->gpio, 50);
    if (!dev->wm9712L) ERR("Cannot init WM9712L");

    dev->nand = directNandInit(sp->mem, 0x04000002UL, 0x04000004UL, 0x04000000UL, 0x00fffff9ul,
                               sp->gpio, 79, &nandSpecs, nandFile);
    if (!dev->nand) ERR("Cannot init NAND");

    if (!keypadAddGpioKey(kp, SDLK_F1, 11, false)) ERR("Cannot init hardkey1 (datebook)\n");

    if (!keypadAddGpioKey(kp, SDLK_F2, 13, false)) ERR("Cannot init hardkey2 (address)\n");

    if (!keypadAddGpioKey(kp, SDLK_F3, 7, false)) ERR("Cannot init hardkey3 (ToDo)\n");

    if (!keypadAddGpioKey(kp, SDLK_F4, 5, false)) ERR("Cannot init hardkey4 (Memos)\n");

    if (!keypadAddGpioKey(kp, SDLK_DOWN, 21, false)) ERR("Cannot init down key\n");

    if (!keypadAddGpioKey(kp, SDLK_UP, 22, false)) ERR("Cannot init up key\n");

    if (!keypadAddGpioKey(kp, SDLK_LEFT, 19, false)) ERR("Cannot init left key\n");

    if (!keypadAddGpioKey(kp, SDLK_RIGHT, 20, false)) ERR("Cannot init right key\n");

    if (!keypadAddGpioKey(kp, SDLK_RETURN, 14, false)) ERR("Cannot init select key\n");

    if (!keypadAddGpioKey(kp, SDLK_ESCAPE, 2, false))  // XXX: change this to tru eot boot with
                                                       // poewr key inverted. press ESC to release.
        ERR("Cannot init power key\n");

    socGpioSetState(sp->gpio, 1, true);  // reset button

    socGpioSetState(sp->gpio, 4,
                    true);  // not in cradle (also prevents FIQ wtchdog from firing after 5 sec)

    socGpioSetState(sp->gpio, 40, true);  // no hotsync button pressed [??]

    socGpioSetState(sp->gpio, 12, false);  // no headphones [??]

    socGpioSetState(sp->gpio, 52, true);  // no manufacturing test mode please

    wm9712LsetAuxVoltage(dev->wm9712L, WM9712LauxPinBmon, 4200 / 3);  // main battery is 4.2V

    socGpioSetState(sp->gpio, 10, !vsd);

    sp->dbgUart = sp->uarts[1];  // HWUART

    return dev;
}

void devicePeriodic(struct Device *dev, uint32_t cycles) {
    if (!(cycles & 0x000007FFUL)) wm9712Lperiodic(dev->wm9712L);

    if (!(cycles & 0x000000FFUL)) directNandPeriodic(dev->nand);
}

void deviceTouch(struct Device *dev, int x, int y) {
    wm9712LsetPen(dev->wm9712L, (x >= 0 && y >= 0) ? 280 + 173 * x / 16 : -1,
                  (x >= 0 && y >= 0) ? 210 + 134 * y / 16 : y, 1000);
}

void deviceKey(struct Device *dev, uint32_t key, bool down) {
    // nothing
}
