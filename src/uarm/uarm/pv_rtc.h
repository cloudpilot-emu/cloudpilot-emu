#ifndef _PV_RTC_H_
#define _PV_RTC_H_

struct PvRtc;
struct ArmMem;
struct PvIc;

PvRtc* pvRtcInit(ArmMem* mem, PvIc* ic);

void pvRtcTick(PvRtc* rtc);

#endif  // _PV_RTC_H_
