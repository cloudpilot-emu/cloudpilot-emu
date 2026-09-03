#include "uarm.h"

#include <cstring>
#include <iostream>
#include <memory>

#include "Defer.h"
#include "app_launcher.h"
#include "audio_queue.h"
#include "buffer.h"
#include "cputil.h"
#include "db_backup.h"
#include "db_installer.h"
#include "device.h"
#include "device_configuration.h"
#include "device_type5.h"
#include "encoding.h"
#include "rom_info5.h"
#include "sdcard.h"
#include "soc_PXA.h"
#include "soc_pv.h"
#include "system_state.h"

using namespace std;

namespace {
    constexpr size_t AUDIO_QUEUE_SIZE = 44100 / MAIN_LOOP_FPS * 10;
    constexpr size_t NAND_SIZE = 34603008;
}  // namespace

Uarm& Uarm::SetRamSize(unsigned int size) {
    ramSize = size;
    return *this;
}

Uarm& Uarm::SetDisplayMode(unsigned int displayMode) {
    this->displayMode = static_cast<DisplayMode>(displayMode);
    return *this;
}

Uarm& Uarm::SetNand(unsigned int size, void* data) {
    nandSize = size;
    nandData = static_cast<uint8_t*>(data);

    return *this;
}

Uarm& Uarm::SetMemory(unsigned int size, void* data) {
    memorySize = size;
    memoryData = static_cast<uint8_t*>(data);

    return *this;
}

Uarm& Uarm::SetSavestate(unsigned int size, void* data) {
    savestateSize = size;
    savestateData = static_cast<uint8_t*>(data);

    return *this;
}

Uarm& Uarm::SetSd(unsigned int size, void* data, const char* id) {
    sdSize = size;
    sdData = static_cast<uint8_t*>(data);
    sdId = id;

    return *this;
}

Uarm& Uarm::SetDefaultMips(unsigned int defaultMips) {
    this->defaultMips = defaultMips;

    return *this;
}

bool Uarm::Launch(unsigned int romSize, void* romData) {
    Defer cleanup([&]() {
        if (memoryData) free(memoryData);
        if (savestateData) free(savestateData);
    });

    RomInfo5 romInfo(static_cast<uint8_t*>(romData), romSize);
    if (!romInfo.IsValid()) {
        cerr << "invalid NOR" << endl;
        return false;
    }
    deviceType = romInfo.GetDeviceType();

    if (sdSize > 0) sdCardInitializeWithData(sdSize / SD_SECTOR_SIZE, sdData, sdId.c_str());

    if (ramSize != 0 && !deviceConfigurationSupportsRamSize(deviceType, ramSize)) {
        cerr << "ignoring invalid RAM size " << ramSize << endl;
        ramSize = 0;
    }

    if (ramSize == 0) ramSize = romInfo.GetRecommendedRamSize();

    cout << romInfo << endl;
    cout << "using " << ramSize << " bytes of RAM" << endl;

    if (!displayMode.has_value()) displayMode = deviceConfigurationDefaultDisplayMode(deviceType);

    if (!deviceConfigurationDeviceSupportsDisplayMode(deviceType, *displayMode)) {
        cerr << "ignoring invalid display mode " << static_cast<int>(*displayMode) << endl;
        displayMode = deviceConfigurationDefaultDisplayMode(deviceType);
    }

    unique_ptr<uint8_t[]> nandStub;
    if (!nandData && deviceConfigurationGetNandSize(deviceType) > 0) {
        nandStub = make_unique<uint8_t[]>(NAND_SIZE);
        memset(nandStub.get(), 0xff, NAND_SIZE);
    }

    soc = deviceType == deviceTypePV
              ? static_cast<SoC*>(new SocPV(ramSize, romData, romSize, *displayMode, 0))
              :

              static_cast<SoC*>(new SocPXA(deviceType, ramSize, romData, romSize,
                                           nandData ? nandData : nandStub.get(),
                                           nandData ? nandSize : NAND_SIZE, 0, deviceGetSocRev()));

    audioQueue = audioQueueCreate(AUDIO_QUEUE_SIZE);
    soc->SetAudioQueue(audioQueue);

    Buffer memory = soc->GetMemoryData();
    if (memorySize > memory.size) {
        cerr << "ignoring invalid RAM snapshot" << endl;

        memorySize = 0;
        free(memoryData);
    }
    if (memoryData) memcpy(memory.data, memoryData, memorySize);

    if (memoryData && !soc->Load(savestateSize, savestateData)) {
        cerr << "failed to restore savestate" << endl;

        savestateData = nullptr;
        savestateSize = 0;
    }

    if (soc->SdInserted()) {
        if (!soc->SdRemount()) {
            cerr << "failed to remount SD card" << endl;
            sdCardReset();
        }
    } else if (!savestateData && sdCardInitialized())
        soc->SdInsert();

    mainLoop = make_unique<MainLoop>(soc);
    if (defaultMips > 0) mainLoop->SetCyclesPerSecondLimit(defaultMips * 1000000);

    return true;
}

double Uarm::Cycle(uint64_t now) { return mainLoop->Cycle(now); }

void* Uarm::GetFrame() { return soc->GetPendingFrame(); }

uint32_t Uarm::GetFrameSize() {
    DisplayConfiguration display = deviceConfigurationDisplayConfigForMode(soc->GetDisplayMode());

    return 4 * display.height * display.width;
}

void Uarm::ResetFrame() { soc->ResetPendingFrame(); }

uint32_t Uarm::GetTimesliceSizeUsec() { return mainLoop->GetTimesliceSizeUsec(); }

void Uarm::PenDown(int x, int y) { soc->PenDown(x, y); }

void Uarm::PenUp() { soc->PenUp(); }

uint32_t Uarm::CurrentIps() { return mainLoop->GetCurrentIps(); }

uint64_t Uarm::CurrentIpsMax() { return mainLoop->GetCurrentIpsMax(); }

void Uarm::SetMaxLoad(unsigned int maxLoad) { mainLoop->SetMaxLoad(maxLoad); }

void Uarm::SetCyclesPerSecondLimit(unsigned int cyclesPerSecondLimit) {
    mainLoop->SetCyclesPerSecondLimit(cyclesPerSecondLimit);
}

uint64_t Uarm::GetTimestampUsec() { return timestampUsec(); }

void Uarm::KeyDown(int key) { soc->KeyDown((enum KeyId)key); }

void Uarm::KeyUp(int key) { soc->KeyUp((enum KeyId)key); }

uint32_t Uarm::PendingSamples() { return audioQueuePendingSamples(audioQueue); }

uint32_t* Uarm::PopQueuedSamples() {
    static uint32_t samples[AUDIO_QUEUE_SIZE];
    audioQueuePopChunk(audioQueue, samples, audioQueuePendingSamples(audioQueue));

    return samples;
}

uint32_t Uarm::GetSampleQueueSize() { return AUDIO_QUEUE_SIZE; }

void Uarm::SetPcmOutputEnabled(bool enabled) { soc->SetPcmOutputEnabled(enabled); }

void Uarm::SetPcmSuspended(bool suspended) { soc->SetPcmSuspended(suspended); }

uint32_t Uarm::GetRomDataSize() { return soc->GetRomData().size; }

void* Uarm::GetRomData() { return soc->GetRomData().data; }

uint32_t Uarm::GetNandDataSize() { return soc->GetNandData().size; }

void* Uarm::GetNandData() { return soc->GetNandData().data; }

void* Uarm::GetNandDirtyPages() { return soc->GetNandDirtyPages().data; }

bool Uarm::IsNandDirty() { return soc->IsNandDirty(); }

void Uarm::SetNandDirty(bool isDirty) { soc->SetNandDirty(isDirty); }

uint32_t Uarm::GetSdCardDataSize() { return sdCardData().size; }

void* Uarm::GetSdCardData() { return sdCardData().data; }

void* Uarm::GetSdCardDirtyPages() { return sdCardDirtyPages().data; }

bool Uarm::IsSdCardDirty() { return sdCardIsDirty(); }

void Uarm::SetSdCardDirty(bool isDirty) { sdCardSetDirty(isDirty); }

uint32_t Uarm::GetMemoryDataSize() { return soc->GetMemoryData().size; }

void* Uarm::GetMemoryData() { return soc->GetMemoryData().data; }

void* Uarm::GetMemoryDirtyPages() { return soc->GetMemoryDirtyPages().data; }

uint32_t Uarm::GetDeviceType() { return static_cast<uint32_t>(deviceType); }

bool Uarm::SdCardInsert(void* data, int length, const char* id) {
    if (soc->SdInserted()) return false;

    sdCardInitializeWithData(length / SD_SECTOR_SIZE, data, id);
    soc->SdInsert();

    return true;
}

void Uarm::SdCardEject() {
    soc->SdEject();
    sdCardReset();
}

bool Uarm::SdCardInitialized() { return ::sdCardInitialized(); }

const char* Uarm::GetSdCardId() { return sdCardGetId(); }

void Uarm::Reset() { soc->Reset(); }

void Uarm::Save() { soc->Save(); }

uint32_t Uarm::GetSavestateSize() { return soc->GetSavestate().size; }

void* Uarm::GetSavestateData() { return soc->GetSavestate().data; }

bool Uarm::IsSdInserted() { return soc->SdInserted(); }

uint32_t Uarm::GetRamSize() { return soc->GetRamSize(); }

void Uarm::JamKey(int key, uint32_t durationMsec) {
    soc->JamKey(static_cast<KeyId>(key), durationMsec);
}

uint32_t Uarm::InstallDatabase(uint32_t len, void* data) {
    return dbInstallerInstall(soc->GetSyscallDispatch(), len, data);
}

DbBackup* Uarm::NewDbBackup(int type) { return new DbBackup(soc->GetSyscallDispatch(), type); }

bool Uarm::IsUiInitialized() { return systemStateIsUiInitialized(soc->GetSystemState()); }

bool Uarm::IsOsVersionSet() {
    return systemStateGetOsVersion(soc->GetSystemState()) != SYSTEM_STATE_OS_VERSION_UNDEFINED;
}

uint32_t Uarm::GetOsVersion() { return systemStateGetOsVersion(soc->GetSystemState()); }

bool Uarm::IsLcdEnabled() { return soc->LcdEnabled(); }

bool Uarm::LaunchAppByName(const char* name) {
    string encodedName = Utf8ToIsolatin1(name);
    if (encodedName.length() > 31) return false;

    return launchAppByName(soc->GetSyscallDispatch(), encodedName.c_str());
}

bool Uarm::LaunchAppByDbHeader(void* header, int len) {
    if (len < 32 || strnlen(static_cast<const char*>(header), 32) == 32) return false;

    return launchAppByName(soc->GetSyscallDispatch(), static_cast<const char*>(header));
}
