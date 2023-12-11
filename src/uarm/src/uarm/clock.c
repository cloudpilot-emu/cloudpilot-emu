#include "clock.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

struct Clock* clockInit() {
    struct Clock* clock = malloc(sizeof(*clock));
    memset(clock, 0, sizeof(*clock));

    return clock;
}

void clockRegisterConsumer(struct Clock* clock, uint64_t periodNsec, clockConsumerCallback cb,
                           void* userData) {
    if (clock->consumerCount == CLOCK_MAX_CONSUMERS) ERR("max number of consumers reached\n");

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

uint64_t clockForward(struct Clock* clock, uint64_t cyclesPerSecond) {
    const uint64_t cyclesAdvance =
        ((clock->nextConsumer->nextTickNsec - clock->accTimeNsec) * cyclesPerSecond) /
            1000000000ULL +
        1;

    clockAdvance(clock, cyclesAdvance, cyclesPerSecond);

    return cyclesAdvance;
}
