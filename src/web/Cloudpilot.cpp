#include "Cloudpilot.h"

#include <cstdlib>

#include "EmDevice.h"
#include "EmHAL.h"
#include "EmROMReader.h"
#include "EmSession.h"
#include "EmSystemState.h"

namespace {
    unique_ptr<EmROMReader> createReader(void* buffer, long size) {
        auto reader = make_unique<EmROMReader>(buffer, size);

        if (!reader->Read()) {
            cerr << "unable to read ROM --- not a valid ROM image?" << endl;

            return nullptr;
        }

        return reader;
    }
}  // namespace

void* Cloudpilot::Malloc(long size) { return ::malloc(size); }

void Cloudpilot::Free(void* buffer) { ::free(buffer); }

bool Cloudpilot::InitializeSession(void* buffer, long size, const char* deviceType) {
    auto reader = createReader(buffer, size);

    if (!reader) {
        cerr << "unable to read ROM --- not a valid ROM image?" << endl << flush;

        return false;
    }

    unique_ptr<EmDevice> device = make_unique<EmDevice>(deviceType);

    if (!device->Supported()) {
        cerr << "unsupported device type " << deviceType << endl << flush;

        return false;
    }

    if (!device->SupportsROM(*reader)) {
        cerr << "ROM not supported for device " << deviceType << endl << flush;

        return false;
    }

    if (!gSession->Initialize(device.get(), (uint8*)buffer, size)) {
        cerr << "Session failed to initialize" << endl << flush;

        this->device.release();

        return false;
    }

    this->device.swap(device);

    return true;
}

long Cloudpilot::GetCyclesPerSecond() { return gSession->GetClocksPerSecond(); }

long Cloudpilot::RunEmulation(long cycles) { return gSession->RunEmulation(cycles); }

Frame& Cloudpilot::CopyFrame() {
    EmHAL::CopyLCDFrame(frame);

    return frame;
}

bool Cloudpilot::IsScreenDirty() { return gSystemState.IsScreenDirty(); }

void Cloudpilot::MarkScreenClean() { gSystemState.MarkScreenClean(); }

long Cloudpilot::MinMemoryForDevice(string id) {
    EmDevice device(id);

    return device.IsValid() ? device.MinRAMSize() : -1;
}
