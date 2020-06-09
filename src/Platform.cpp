#include "Platform.h"

#include <chrono>
#include <cstring>
#include <ctime>

long Platform::getMilliseconds() {
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch())
        .count();
}

void Platform::getTime(uint32& hour, uint32& month, uint32& day) {
    time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());

    tm t;
    localtime_r(&time, &t);

    hour = t.tm_hour;
    month = t.tm_mon;
    day = t.tm_mday;
}

void* Platform::AllocateMemory(size_t count) {
    void* mem = malloc(count);

    return mem;
}

void* Platform::AllocateMemoryClear(size_t count) {
    void* mem = Platform::AllocateMemory(count);
    memset(mem, 0, count);

    return mem;
}
