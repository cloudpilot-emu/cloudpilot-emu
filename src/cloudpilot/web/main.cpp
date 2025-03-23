#include <emscripten.h>

#include "Logging.h"

int EMSCRIPTEN_KEEPALIVE main() {
    logDisable();

    return 0;
}
