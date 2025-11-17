#include "uarm.h"

#include <iostream>

#include "Defer.h"
#include "SoC.h"
#include "audio_queue.h"
#include "buffer.h"
#include "cputil.h"
#include "db_backup.h"
#include "db_installer.h"
#include "device.h"
#include "rom_info5.h"
#include "sdcard.h"

using namespace std;

namespace {
    constexpr size_t AUDIO_QUEUE_SIZE = 44100 / MAIN_LOOP_FPS * 10;

}

Uarm& Uarm::SetRamSize(unsigned int size) {
    ramSize = size;
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

    if (sdSize > 0) sdCardInitializeWithData(sdSize / SD_SECTOR_SIZE, sdData, sdId.c_str());

    if (ramSize != 0 && !deviceSupportsRamSize(ramSize)) {
        cerr << "ignoring invalid RAM size " << ramSize << endl;
        ramSize = 0;
    }

    if (ramSize == 0) ramSize = romInfo.GetRecommendedRamSize();

    cout << romInfo << endl;
    cout << "using " << ramSize << " bytes of RAM" << endl;

    deviceType = romInfo.GetDeviceType();
    soc = socInit(deviceType, ramSize, romData, romSize, nandData, nandSize, 0, deviceGetSocRev());

    audioQueue = audioQueueCreate(AUDIO_QUEUE_SIZE);
    socSetAudioQueue(soc, audioQueue);

    Buffer memory = socGetMemoryData(soc);
    if (memorySize > memory.size) {
        cerr << "ignoring invalid RAM snapshot" << endl;

        memorySize = 0;
        free(memoryData);
    }
    if (memoryData) memcpy(memory.data, memoryData, memorySize);

    if (memoryData && !socLoad(soc, savestateSize, savestateData)) {
        cerr << "failed to restore savestate" << endl;

        savestateData = nullptr;
        savestateSize = 0;
    }

    if (socSdInserted(soc)) {
        if (!socSdRemount(soc)) {
            cerr << "failed to remount SD card" << endl;
            sdCardReset();
        }
    } else if (!savestateData && sdCardInitialized())
        socSdInsert(soc);

    mainLoop = make_unique<MainLoop>(soc);
    if (defaultMips > 0) mainLoop->SetCyclesPerSecondLimit(defaultMips * 1000000);

    return true;
}

double Uarm::Cycle(uint64_t now) { return mainLoop->Cycle(now); }

void* Uarm::GetFrame() { return socGetPendingFrame(soc); }

void Uarm::ResetFrame() { socResetPendingFrame(soc); }

uint32_t Uarm::GetTimesliceSizeUsec() { return mainLoop->GetTimesliceSizeUsec(); }

void Uarm::PenDown(int x, int y) { socPenDown(soc, x, y); }

void Uarm::PenUp() { socPenUp(soc); }

uint32_t Uarm::CurrentIps() { return mainLoop->GetCurrentIps(); }

uint64_t Uarm::CurrentIpsMax() { return mainLoop->GetCurrentIpsMax(); }

void Uarm::SetMaxLoad(unsigned int maxLoad) { mainLoop->SetMaxLoad(maxLoad); }

void Uarm::SetCyclesPerSecondLimit(unsigned int cyclesPerSecondLimit) {
    mainLoop->SetCyclesPerSecondLimit(cyclesPerSecondLimit);
}

uint64_t Uarm::GetTimestampUsec() { return timestampUsec(); }

void Uarm::KeyDown(int key) { socKeyDown(soc, (enum KeyId)key); }

void Uarm::KeyUp(int key) { socKeyUp(soc, (enum KeyId)key); }

uint32_t Uarm::PendingSamples() { return audioQueuePendingSamples(audioQueue); }

uint32_t* Uarm::PopQueuedSamples() {
    static uint32_t samples[AUDIO_QUEUE_SIZE];
    audioQueuePopChunk(audioQueue, samples, audioQueuePendingSamples(audioQueue));

    return samples;
}

uint32_t Uarm::GetSampleQueueSize() { return AUDIO_QUEUE_SIZE; }

void Uarm::SetPcmOutputEnabled(bool enabled) { socSetPcmOutputEnabled(soc, enabled); }

void Uarm::SetPcmSuspended(bool suspended) { socSetPcmSuspended(soc, suspended); }

uint32_t Uarm::GetRomDataSize() { return socGetRomData(soc).size; }

void* Uarm::GetRomData() { return socGetRomData(soc).data; }

uint32_t Uarm::GetNandDataSize() { return socGetNandData(soc).size; }

void* Uarm::GetNandData() { return socGetNandData(soc).data; }

void* Uarm::GetNandDirtyPages() { return socGetNandDirtyPages(soc).data; }

bool Uarm::IsNandDirty() { return socIsNandDirty(soc); }

void Uarm::SetNandDirty(bool isDirty) { socSetNandDirty(soc, isDirty); }

uint32_t Uarm::GetSdCardDataSize() { return sdCardData().size; }

void* Uarm::GetSdCardData() { return sdCardData().data; }

void* Uarm::GetSdCardDirtyPages() { return sdCardDirtyPages().data; }

bool Uarm::IsSdCardDirty() { return sdCardIsDirty(); }

void Uarm::SetSdCardDirty(bool isDirty) { sdCardSetDirty(isDirty); }

uint32_t Uarm::GetMemoryDataSize() { return socGetMemoryData(soc).size; }

void* Uarm::GetMemoryData() { return socGetMemoryData(soc).data; }

void* Uarm::GetMemoryDirtyPages() { return socGetMemoryDirtyPages(soc).data; }

uint32_t Uarm::GetDeviceType() { return static_cast<uint32_t>(deviceType); }

bool Uarm::SdCardInsert(void* data, int length, const char* id) {
    if (socSdInserted(soc)) return false;

    sdCardInitializeWithData(length / SD_SECTOR_SIZE, data, id);
    socSdInsert(soc);

    return true;
}

void Uarm::SdCardEject() {
    socSdEject(soc);
    sdCardReset();
}

bool Uarm::SdCardInitialized() { return ::sdCardInitialized(); }

const char* Uarm::GetSdCardId() { return sdCardGetId(); }

void Uarm::Reset() { socReset(soc); }

void Uarm::Save() { socSave(soc); }

uint32_t Uarm::GetSavestateSize() { return socGetSavestate(soc).size; }

void* Uarm::GetSavestateData() { return socGetSavestate(soc).data; }

bool Uarm::IsSdInserted() { return socSdInserted(soc); }

uint32_t Uarm::GetRamSize() { return socGetRamSize(soc); }

uint32_t Uarm::InstallDatabase(uint32_t len, void* data) {
    return dbInstallerInstall(socGetSyscallDispatch(soc), len, data);
}

DbBackup* Uarm::NewDbBackup(int type) { return new DbBackup(socGetSyscallDispatch(soc), type); }
