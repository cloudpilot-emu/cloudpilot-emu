#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int logPrintf(const char* format, ...);

int logPrintfDom(uint32_t domain, const char* format, ...);

void logEnable();

void logDisable();

void logEnableDomain(uint32_t domain);

void logDisableDomain(uint32_t domain);

#ifdef __cplusplus
}
#endif

#endif  // _LOGGING_H_
