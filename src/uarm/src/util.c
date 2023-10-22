#include "util.h"

#include <time.h>
#include <unistd.h>

uint64_t timestampUsec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}
