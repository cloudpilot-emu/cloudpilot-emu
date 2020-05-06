#include "logging.h"

#include <cstdarg>
#include <cstdio>

int log::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    return vfprintf(stderr, format, args);
}
