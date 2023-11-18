#include "util.h"

#include <stdlib.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
    #include "emscripten.h"

EM_JS(void, __emscripten_abort, (), { throw new Error("emulator terminated"); })
#endif

uint64_t timestampUsec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

void uarmAbort() {
#ifdef __EMSCRIPTEN__
    __emscripten_abort();
#else
    abort();
#endif
}
