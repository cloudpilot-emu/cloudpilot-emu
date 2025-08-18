#include "uarm.h"

#include <iostream>

#include "Defer.h"
#include "SoC.h"
#include "audio_queue.h"
#include "buffer.h"
#include "device.h"
#include "rom_info.h"
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

    RomInfo romInfo(static_cast<uint8_t*>(romData), romSize);
    if (!romInfo.IsValid()) {
        cerr << "invalid NOR" << endl;
        return false;
    }

    if (sdSize > 0) sdCardInitializeWithData(sdSize / SD_SECTOR_SIZE, sdData, sdId.c_str());

    if (ramSize != 0 && !deviceSupportsRamSize(ramSize)) {
        cerr << "ignoring invalid RAM size " << ramSize << endl;
        ramSize = 0;
    }

    if (ramSize == 0)
        ramSize = (romInfo.GetCardName() == "PalmCard" && romInfo.GetHalId() == 'hspr')
                      ? (16ul << 20)
                      : (32ul << 20);

    cout << romInfo << endl;
    cout << "using " << ramSize << " bytes of RAM" << endl;

    if (ramSize == 0)
        ramSize = (romInfo.GetCardName() == "PalmCard" && romInfo.GetHalId() == 'hspr')
                      ? (16ul << 20)
                      : (32ul << 20);

    soc = socInit(romInfo.GetDeviceType(), ramSize, romData, romSize, nandData, nandSize, 0,
                  deviceGetSocRev());

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
