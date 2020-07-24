#include "Cloudpilot.h"

#include <cstdlib>

#include "EmROMReader.h"
#include "EmSession.h"

namespace {
    unique_ptr<EmROMReader> createReader(void* buffer, long size) {
        auto reader = make_unique<EmROMReader>(buffer, size);

        if (!reader->AcquireCardHeader() || !reader->AcquireROMHeap() ||
            !reader->AcquireDatabases() || !reader->AcquireFeatures()) {
            cerr << "unable to read ROM --- not a valid ROM image?" << endl;

            return nullptr;
        }

        return reader;
    }
}  // namespace

void* Cloudpilot::Malloc(long size) { return ::malloc(size); }

void Cloudpilot::Free(void* buffer) { ::free(buffer); }

bool Cloudpilot::GetRomInfo(void* buffer, long size, RomInfo& romInfo) {
    auto reader = createReader(buffer, size);

    if (!reader) {
        cerr << "unable to read ROM --- not a valid ROM image?" << endl << flush;

        return false;
    }

    romInfo = {.cardVersion = (int)reader->GetCardVersion(),
               .cardName = reader->GetCardName(),
               .romVersion = (int)reader->GetRomVersion(),
               .romVersionString = reader->GetRomVersionString()};

    return true;
}

bool Cloudpilot::InitializeSession(void* buffer, long size, const char* deviceType) {
    if (device) {
        cerr << "session already initialized" << endl << flush;

        return false;
    }

    auto reader = createReader(buffer, size);

    if (!reader) {
        cerr << "unable to read ROM --- not a valid ROM image?" << endl << flush;

        return false;
    }

    device = make_unique<EmDevice>(deviceType);

    if (!device->Supported()) {
        cerr << "unsupported device type " << deviceType << endl << flush;

        device.release();

        return false;
    }

    if (!device->SupportsROM(*reader)) {
        cerr << "ROM not supported for device " << deviceType << endl << flush;

        device.release();

        return false;
    }

    if (!gSession->Initialize(device.get(), (uint8*)buffer, size)) {
        cerr << "Session failed to initialize" << endl << flush;

        device.release();

        return false;
    }

    return true;
}
