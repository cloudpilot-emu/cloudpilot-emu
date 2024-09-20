//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

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

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #include <SDL.h>
    #include <SDL_image.h>

    #include <atomic>

    #include "SdlAudioDriver.h"
    #include "SdlEventHandler.h"
    #include "SdlRenderer.h"
#endif

#include "MainLoop.h"
#include "SoC.h"
#include "audio_queue.h"
#include "device.h"
#include "sdcard.h"
#include "util.h"

using namespace std;

#ifndef EMSCRIPTEN_KEEPALIVE
    #define EMSCRIPTEN_KEEPALIVE
#endif

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

#ifndef __EMSCRIPTEN__
    void initSdl(struct DeviceDisplayConfiguration displayConfiguration, int scale,
                 SDL_Window** window, SDL_Renderer** renderer, bool enableAudio) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | (enableAudio ? SDL_INIT_AUDIO : 0)) < 0) {
            printf("Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }

        IMG_Init(IMG_INIT_PNG);

        atexit(SDL_Quit);

        if (SDL_CreateWindowAndRenderer(
                displayConfiguration.width * scale,
                (displayConfiguration.height + displayConfiguration.graffitiHeight) * scale, 0,
                window, renderer) != 0) {
            fprintf(stderr, "unabe to initialize window");
            exit(1);
        }
    }
#endif
}  // namespace

extern "C" int socExtSerialReadChar(void) {
#if 1
    return CHAR_NONE;
#else
    timeval tv;
    fd_set set;
    char c;
    int i, ret = CHAR_NONE;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&set);
    FD_SET(0, &set);

    i = select(1, &set, NULL, NULL, &tv);
    if (i == 1 && 1 == read(0, &c, 1)) {
        ret = c;
    }

    return ret;
#endif
}

extern "C" void socExtSerialWriteChar(int chr) {
    if (!(chr & 0xFF00))
        printf("%c", chr);
    else
        printf("<<~~ EC_0x%x ~~>>", chr);

    fflush(stdout);
}

#ifdef __EMSCRIPTEN__

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

uint32_t EMSCRIPTEN_KEEPALIVE getNandDirtyPagesSize() { return socGetNandDirtyPages(soc).size; }

void* EMSCRIPTEN_KEEPALIVE getNandDirtyPages() { return socGetNandDirtyPages(soc).data; }

bool EMSCRIPTEN_KEEPALIVE isNandDirty() { return socIsNandDirty(soc); }

void EMSCRIPTEN_KEEPALIVE setNandDirty(bool isDirty) { socSetNandDirty(soc, isDirty); }

uint32_t EMSCRIPTEN_KEEPALIVE getSdCardDataSize() { return sdCardData().size; }

void* EMSCRIPTEN_KEEPALIVE getSdCardData() { return sdCardData().data; }

uint32_t EMSCRIPTEN_KEEPALIVE getSdCardDirtyPagesSize() { return sdCardDirtyPages().size; }

void* EMSCRIPTEN_KEEPALIVE getSdCardDirtyPages() { return sdCardDirtyPages().data; }

bool EMSCRIPTEN_KEEPALIVE isSdCardDirty() { return sdCardIsDirty(); }

void EMSCRIPTEN_KEEPALIVE setSdCardDirty(bool isDirty) { return sdCardSetDirty(isDirty); }

uint32_t EMSCRIPTEN_KEEPALIVE getRamDataSize() { return socGetRamData(soc).size; }

void* EMSCRIPTEN_KEEPALIVE getRamData() { return socGetRamData(soc).data; }

uint32_t EMSCRIPTEN_KEEPALIVE getRamDirtyPagesSize() { return socGetRamDirtyPages(soc).size; }

void* EMSCRIPTEN_KEEPALIVE getRamDirtyPages() { return socGetRamDirtyPages(soc).data; }

void EMSCRIPTEN_KEEPALIVE clearRamDirtyPages() {
    Buffer buffer = socGetRamDirtyPages(soc);
    memset(buffer.data, 0, buffer.size);
}
}
#endif

void run(uint8_t* rom, uint32_t romLen, uint8_t* nand, size_t nandLen, int gdbPort,
         bool enableAudio, uint32_t mips = 0) {
    soc = socInit(rom, romLen, sdCardSectorCount(), sdCardRead, sdCardWrite, nand, nandLen, gdbPort,
                  deviceGetSocRev());

    audioQueue = audioQueueCreate(AUDIO_QUEUE_SIZE);
    socSetAudioQueue(soc, audioQueue);

    mainLoop = make_unique<MainLoop>(soc);
    if (mips > 0) mainLoop->SetCyclesPerSecondLimit(mips * 1000000);

#ifndef __EMSCRIPTEN__
    constexpr int SCALE = 2;

    DeviceDisplayConfiguration displayConfiguration;
    deviceGetDisplayConfiguration(&displayConfiguration);

    SDL_Window* window;
    SDL_Renderer* renderer;

    initSdl(displayConfiguration, SCALE, &window, &renderer, enableAudio);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);
    SdlRenderer sdlRenderer(window, renderer, soc, SCALE);
    SdlEventHandler sdlEventHandler(soc, SCALE);
    unique_ptr<SdlAudioDriver> audioDriver;

    if (enableAudio) {
        audioDriver = make_unique<SdlAudioDriver>(soc, audioQueue);
        audioDriver->Start();
    }

    uint64_t lastSpeedDump = timestampUsec();

    while (true) {
        uint64_t now = timestampUsec();

        if (audioDriver) socSetPcmSuspended(soc, audioDriver->GetAudioBackpressure());

        mainLoop->Cycle(now);

        sdlRenderer.Draw(sdlEventHandler.RedrawRequested());
        sdlEventHandler.ClearRedrawRequested();

        sdlEventHandler.HandleEvents();

        if (now - lastSpeedDump > 1000000) {
            const uint64_t currentIps = mainLoop->GetCurrentIps();
            const uint64_t currentIpsMax = mainLoop->GetCurrentIpsMax();
            lastSpeedDump = now;

            cout << "current speed: " << currentIps << " IPS, theoretical speed: " << currentIpsMax
                 << " IPS -> " << (100 * currentIps) / currentIpsMax << "%" << endl
                 << flush;
        }

        const int64_t timesliceRemaining =
            mainLoop->GetTimesliceSizeUsec() - static_cast<int64_t>(timestampUsec() - now);

        if (timesliceRemaining > 10) usleep(timesliceRemaining);
    }
#endif
}

#ifndef __EMSCRIPTEN__
int main(int argc, char** argv) {
    uint32_t romLen = 0;
    size_t nandLen = 0;
    const char* self = argv[0];
    bool noRomMode = false;
    FILE* nandFile = NULL;
    FILE* romFile = NULL;
    uint8_t* rom = NULL;
    uint8_t* nand = NULL;
    int gdbPort = -1;
    bool enableAudio{true};
    int c;
    uint32_t mips = 0;

    while ((c = getopt(argc, argv, "g:s:r:n:m:xq")) != -1) switch (c) {
            case 'g':  // gdb port
                gdbPort = optarg ? atoi(optarg) : -1;
                if (gdbPort < 1024 || gdbPort > 65535) usage(self);
                break;

            case 's':  // sd card
                readSdCard(optarg);
                break;

            case 'r':  // ROM
                if (optarg) romFile = fopen(optarg, "rb");
                break;
    #if 0
            case 'x':  // NO_ROM mode
                noRomMode = true;
                break;
    #endif
            case 'n':  // NAND
                if (optarg) nandFile = fopen(optarg, "r+b");
                break;

            case 'q':
                enableAudio = false;
                break;

            case 'm':
                mips = atoi(optarg);
                if (mips < 50 || mips > 500) {
                    fprintf(stderr, "mips must be between 50 and 500\n");
                    exit(-1);
                }

                break;

            default:
                usage(self);
                break;
        }

    if ((romFile && noRomMode) || (!romFile && !noRomMode)) usage(self);

    if (romFile) {
        fseek(romFile, 0, SEEK_END);
        romLen = ftell(romFile);
        rewind(romFile);

        rom = (uint8_t*)malloc(romLen);
        if (!rom) {
            fprintf(stderr, "CANNOT ALLOC ROM\n");
            exit(-2);
        }

        if (romLen != fread(rom, 1, romLen, romFile)) {
            fprintf(stderr, "CANNOT READ ROM\n");
            exit(-2);
        }
        fclose(romFile);
    }

    if (nandFile) {
        fseek(nandFile, 0, SEEK_END);
        nandLen = ftell(nandFile);
        rewind(nandFile);

        nand = (uint8_t*)malloc(nandLen);
        if (!nand) {
            fprintf(stderr, "CANNOT ALLOC NAND\n");
            exit(-2);
        }

        if (nandLen != fread(nand, 1, nandLen, nandFile)) {
            fprintf(stderr, "CANNOT READ RNANDOM\n");
            exit(-2);
        }
        fclose(nandFile);
    }

    fprintf(stderr, "Read %u bytes of ROM\n", romLen);
    fprintf(stderr, "Read %lu bytes of NAND\n", nandLen);

    run(rom, romLen, nand, nandLen, gdbPort, enableAudio, mips);
}
#endif

#ifdef __EMSCRIPTEN__

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

#endif
