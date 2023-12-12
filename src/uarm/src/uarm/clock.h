#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*clockConsumerCallback)(void* userData);

struct Clock;

struct Clock* clockInit();
void clockRegisterConsumer(struct Clock* clock, uint64_t periodNsec, clockConsumerCallback cb,
                           void* userData);

uint64_t clockCyclesToNextTick(struct Clock* clock, uint64_t cyclesPerSecond);

void clockAdvance(struct Clock* clock, uint64_t cpuCycles, uint64_t cyclesPerSecond);

#ifdef __cplusplus
}
#endif

#endif  // _CLOCK_H_
