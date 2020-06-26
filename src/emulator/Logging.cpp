#include "Logging.h"

#include <cstdarg>
#include <cstdio>

namespace {
    bool loggingEnabled = true;
}

int log::printf(const char* format, ...) {
    if (!loggingEnabled) return 0;

    va_list args;
    va_start(args, format);

    int res = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    return res;
}

void log::enable() { loggingEnabled = true; }

void log::disable() { loggingEnabled = false; }
