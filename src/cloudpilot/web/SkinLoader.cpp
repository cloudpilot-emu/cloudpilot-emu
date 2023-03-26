#include "SkinLoader.h"

#include <cstdlib>

#include "Skins.h"

SkinLoader::SkinLoader(const char* name) {
    size_t len;
    data = skins::GetSkin(name, len);

    size = len;
}

SkinLoader::~SkinLoader() {
    if (data) free(data);
}

void* SkinLoader::GetData() const { return data; }

unsigned int SkinLoader::GetSize() const { return size; }
