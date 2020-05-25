#include "Logging.h"

#include <cstdarg>
#include <cstdio>

int log::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int res = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    return res;
}
