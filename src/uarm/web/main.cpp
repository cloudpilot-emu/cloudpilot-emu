//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <emscripten.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <memory>

#include "MainLoop.h"
#include "SoC.h"
#include "audio_queue.h"
#include "cputil.h"
#include "db_installer.h"
#include "device.h"
#include "rom_info.h"
#include "sdcard.h"

using namespace std;

#pragma clang diagnostic ignored "-Wunused-function"

namespace {
    constexpr size_t AUDIO_QUEUE_SIZE = 44100 / MAIN_LOOP_FPS * 10;

    SoC* soc = nullptr;

    AudioQueue* audioQueue = nullptr;
    unique_ptr<MainLoop> mainLoop;

    DeviceType deviceType = deviceTypeInvalid;
}  // namespace

extern "C" {

int socExtSerialReadChar(void) { return CHAR_NONE; }

void socExtSerialWriteChar(int chr) {
    if (!(chr & 0xFF00))
        printf("%c", chr);
    else
        printf("<<~~ EC_0x%x ~~>>", chr);

    fflush(stdout);
}

void EMSCRIPTEN_KEEPALIVE cycle(uint64_t now) {
    if (!mainLoop) return;

    mainLoop->Cycle(now);
}

void* EMSCRIPTEN_KEEPALIVE getFrame() {
    if (!soc) return nullptr;

    return socGetPendingFrame(soc);
}

void EMSCRIPTEN_KEEPALIVE resetFrame() {
    if (!soc) return;

    socResetPendingFrame(soc);
}

uint32_t EMSCRIPTEN_KEEPALIVE getTimesliceSizeUsec() {
    return mainLoop ? mainLoop->GetTimesliceSizeUsec() : 0;
}

void EMSCRIPTEN_KEEPALIVE penDown(int x, int y) {
    if (!soc) return;

    socPenDown(soc, x, y);
}

void EMSCRIPTEN_KEEPALIVE penUp() {
    if (!soc) return;

    socPenUp(soc);
}

uint32_t EMSCRIPTEN_KEEPALIVE currentIps() { return mainLoop ? mainLoop->GetCurrentIps() : 0; }

uint64_t EMSCRIPTEN_KEEPALIVE currentIpsMax() {
    return mainLoop ? mainLoop->GetCurrentIpsMax() : 0;
}

void EMSCRIPTEN_KEEPALIVE setMaxLoad(unsigned int maxLoad) {
    if (mainLoop) mainLoop->SetMaxLoad(maxLoad);
}

void EMSCRIPTEN_KEEPALIVE setCyclesPerSecondLimit(unsigned int cyclesPerSecondLimit) {
    if (mainLoop) mainLoop->SetCyclesPerSecondLimit(cyclesPerSecondLimit);
}

uint64_t EMSCRIPTEN_KEEPALIVE getTimestampUsec() { return timestampUsec(); }

void EMSCRIPTEN_KEEPALIVE keyDown(int key) {
    if (!soc) return;

    socKeyDown(soc, (enum KeyId)key);
}

void EMSCRIPTEN_KEEPALIVE keyUp(int key) {
    if (!soc) return;

    socKeyUp(soc, (enum KeyId)key);
}

uint32_t EMSCRIPTEN_KEEPALIVE pendingSamples() { return audioQueuePendingSamples(audioQueue); }

uint32_t* EMSCRIPTEN_KEEPALIVE popQueuedSamples() {
    static uint32_t samples[AUDIO_QUEUE_SIZE];

    audioQueuePopChunk(audioQueue, samples, audioQueuePendingSamples(audioQueue));

    return samples;
}

void EMSCRIPTEN_KEEPALIVE setPcmOutputEnabled(bool enabled) {
    socSetPcmOutputEnabled(soc, enabled);
}

void EMSCRIPTEN_KEEPALIVE setPcmSuspended(bool suspended) { socSetPcmSuspended(soc, suspended); }

uint32_t EMSCRIPTEN_KEEPALIVE getRomDataSize() { return socGetRomData(soc).size; }

void* EMSCRIPTEN_KEEPALIVE getRomData() { return socGetRomData(soc).data; }

uint32_t EMSCRIPTEN_KEEPALIVE getNandDataSize() { return socGetNandData(soc).size; }

void* EMSCRIPTEN_KEEPALIVE getNandData() { return socGetNandData(soc).data; }

void* EMSCRIPTEN_KEEPALIVE getNandDirtyPages() { return socGetNandDirtyPages(soc).data; }

bool EMSCRIPTEN_KEEPALIVE isNandDirty() { return socIsNandDirty(soc); }

void EMSCRIPTEN_KEEPALIVE setNandDirty(bool isDirty) { socSetNandDirty(soc, isDirty); }

uint32_t EMSCRIPTEN_KEEPALIVE getSdCardDataSize() { return sdCardData().size; }

void* EMSCRIPTEN_KEEPALIVE getSdCardData() { return sdCardData().data; }

void* EMSCRIPTEN_KEEPALIVE getSdCardDirtyPages() { return sdCardDirtyPages().data; }

bool EMSCRIPTEN_KEEPALIVE isSdCardDirty() { return sdCardIsDirty(); }

void EMSCRIPTEN_KEEPALIVE setSdCardDirty(bool isDirty) { return sdCardSetDirty(isDirty); }

uint32_t EMSCRIPTEN_KEEPALIVE getRamDataSize() { return socGetMemoryData(soc).size; }

void* EMSCRIPTEN_KEEPALIVE getRamData() { return socGetMemoryData(soc).data; }

void* EMSCRIPTEN_KEEPALIVE getRamDirtyPages() { return socGetMemoryDirtyPages(soc).data; }

uint32_t EMSCRIPTEN_KEEPALIVE getDeviceType() { return deviceType; }

bool EMSCRIPTEN_KEEPALIVE sdCardInsert(uint8_t* data, int length, const char* id) {
    if (socSdInserted(soc)) return false;

    sdCardInitializeWithData(length / SD_SECTOR_SIZE, data, id);
    socSdInsert(soc);

    return true;
}

void EMSCRIPTEN_KEEPALIVE sdCardEject() {
    socSdEject(soc);
    sdCardReset();
}

void EMSCRIPTEN_KEEPALIVE reset() { socReset(soc); }

void EMSCRIPTEN_KEEPALIVE save() { socSave(soc); }

uint32_t EMSCRIPTEN_KEEPALIVE getSavestateSize() { return socGetSavestate(soc).size; }

void* EMSCRIPTEN_KEEPALIVE getSavestateData() { return socGetSavestate(soc).data; }

bool EMSCRIPTEN_KEEPALIVE isSdInserted() { return socSdInserted(soc); }

uint32_t EMSCRIPTEN_KEEPALIVE getRamSize() { return socGetRamSize(soc); }

uint32_t EMSCRIPTEN_KEEPALIVE installDatabase(uint32_t len, void* data) {
    return dbInstallerInstall(socGetSyscallDispatch(soc), len, data);
}
}

void run(uint32_t ramSize, uint8_t* rom, uint32_t romLen, uint8_t* nand, size_t nandLen,
         uint8_t* savedMemory, size_t savedMemoryLen, uint8_t* savestate, size_t savestateLen,
         uint32_t mips = 0) {
    soc = socInit(deviceType, ramSize, rom, romLen, nand, nandLen, 0, deviceGetSocRev());

    audioQueue = audioQueueCreate(AUDIO_QUEUE_SIZE);
    socSetAudioQueue(soc, audioQueue);

    Buffer memory = socGetMemoryData(soc);
    if (savedMemoryLen > memory.size) {
        cerr << "ignoring invalid RAM snapshot" << endl;
        savedMemory = nullptr;
    }

    if (savedMemory) memcpy(memory.data, savedMemory, savedMemoryLen);

    if (savedMemory && !socLoad(soc, savestateLen, savestate)) {
        cerr << "failed to restore savestate" << endl;
    }

    if (socSdInserted(soc)) {
        if (!socSdRemount(soc)) {
            cerr << "failed to remount SD card" << endl;
            sdCardReset();
        }
    } else if (!savestate && sdCardInitialized()) {
        socSdInsert(soc);
    }

    mainLoop = make_unique<MainLoop>(soc);
    if (mips > 0) mainLoop->SetCyclesPerSecondLimit(mips * 1000000);
}

int main() {}

extern "C" EMSCRIPTEN_KEEPALIVE void webMain(uint32_t ramSize, uint8_t* rom, int romLen,
                                             uint8_t* nand, int nandLen, uint8_t* memory,
                                             int memoryLen, uint8_t* sd, int sdLen,
                                             uint8_t* savestate, int savestateLen,
                                             const char* sdId) {
    if (sd) sdCardInitializeWithData(sdLen / SD_SECTOR_SIZE, sd, sdId);

    RomInfo romInfo(rom, romLen);
    if (!romInfo.IsValid()) {
        cerr << "invalid NOR" << endl;
        return;
    }

    if (ramSize == 0)
        ramSize = (romInfo.GetCardName() == "PalmCard" && romInfo.GetHalId() == 'hspr')
                      ? (16ul << 20)
                      : (32ul << 20);

    cerr << romInfo << endl;
    cerr << "using " << ramSize << " bytes of RAM" << endl;

    deviceType = romInfo.GetDeviceType();

    if (ramSize == 0)
        ramSize = (romInfo.GetCardName() == "PalmCard" && romInfo.GetHalId() == 'hspr')
                      ? (16ul << 20)
                      : (32ul << 20);

    if (!deviceSupportsRamSize(ramSize)) {
        cerr << "unsupported RAM size: " << ramSize << " bytes" << endl;
        return;
    }

    run(ramSize, rom, (uint32_t)romLen, nand, (size_t)nandLen, memory, (size_t)memoryLen, savestate,
        (size_t)savestateLen);
}
