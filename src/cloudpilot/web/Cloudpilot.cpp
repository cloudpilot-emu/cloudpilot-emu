#include "Cloudpilot.h"

#include <cstdlib>
#include <cstring>

#include "ButtonEvent.h"
#include "DbInstaller.h"
#include "EmDevice.h"
#include "EmFileImport.h"
#include "EmHAL.h"
#include "EmROMReader.h"
#include "EmSPISlaveSD.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "ExternalStorage.h"
#include "Feature.h"
#include "LogDomain.h"
#include "Logging.h"
#include "MemoryStick.h"
#include "NetworkProxy.h"
#include "SuspendManager.h"

namespace {
    EmTransportSerialBuffer serialTransportIR;
    EmTransportSerialBuffer serialTransportSerial;

    unique_ptr<EmROMReader> createReader(void* buffer, int size) {
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

Cloudpilot::Cloudpilot() {
    gSession->SetTransportSerial(kUARTIR, &serialTransportIR);
    gSession->SetTransportSerial(kUARTSerial, &serialTransportSerial);
}

void* Cloudpilot::Malloc(int size) { return ::malloc(size); }

void Cloudpilot::Free(void* buffer) {
    if (buffer) ::free(buffer);
}

void* Cloudpilot::Nullptr() { return nullptr; }

void Cloudpilot::EnableLogging(bool enable) {
    logEnableDomain(logging::Domain::domainNetlib);

    if (enable) {
        logEnable();
    } else {
        logDisable();
    }
}

bool Cloudpilot::InitializeSession(void* buffer, int size, const char* deviceType) {
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

int Cloudpilot::GetCyclesPerSecond() { return gSession->GetClocksPerSecond(); }

int Cloudpilot::RunEmulation(int cycles) { return gSession->RunEmulation(cycles); }

void Cloudpilot::SetClockFactor(double clockFactor) { gSession->SetClockFactor(clockFactor); }

Frame& Cloudpilot::CopyFrame() {
    EmHAL::CopyLCDFrame(frame);

    return frame;
}

bool Cloudpilot::IsScreenDirty() { return gSystemState.IsScreenDirty(); }

bool Cloudpilot::IsUIInitialized() { return gSystemState.IsUIInitialized(); }

int Cloudpilot::GetOSVersion() { return gSystemState.OSVersion(); }

void Cloudpilot::MarkScreenClean() { gSystemState.MarkScreenClean(); }

int Cloudpilot::MinMemoryForDevice(string id) {
    EmDevice device(id);

    return device.IsValid() ? device.MinRAMSize() : -1;
}

int Cloudpilot::FramebufferSizeForDevice(string id) {
    EmDevice device(id);

    return device.IsValid() ? device.FramebufferSize() : -1;
}

int Cloudpilot::TotalMemoryForDevice(string id) {
    EmDevice device(id);

    return device.IsValid() ? device.TotalMemorySize() : -1;
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
    return static_cast<int>(DbInstaller(len, static_cast<uint8*>(buffer)).Install());
}

int Cloudpilot::GetPalette2bitMapping() { return EmHAL::GetLCD2bitMapping(); }

void* Cloudpilot::GetMemoryPtr() { return gSession->GetMemoryPtr(); }

void* Cloudpilot::GetDirtyPagesPtr() { return gSession->GetDirtyPagesPtr(); }

int Cloudpilot::GetMemorySize() { return gSession->GetMemorySize(); }

bool Cloudpilot::ImportMemoryImage(void* buffer, size_t len) {
    EmDevice& device(gSession->GetDevice());

    if (len == (device.MinRAMSize() + device.FramebufferSize()) * 1024)
        return EmMemory::LoadMemoryV2(buffer, len);

    if (static_cast<ssize_t>(len) == device.MinRAMSize() * 1024)
        return EmMemory::LoadMemoryV1(buffer, len);

    return EmMemory::LoadMemoryV4(buffer, len);
}

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

void Cloudpilot::SetNetworkRedirection(bool toggle) {
    Feature::SetNetworkRedirection(toggle);
    if (!toggle) gNetworkProxy.Reset();
}

bool Cloudpilot::GetNetworkRedirection() { return Feature::GetNetworkRedirection(); }

void Cloudpilot::RegisterProxyDisconnectHandler(uint32 handlerPtr) {
    typedef void (*handler_ptr)();

    gNetworkProxy.onDisconnect.AddHandler(reinterpret_cast<handler_ptr>(handlerPtr));
}

void Cloudpilot::SetHotsyncNameManagement(bool toggle) {
    Feature::SetHotsyncNameManagement(toggle);
}

bool Cloudpilot::LaunchAppByName(const char* name) {
    string encodedName = Utf8ToIsolatin1(name);
    if (encodedName.length() > 31) return false;

    return gSession->LaunchAppByName(encodedName);
}

bool Cloudpilot::LaunchAppByDbHeader(void* header, int len) {
    if (len < 32 || strnlen(static_cast<const char*>(header), 32) == 32) return false;

    return gSession->LaunchAppByName(static_cast<const char*>(header));
}

bool Cloudpilot::DeviceSupportsCardSize(uint32 size) {
    if (size % 512 != 0) return false;

    return EmHAL::SupportsImageInSlot(EmHAL::Slot::sdcard, size / 512) ||
           EmHAL::SupportsImageInSlot(EmHAL::Slot::memorystick, size / 512);
}

void Cloudpilot::ClearExternalStorage() { gExternalStorage.Clear(); }

bool Cloudpilot::AllocateCard(const char* key, uint32 blockCount) {
    uint8* data = new uint8[CardImage::BLOCK_SIZE * blockCount];
    memset(data, 0, CardImage::BLOCK_SIZE * blockCount);

    if (!gExternalStorage.AddImage(key, data, CardImage::BLOCK_SIZE * blockCount)) {
        delete[] data;
        return false;
    }

    return true;
}

bool Cloudpilot::AdoptCard(const char* key, void* data, uint32 blockCount) {
    if (!gExternalStorage.AddImage(key, static_cast<uint8_t*>(data),
                                   CardImage::BLOCK_SIZE * blockCount)) {
        free(data);
        return false;
    }

    return true;
}

bool Cloudpilot::MountCard(const char* key) { return gExternalStorage.Mount(key); }

bool Cloudpilot::RemoveCard(const char* key) { return gExternalStorage.RemoveImage(key); }

void* Cloudpilot::GetCardData(const char* key) {
    CardImage* image = gExternalStorage.GetImage(key);
    if (!image) return nullptr;

    return image->RawData();
}

void* Cloudpilot::GetCardDirtyPages(const char* key) {
    CardImage* image = gExternalStorage.GetImage(key);
    if (!image) return nullptr;

    return image->DirtyPages();
}

int Cloudpilot::GetCardSize(const char* key) {
    CardImage* image = gExternalStorage.GetImage(key);
    if (!image) return 0;

    return image->BlocksTotal() * CardImage::BLOCK_SIZE;
}

void Cloudpilot::RemountCards() { gExternalStorage.Remount(); }

int Cloudpilot::GetSupportLevel(uint32 size) {
    if (size % 512) return static_cast<int>(CardSupportLevel::unsupported);

    if (EmSPISlaveSD::IsSizeRepresentable(size >> 9) && MemoryStick::IsSizeRepresentable(size >> 9))
        return static_cast<int>(CardSupportLevel::sdAndMs);

    if (EmSPISlaveSD::IsSizeRepresentable(size >> 9))
        return static_cast<int>(CardSupportLevel::sdOnly);

    return static_cast<int>(CardSupportLevel::unsupported);
}

const char* Cloudpilot::GetMountedKey() {
    static string key;
    key = "";

    for (auto slot : {EmHAL::Slot::memorystick, EmHAL::Slot::sdcard}) {
        if (!gExternalStorage.IsMounted(slot)) continue;

        key = gExternalStorage.GetImageKeyInSlot(slot);
        break;
    }

    return key.c_str();
}

EmTransportSerialBuffer* Cloudpilot::GetTransportIR() { return &serialTransportIR; }

EmTransportSerialBuffer* Cloudpilot::GetTransportSerial() { return &serialTransportSerial; }
