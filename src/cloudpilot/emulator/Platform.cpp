#include "Platform.h"

#include <chrono>
#include <cstring>
#include <ctime>

namespace {
    chrono::milliseconds::rep getMillisecondsBare() {
        return chrono::duration_cast<chrono::milliseconds>(
                   chrono::system_clock::now().time_since_epoch())
            .count();
    }

    struct TimestampReference {
        TimestampReference() { timestamp = getMillisecondsBare(); }

        chrono::milliseconds::rep timestamp;
    };

    TimestampReference timestampReferece;
}  // namespace

long Platform::GetMilliseconds() { return getMillisecondsBare() - timestampReferece.timestamp; }

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
