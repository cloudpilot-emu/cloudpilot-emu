#include "Cloudpilot.h"

#include <cstdlib>

#include "EmROMReader.h"

void* Cloudpilot::Malloc(long size) { return ::malloc(size); }

void Cloudpilot::Free(void* buffer) { ::free(buffer); }

bool Cloudpilot::GetRomInfo(void* buffer, long size, RomInfo& romInfo) {
    EmROMReader reader(buffer, size);

    if (!reader.AcquireCardHeader() || !reader.AcquireROMHeap() || !reader.AcquireDatabases() ||
        !reader.AcquireFeatures()) {
        cerr << "unable to read ROM --- not a valid ROM image?" << endl;

        return false;
    }

    romInfo = {.cardVersion = (int)reader.GetCardVersion(),
               .cardName = reader.GetCardName(),
               .romVersion = (int)reader.GetRomVersion(),
               .romVersionString = reader.GetRomVersionString()};

    return true;
}
