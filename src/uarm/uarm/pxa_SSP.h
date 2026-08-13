//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_SSP_H_
#define _PXA_SSP_H_

#include <cstdint>

#include "CPU.h"
#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"
#include "reschedule.h"

// PXA25x/PXA26x/PXA27x
#define PXA_SSP1_BASE 0x41000000UL

// PXA25x/PXA26x
#define PXA_NSSP_BASE 0x41400000UL

// PXA26x
#define PXA_ASSP_BASE 0x41500000UL

// PXA27x
#define PXA_SSP2_BASE 0x41700000UL
#define PXA_SSP3_BASE 0x41900000UL

struct PxaSsp;

typedef uint_fast16_t (*SspClientProcF)(
    void* userData, uint_fast8_t nBits,
    uint_fast16_t sent);  // return result byte. 0 if nothing to say (so that replies can be ORRed)

struct PxaSsp* pxaSspInit(struct ArmMem* physMem, struct Reschedule reschedule, struct PxaIc* ic,
                          struct PxaDma* dma, uint32_t base, uint_fast8_t irqNo,
                          uint_fast8_t dmaReqNoBase);
void pxaSspPeriodic(struct PxaSsp* ssp);
bool pxaSspAddClient(struct PxaSsp* ssp, SspClientProcF procF, void* userData);

bool pxaSspTaskRequired(struct PxaSsp* ssp);

template <typename T>
void pxaSspSave(struct PxaSsp* ssp, T& savestate, uint32_t index = 0);

template <typename T>
void pxaSspLoad(struct PxaSsp* ssp, T& loader, uint32_t index = 0);

#endif
