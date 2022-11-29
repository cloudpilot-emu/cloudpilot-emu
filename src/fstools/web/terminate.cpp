#include "../terminate.h"

#include <emscripten.h>

void terminate() { EM_ASM(throw new Error("terminated");); }
