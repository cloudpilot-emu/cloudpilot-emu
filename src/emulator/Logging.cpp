#include "Logging.h"

#include <cstdarg>
#include <cstdio>

namespace {
    bool loggingEnabled = true;
    uint32 enabledDomains = 0;
}  // namespace

int logging::printf(const char* format, ...) {
    if (!loggingEnabled) return 0;

    va_list args;
    va_start(args, format);

    int res = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    return res;
}

int logging::printf(uint32 domain, const char* format, ...) {
    if (!loggingEnabled || !(enabledDomains & domain)) return 0;

    va_list args;
    va_start(args, format);

    int res = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    return res;
}

void logging::enable() { loggingEnabled = true; }

void logging::disable() { loggingEnabled = false; }

void logging::enableDomain(Domain domain) { enabledDomains |= domain; }

void logging::disableDomain(Domain domain) { enabledDomains &= ~domain; }
