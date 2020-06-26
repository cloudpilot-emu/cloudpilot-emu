#include "Cloudpilot.h"

#include <cstdlib>

void* Cloudpilot::malloc(long size) { return ::malloc(size); }

void Cloudpilot::free(void* buffer) { ::free(buffer); }
