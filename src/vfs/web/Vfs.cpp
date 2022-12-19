#include "Vfs.h"

#include <cstdlib>

void* Vfs::Malloc(long size) { return malloc(size); }

void Vfs::Free(void* buffer) { free(buffer); }

void* Vfs::Nullptr() { return nullptr; }
