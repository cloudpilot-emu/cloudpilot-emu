#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Clock;

typedef void (*clockConsumerCallback)(void* userData);

struct Clock* clockInit();
void clockRegisterConsumer(struct Clock* clock, uint64_t periodNsec, clockConsumerCallback cb,
                           void* userData);

void clockAdvance(struct Clock* clock, uint64_t cpuCycles, uint64_t cyclesPerSecond);
uint64_t clockForward(struct Clock* clock, uint64_t cyclesPerSecond);

#ifdef __cplusplus
}
#endif

#endif  // _CLOCK_H_
