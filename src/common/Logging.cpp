#include "Logging.h"

#include <cstdarg>
#include <cstdio>

namespace {
    bool loggingEnabled = true;
    uint32_t enabledDomains = 0;
}  // namespace

int logPrintf(const char* format, ...) {
    if (!loggingEnabled) return 0;

    va_list args;
    va_start(args, format);

    int res = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    return res;
}

int logPrintfDom(uint32_t domain, const char* format, ...) {
    if (!loggingEnabled || !(enabledDomains & domain)) return 0;

    va_list args;
    va_start(args, format);

    int res = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    return res;
}

void logEnable() { loggingEnabled = true; }

void logDisable() { loggingEnabled = false; }

void logEnableDomain(uint32_t domain) { enabledDomains |= domain; }

void logDisableDomain(uint32_t domain) { enabledDomains &= ~domain; }
