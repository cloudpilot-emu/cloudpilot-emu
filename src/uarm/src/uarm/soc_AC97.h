//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_AC97_H_
#define _SOC_AC97_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "CPU.h"
#include "mem.h"
#include "soc_DMA.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SocAC97;

enum Ac97Codec {
    Ac97PrimaryAudio,
    Ac97SecondaryAudio,
    Ac97PrimaryModem,
    Ac97SecondaryModem,
};

typedef bool (*Ac97CodecRegR)(void *userData, uint32_t regAddr, uint16_t *regValP);
typedef bool (*Ac97CodecRegW)(void *userData, uint32_t regAddr, uint16_t val);

typedef bool (*Ac97CodecFifoR)(void *userData, uint32_t *regValP);
typedef bool (*Ac97CodecFifoW)(void *userData, uint32_t val);

struct SocAC97 *socAC97Init(struct ArmMem *physMem, struct SocIc *ic, struct SocDma *dma);
void socAC97Periodic(struct SocAC97 *ac97);

// client api
void socAC97clientAdd(struct SocAC97 *ac97, enum Ac97Codec which, Ac97CodecRegR regR,
                      Ac97CodecRegW regW, void *userData);
bool socAC97clientClientWantData(struct SocAC97 *ac97, enum Ac97Codec which, uint32_t *dataPtr);
void socAC97clientClientHaveData(struct SocAC97 *ac97, enum Ac97Codec which, uint32_t data);

#ifdef __cplusplus
}
#endif

#endif
