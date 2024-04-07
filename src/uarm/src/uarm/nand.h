//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _NAND_H_
#define _NAND_H_

#include <stdio.h>

#include "CPU.h"
#include "mem.h"
#include "reschedule.h"
#include "soc_GPIO.h"

#ifdef __cplusplus
extern "C" {
#endif

struct NAND;

typedef void (*NandReadyCbk)(void *userData, bool ready);

// options
#define NAND_FLAG_SAMSUNG_ADDRESSED_VIA_AREAS \
    0x01  // use 0x01 and 0x50 commands to save one bit on byte addressing
#define NAND_HAS_SECOND_READ_CMD 0x02

struct NandSpecs {
    uint32_t bytesPerPage;
    uint32_t blocksPerDevice;
    uint8_t pagesPerBlockLg2;
    uint8_t flags;
    uint8_t devIdLen;
    uint8_t devId[];
};

struct NAND *nandInit(uint8_t *nandContent, struct Reschedule reschedule, size_t nandSize,
                      const struct NandSpecs *specs, NandReadyCbk readyCbk, void *readyCbkData);

void nandSecondReadyCbkSet(struct NAND *nand, NandReadyCbk readyCbk, void *readyCbkData);

bool nandWrite(struct NAND *nand, bool cle, bool ale, uint8_t val);
bool nandRead(struct NAND *nand, bool cle, bool ale, uint8_t *valP);

bool nandIsReady(struct NAND *nand);

void nandPeriodic(struct NAND *nand);

bool nandTaskRequired(struct NAND *nand);

#ifdef __cplusplus
}
#endif

#endif
