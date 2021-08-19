#include <emscripten.h>

#include "Logging.h"

int EMSCRIPTEN_KEEPALIVE main() {
    logging::disable();

    return 0;
}
