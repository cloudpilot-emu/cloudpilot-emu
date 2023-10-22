//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA270_KPC_H_
#define _PXA270_KPC_H_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PxaKpc;

struct PxaKpc *pxaKpcInit(struct ArmMem *physMem, struct SocIc *ic);

// keep in mind that colums are out and rows are in
void pxaKpcMatrixKeyChange(struct PxaKpc *kpc, uint_fast8_t row, uint_fast8_t col, bool isDown);
void pxaKpcDirectKeyChange(struct PxaKpc *kpc, uint_fast8_t keyIdx, bool isDown);
void pxaKpcJogInput(struct PxaKpc *kpc, uint_fast8_t jogIdx, bool up);  // else down

#ifdef __cplusplus
}
#endif

#endif
