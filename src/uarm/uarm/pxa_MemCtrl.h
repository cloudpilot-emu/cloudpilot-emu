//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_MEM_CTRL_H_
#define _PXA_MEM_CTRL_H_

#include <cstdint>

#include "mem.h"

struct PxaMemCtrlr;

struct PxaMemCtrlr* pxaMemCtrlrInit(struct ArmMem* physMem, uint_fast8_t socRev);

template <typename T>
void pxaMemCtrlrSave(struct PxaMemCtrlr* pxaMemCtrlr, T& savestate);

template <typename T>
void pxaMemCtrlrLoad(struct PxaMemCtrlr* pxaMemCtrlr, T& loader);

#endif
