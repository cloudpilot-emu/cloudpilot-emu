//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_LCD_H_
#define _PXA_LCD_H_

#include "CPU.h"
#include "mem.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct MemoryBuffer;
struct PxaLcd;
struct SoC;

struct PxaLcd *pxaLcdInit(struct ArmMem *physMem, struct SoC *soc, struct SocIc *ic,
                          struct MemoryBuffer *buffer, uint16_t width, uint16_t heigh);

void pxaLcdResetPaletteBuffer(struct PxaLcd *lcd);

void pxaLcdTick(struct PxaLcd *lcd);

uint32_t *pxaLcdGetPendingFrame(struct PxaLcd *lcd);
void pxaLcdResetPendingFrame(struct PxaLcd *lcd);

void pxaLcdSetFramebufferDirty(struct PxaLcd *lcd);

bool pxaLcdIsEnabled(struct PxaLcd *lcd);

#ifdef __cplusplus
}

template <typename T>
void pxaLcdSave(struct PxaLcd *lcd, T &savestate);

template <typename T>
void pxaLcdLoad(struct PxaLcd *lcd, T &loader);
#endif

#endif
