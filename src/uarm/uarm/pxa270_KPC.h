//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA270_KPC_H_
#define _PXA270_KPC_H_

#include <cstdint>

#include "mem.h"
#include "pxa_IC.h"

struct PxaKpc;

struct PxaKpc *pxaKpcInit(struct ArmMem *physMem, struct PxaIc *ic);

// keep in mind that colums are out and rows are in
void pxaKpcMatrixKeyChange(struct PxaKpc *kpc, uint_fast8_t row, uint_fast8_t col, bool isDown);
void pxaKpcDirectKeyChange(struct PxaKpc *kpc, uint_fast8_t keyIdx, bool isDown);
void pxaKpcJogInput(struct PxaKpc *kpc, uint_fast8_t jogIdx, bool up);  // else down

#endif
