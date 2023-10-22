//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _PXA_RTC_H_
#define _PXA_RTC_H_

#include "CPU.h"
#include "mem.h"
#include "soc_IC.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PxaRtc;

struct PxaRtc* pxaRtcInit(struct ArmMem* physMem, struct SocIc* ic);
void pxaRtcUpdate(struct PxaRtc* rtc);

#ifdef __cplusplus
}
#endif

#endif
