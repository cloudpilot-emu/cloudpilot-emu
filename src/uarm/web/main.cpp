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
#include "device.h"
#include "sdcard.h"

using namespace std;

#pragma clang diagnostic ignored "-Wunused-function"

namespace {
    constexpr size_t AUDIO_QUEUE_SIZE = 44100 / MAIN_LOOP_FPS * 10;

    SoC* soc = nullptr;

    AudioQueue* audioQueue = nullptr;
    unique_ptr<MainLoop> mainLoop;

    void usage(const char* self) {
        fprintf(stderr,
                "USAGE: %s {-r ROMFILE.bin | -x} [-g gdbPort] [-s SDCARD_IMG.bin] [-n NAND.bin] "
                "[-q] [-m mips]\n",
                self);

        exit(-1);
    }

    void readSdCard(const char* fname) {
        FILE* cardFile = fopen(fname, "r+b");

        if (!cardFile) {
            fprintf(stderr, "unable to open sd card image %s\n", fname);
            exit(-4);
        }

        fseek(cardFile, 0, SEEK_END);
        size_t sdCardSize = ftell(cardFile);

        if (sdCardSize % SD_SECTOR_SIZE) {
            fprintf(stderr, "SD card image not a multiple of %u bytes\n", (unsigned)SD_SECTOR_SIZE);
            exit(-4);
        }

        sdCardInitialize(sdCardSize / SD_SECTOR_SIZE);

        fseek(cardFile, 0, SEEK_SET);
        size_t bytesRead = fread(sdCardData().data, 1, sdCardSize, cardFile);

        if (bytesRead != sdCardSize) {
            fprintf(stderr, "failed to read sd card image %lu %lu\n", bytesRead, sdCardSize);
            exit(4);
        }

        fclose(cardFile);
    }
}  // namespace

extern "C" int socExtSerialReadChar(void) { return CHAR_NONE; }

extern "C" void socExtSerialWriteChar(int chr) {
    if (!(chr & 0xFF00))
        printf("%c", chr);
    else
        printf("<<~~ EC_0x%x ~~>>", chr);

    fflush(stdout);
}

extern "C" {

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

uint32_t EMSCRIPTEN_KEEPALIVE getRamDataSize() { return socGetRamData(soc).size; }

void* EMSCRIPTEN_KEEPALIVE getRamData() { return socGetRamData(soc).data; }

void* EMSCRIPTEN_KEEPALIVE getRamDirtyPages() { return socGetRamDirtyPages(soc).data; }
}

void run(uint8_t* rom, uint32_t romLen, uint8_t* nand, size_t nandLen, int gdbPort,
         bool enableAudio, uint32_t mips = 0) {
    soc = socInit(rom, romLen, sdCardSectorCount(), sdCardRead, sdCardWrite, nand, nandLen, gdbPort,
                  deviceGetSocRev());

    audioQueue = audioQueueCreate(AUDIO_QUEUE_SIZE);
    socSetAudioQueue(soc, audioQueue);

    mainLoop = make_unique<MainLoop>(soc);
    if (mips > 0) mainLoop->SetCyclesPerSecondLimit(mips * 1000000);
}

int main() {}

extern "C" EMSCRIPTEN_KEEPALIVE void webMain(uint8_t* rom, int romLen, uint8_t* nand, int nandLen,
                                             uint8_t* sd, int sdLen) {
    if (sd) {
        fprintf(stderr, "using %u bytes of SD\n", sdLen);
        sdCardInitializeWithData(sdLen / SD_SECTOR_SIZE, sd);
    }

    fprintf(stderr, "using %u bytes of NOR\n", romLen);
    fprintf(stderr, "using %u bytes of NAND\n", nandLen);

    run(rom, (uint32_t)romLen, nand, (size_t)nandLen, 0, true);
}
