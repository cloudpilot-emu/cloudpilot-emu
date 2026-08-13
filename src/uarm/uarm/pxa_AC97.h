#ifndef _PXA_AC97_H_
#define _PXA_AC97_H_

#include <cstdint>

#include "CPU.h"
#include "mem.h"
#include "pxa_DMA.h"
#include "pxa_IC.h"

struct PxaAC97;

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

struct PxaAC97 *pxaAC97Init(struct ArmMem *physMem, struct PxaIc *ic, struct PxaDma *dma);
void pxaAC97Periodic(struct PxaAC97 *ac97);

// client api
void pxaAC97clientAdd(struct PxaAC97 *ac97, enum Ac97Codec which, Ac97CodecRegR regR,
                      Ac97CodecRegW regW, void *userData);
bool pxaAC97clientClientWantData(struct PxaAC97 *ac97, enum Ac97Codec which, uint32_t *dataPtr);
void pxaAC97clientClientHaveData(struct PxaAC97 *ac97, enum Ac97Codec which, uint32_t data);

template <typename T>
void pxaAC97Save(struct PxaAC97 *ac97, T &savestate);

template <typename T>
void pxaAC97Load(struct PxaAC97 *ac97, T &loader);

#endif  // _PXA_AC97_H_
