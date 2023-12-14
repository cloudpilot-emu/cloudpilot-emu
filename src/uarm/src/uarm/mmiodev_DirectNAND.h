//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _DIRECT_NAND_H_
#define _DIRECT_NAND_H_

#include <stdio.h>

#include "CPU.h"
#include "mem.h"
#include "nand.h"
#include "soc_GPIO.h"

#ifdef __cplusplus
extern "C" {
#endif

struct DirectNAND;

struct DirectNAND *directNandInit(struct ArmMem *physMem, uint32_t baseCleAddr,
                                  uint32_t baseAleAddr, uint32_t baseDataAddr,
                                  uint32_t maskBitsAddr, struct SocGpio *gpio, int rdyPin,
                                  const struct NandSpecs *specs, uint8_t *nandContent,
                                  size_t nandSize);

void directNandPeriodic(struct DirectNAND *nand);

#ifdef __cplusplus
}
#endif

#endif
