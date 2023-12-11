#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLOCK_MAX_CONSUMERS 16

typedef void (*clockConsumerCallback)(void* userData);

struct Consumer {
    clockConsumerCallback cb;
    void* userData;
    uint64_t periodNsec;
    uint64_t nextTickNsec;

    struct Consumer* next;
};

struct Clock {
    struct Consumer consumers[CLOCK_MAX_CONSUMERS];
    struct Consumer* nextConsumer;

    uint32_t consumerCount;
    uint64_t accTimeNsec;
    uint64_t nextTickNsec;
};

struct Clock* clockInit();
void clockRegisterConsumer(struct Clock* clock, uint64_t periodNsec, clockConsumerCallback cb,
                           void* userData);

uint64_t clockForward(struct Clock* clock, uint64_t cyclesPerSecond);

FORCE_INLINE void clockAdvance(struct Clock* clock, uint64_t cpuCycles, uint64_t cyclesPerSecond) {
    clock->accTimeNsec += ((cpuCycles * 1000000000ULL) / cyclesPerSecond);
    if (clock->accTimeNsec < clock->nextTickNsec) return;

    struct Consumer* consumer = clock->nextConsumer;

    while (consumer->nextTickNsec <= clock->accTimeNsec) {
        consumer->cb(consumer->userData);
        consumer->nextTickNsec += consumer->periodNsec;

        if (!consumer->next || consumer->nextTickNsec <= consumer->next->nextTickNsec) continue;

        struct Consumer** next = &(consumer->next->next);
        while (*next && consumer->nextTickNsec > (*next)->nextTickNsec) next = &((*next)->next);

        clock->nextConsumer = consumer->next;
        consumer->next = *next;
        *next = consumer;

        consumer = clock->nextConsumer;
    }

    clock->nextTickNsec = clock->nextConsumer->nextTickNsec;
}

#ifdef __cplusplus
}
#endif

#endif  // _CLOCK_H_
