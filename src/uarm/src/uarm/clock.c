#include "clock.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define MAX_CONSUMERS 16

struct Consumer {
    clockConsumerCallback cb;
    void* userData;
    uint64_t periodNsec;
    uint64_t nextTickNsec;

    struct Consumer* next;
};

struct Clock {
    struct Consumer consumers[MAX_CONSUMERS];
    struct Consumer* nextConsumer;

    uint32_t consumerCount;
    uint64_t accTimeNsec;
    uint64_t nextTickNsec;
};

struct Clock* clockInit() {
    struct Clock* clock = malloc(sizeof(*clock));
    memset(clock, 0, sizeof(*clock));

    return clock;
}

void clockRegisterConsumer(struct Clock* clock, uint64_t periodNsec, clockConsumerCallback cb,
                           void* userData) {
    if (clock->consumerCount == MAX_CONSUMERS) ERR("max number of consumers reached\n");

    struct Consumer* consumer = clock->consumers + clock->consumerCount++;

    consumer->cb = cb;
    consumer->userData = userData;
    consumer->periodNsec = consumer->nextTickNsec = periodNsec;

    struct Consumer** next = &clock->nextConsumer;
    while (*next && consumer->nextTickNsec > (*next)->nextTickNsec) next = &((*next)->next);

    consumer->next = *next;
    *next = consumer;

    clock->nextTickNsec = clock->nextConsumer->nextTickNsec;
}

void clockAdvance(struct Clock* clock, uint64_t cpuCycles, uint64_t cyclesPerSecond) {
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

uint64_t clockForward(struct Clock* clock, uint64_t cyclesPerSecond) {
    const uint64_t cyclesAdvance =
        ((clock->nextConsumer->nextTickNsec - clock->accTimeNsec) * cyclesPerSecond) /
            1000000000ULL +
        1;

    clockAdvance(clock, cyclesAdvance, cyclesPerSecond);

    return cyclesAdvance;
}
