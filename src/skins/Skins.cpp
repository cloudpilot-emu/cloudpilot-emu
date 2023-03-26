#include "Skins.h"

#include "Defer.h"
#include "zip.h"

namespace {
#include "generated/skins.h"
}

uint8_t* skins::GetSkin(const char* name, size_t& len) {
    zip_t* zip = zip_stream_open(reinterpret_cast<const char*>(skins_zip), skins_zip_len, 0, 'r');
    if (!zip) return nullptr;

    Defer closeZip([&]() { zip_close(zip); });

    if (zip_entry_open(zip, name) != 0) return nullptr;

    uint8_t* resultBuffer;
    ssize_t decompressResult = zip_entry_read(zip, reinterpret_cast<void**>(&resultBuffer), &len);

    zip_entry_close(zip);

    return decompressResult < 0 ? nullptr : resultBuffer;
}
