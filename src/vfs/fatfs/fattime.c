#include <time.h>

#include "ff.h"

DWORD get_fattime() {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    return ((timeinfo->tm_sec >> 1) & 0x1f) | ((timeinfo->tm_min & 0x3f) << 5) |
           ((timeinfo->tm_hour & 0x1f) << 11) | ((timeinfo->tm_mday & 0x1f) << 16) |
           (((timeinfo->tm_mon + 1) & 0x0f) << 21) | (((timeinfo->tm_year - 80) & 0x7f) << 25);
}
