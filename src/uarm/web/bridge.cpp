#include "bridge.h"

#include <cstdlib>

void* Bridge::Malloc(unsigned int size) { return malloc(size); }

void Bridge::Free(void* ptr) { free(ptr); }
