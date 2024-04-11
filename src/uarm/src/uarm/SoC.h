//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _SOC_H_
#define _SOC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "SoC_type.h"
#include "keys.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHAR_CTL_C -1L
#define CHAR_NONE -2L

struct SoC;
struct AudioQueue;

typedef bool (*SdSectorR)(uint32_t secNum, void *buf);
typedef bool (*SdSectorW)(uint32_t secNum, const void *buf);

struct SoC *socInit(void *romData, const uint32_t romSize, uint32_t sdNumSectors, SdSectorR sdR,
                    SdSectorW sdW, uint8_t *nandContent, size_t nandSize, int gdbPort,
                    uint_fast8_t socRev);
uint64_t socRun(struct SoC *soc, uint64_t maxCycles, uint64_t cyclesPerSecond);

void socBootload(struct SoC *soc, uint32_t method, void *param);  // soc-specific

uint32_t *socGetPendingFrame(struct SoC *soc);
void socResetPendingFrame(struct SoC *soc);

void socKeyDown(struct SoC *soc, enum KeyId key);
void socKeyUp(struct SoC *soc, enum KeyId key);
void socPenDown(struct SoC *soc, int x, int y);
void socPenUp(struct SoC *soc);

void socSleep(struct SoC *soc);
void socWakeup(struct SoC *soc, uint8_t wakeupSource);

// externally needed
void socExtSerialWriteChar(int ch);
int socExtSerialReadChar(void);

void socSetFramebufferDirty(struct SoC *soc);
bool socSetFramebuffer(struct SoC *soc, uint32_t start, uint32_t size);

void socSetAudioQueue(struct SoC *soc, struct AudioQueue *audioQueue);
void socSetPcmSuspended(struct SoC *soc, bool pcmSuspended);

#ifdef __cplusplus
}
#endif

#endif
