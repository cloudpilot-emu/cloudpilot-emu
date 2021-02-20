#include "Cloudpilot.h"

#include <cstdlib>

#include "ButtonEvent.h"
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

    ButtonEvent::Button buttonFromId(int id) {
        switch (id) {
            case 0:
                return ButtonEvent::Button::app1;

            case 1:
                return ButtonEvent::Button::app2;

            case 2:
                return ButtonEvent::Button::app3;

            case 3:
                return ButtonEvent::Button::app4;

            case 4:
                return ButtonEvent::Button::rockerUp;

            case 5:
                return ButtonEvent::Button::rockerDown;

            case 6:
                return ButtonEvent::Button::power;

            case 7:
                return ButtonEvent::Button::cradle;

            default:
                return ButtonEvent::Button::invalid;
        }
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

    if (!gSession->Initialize(device.release(), (uint8*)buffer, size)) {
        cerr << "Session failed to initialize" << endl << flush;

        return false;
    }

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

void Cloudpilot::QueuePenMove(int x, int y) { gSession->QueuePenEvent(PenEvent::down(x, y)); }

void Cloudpilot::QueuePenUp() { gSession->QueuePenEvent(PenEvent::up()); }

void Cloudpilot::QueueButtonDown(int id) {
    gSession->QueueButtonEvent(ButtonEvent(buttonFromId(id), ButtonEvent::Type::press));
}

void Cloudpilot::QueueButtonUp(int id) {
    gSession->QueueButtonEvent(ButtonEvent(buttonFromId(id), ButtonEvent::Type::release));
}

bool Cloudpilot::IsPowerOff() { return !gSession->IsPowerOn(); }
