#include "Cloudpilot.h"

#include <cstdlib>

#include "ButtonEvent.h"
#include "DbInstaller.h"
#include "EmDevice.h"
#include "EmFileImport.h"
#include "EmHAL.h"
#include "EmROMReader.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Feature.h"
#include "NetworkProxy.h"
#include "SuspendManager.h"

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

void Cloudpilot::SetClockFactor(double clockFactor) { gSession->SetClockFactor(clockFactor); }

Frame& Cloudpilot::CopyFrame() {
    EmHAL::CopyLCDFrame(frame);

    return frame;
}

bool Cloudpilot::IsScreenDirty() { return gSystemState.IsScreenDirty(); }

bool Cloudpilot::IsUIInitialized() { return gSystemState.IsUIInitialized(); }

int Cloudpilot::GetOSVersion() { return gSystemState.OSVersion(); }

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

void Cloudpilot::QueueKeyboardEvent(uint16 c, bool ctrl) {
    gSession->QueueKeyboardEvent(KeyboardEvent(c, ctrl));
}

bool Cloudpilot::IsPowerOff() { return !gSession->IsPowerOn(); }

bool Cloudpilot::IsSetupComplete() { return gSystemState.IsSetupComplete(); }

void Cloudpilot::Reset() { gSession->Reset(EmSession::ResetType::soft); }

void Cloudpilot::ResetNoExtensions() { gSession->Reset(EmSession::ResetType::noext); }

void Cloudpilot::ResetHard() { gSession->Reset(EmSession::ResetType::hard); }

int Cloudpilot::InstallDb(void* buffer, size_t len) {
    return static_cast<int>(DbInstaller::Install(len, static_cast<uint8*>(buffer)));
}

int Cloudpilot::GetPalette2bitMapping() { return EmHAL::GetLCD2bitMapping(); }

void* Cloudpilot::GetMemoryPtr() { return gSession->GetMemoryPtr(); }

void* Cloudpilot::GetDirtyPagesPtr() { return gSession->GetDirtyPagesPtr(); }

int Cloudpilot::GetMemorySize() { return gSession->GetMemorySize(); }

void* Cloudpilot::GetRomPtr() {
    auto [romSize, romPtr] = gSession->GetRomImage();
    return romPtr;
}
int Cloudpilot::GetRomSize() {
    auto [romSize, romPtr] = gSession->GetRomImage();
    return romSize;
}

void* Cloudpilot::GetSavestatePtr() { return gSession->GetSavestate().GetBuffer(); }

int Cloudpilot::GetSavestateSize() { return gSession->GetSavestate().GetSize(); }

bool Cloudpilot::SaveState() { return gSession->Save(); }

bool Cloudpilot::LoadState(void* buffer, int len) {
    return gSession->Load(len, reinterpret_cast<uint8*>(buffer));
}

const char* Cloudpilot::GetHotsyncName() {
    static string name;
    name = gSystemState.GetHotsyncUserName();

    return name.c_str();
}

void Cloudpilot::SetHotsyncName(const char* name) { gSession->SetHotsyncUserName(name); }

void Cloudpilot::RegisterPwmHandler(uint32 handlerPtr) {
    typedef void (*handler_ptr)(double, double);

    EmHAL::onPwmChange.AddHandler(reinterpret_cast<handler_ptr>(handlerPtr));
}

DbBackup* Cloudpilot::StartBackup() { return new DbBackup(); }

void Cloudpilot::SetClipboardIntegration(bool toggle) { Feature::SetClipboardIntegration(toggle); }

bool Cloudpilot::GetClipboardIntegration() { return Feature::GetClipboardIntegration(); }

bool Cloudpilot::IsSuspended() { return SuspendManager::IsSuspended(); }

SuspendContext& Cloudpilot::GetSuspendContext() { return SuspendManager::GetContext(); }

void Cloudpilot::SetNetworkRedirection(bool toggle) { Feature::SetNetworkRedirection(toggle); }

bool Cloudpilot::GetNetworkRedirection() { return Feature::GetNetworkRedirection(); }

void Cloudpilot::RegisterProxyDisconnectHandler(uint32 handlerPtr) {
    typedef void (*handler_ptr)(const char*);

    gNetworkProxy.onDisconnect.AddHandler(reinterpret_cast<handler_ptr>(handlerPtr));
}
