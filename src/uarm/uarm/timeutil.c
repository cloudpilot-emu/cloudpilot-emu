#include "timeutil.h"

#include <stddef.h>
#include <time.h>

#define REFRESH_TZ_INTERVAL_SECONDS 10
#define PALM_EPOCH_OFFSET 2082844800ull

uint64_t palmEpochSeconds() {
    static time_t lastUpdate = 0;
    static long int tzoffset = 0;

    time_t now = time(NULL);

    if (now - lastUpdate > REFRESH_TZ_INTERVAL_SECONDS) {
        struct tm loc;
        localtime_r(&now, &loc);

        // This is not super portable, but it will do for now.
        tzoffset = loc.tm_gmtoff;
        lastUpdate = now;
    }

    return (uint64_t)now + PALM_EPOCH_OFFSET + tzoffset;
}
