//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_MEM_CTRL_H_
#define _PXA_MEM_CTRL_H_

#include <stdint.h>

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PxaMemCtrlr;

struct PxaMemCtrlr* pxaMemCtrlrInit(struct ArmMem* physMem, uint_fast8_t socRev);

#ifdef __cplusplus
}

template <typename T>
void pxaMemCtrlrSave(struct PxaMemCtrlr* pxaMemCtrlr, T& savestate);

template <typename T>
void pxaMemCtrlrLoad(struct PxaMemCtrlr* pxaMemCtrlr, T& loader);

#endif

#endif
