#include "FSTools.h"

#include <cstdlib>

void* FSTools::Malloc(int size) { return malloc(size); }

void FSTools::Free(void* buffer) { free(buffer); }

void* FSTools::Nullptr() { return nullptr; }
