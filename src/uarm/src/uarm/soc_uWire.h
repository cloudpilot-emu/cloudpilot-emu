//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr
#ifndef _SOC_UWIRE_H_
#define _SOC_UWIRE_H_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"
#include "soc_DMA.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SocUwire;

// uWire is SPI with a higher-level protocol. we use ssp-like callbacks but this is not quite ssp,
// namely bits in and bits out will differ

// if both bits are negative, sent says nonzero if we were selected, 0 if we were deselected
typedef uint_fast16_t (*UWireClientProcF)(
    void *userData, int_fast8_t bitsToDev, int_fast8_t bitsFromDev,
    uint_fast16_t sent);  // return result byte. 0 if nothing to say (so that replies can be ORRed)

struct SocUwire *socUwireInit(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma);
void socUwirePeriodic(struct SocUwire *uw);
bool socUwireAddClient(struct SocUwire *uw, uint_fast8_t cs, UWireClientProcF procF,
                       void *userData);

#ifdef __cplusplus
}
#endif

#endif
