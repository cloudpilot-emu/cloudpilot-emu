#include "Platform.h"

#include <chrono>
#include <cstring>
#include <ctime>

long Platform::GetMilliseconds() {
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch())
        .count();
}

void Platform::GetTime(uint32& hour, uint32& min, uint32& sec) {
    time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());

    tm t;
    localtime_r(&time, &t);

    hour = t.tm_hour;
    min = t.tm_min;
    sec = t.tm_sec;
}

void Platform::GetDate(uint32& year, uint32& month, uint32& day) {
    time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());

    tm t;
    localtime_r(&time, &t);

    year = t.tm_year + 1900;
    month = t.tm_mon + 1;
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

uint32 Platform::Random() { return rand(); }
