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

    #include "SdlEventHandler.h"
    #include "SdlRenderer.h"
#endif

#include "MainLoop.h"
#include "SoC.h"
#include "audio_queue.h"
#include "device.h"
#include "util.h"

using namespace std;

#ifndef EMSCRIPTEN_KEEPALIVE
    #define EMSCRIPTEN_KEEPALIVE
#endif

using namespace std;

#pragma clang diagnostic ignored "-Wunused-function"

namespace {
    constexpr uint32_t SD_SECTOR_SIZE = 512ULL;

    uint8_t* sdCardData = NULL;
    size_t sdCardSecs = 0;

    SoC* soc = nullptr;

    AudioQueue* audioQueue = nullptr;
    unique_ptr<MainLoop> mainLoop;

#ifndef __EMSCRIPTEN__
    std::atomic<bool> audioBuffering{true};
    bool audioBackpressure = false;
#endif

    void usage(const char* self) {
        fprintf(stderr,
                "USAGE: %s {-r ROMFILE.bin | --x} [-g gdbPort] [-s SDCARD_IMG.bin] [-n NAND.bin]\n",
                self);

        exit(-1);
    }

    extern "C" bool prvSdSectorR(uint32_t secNum, void* buf) {
        if (secNum >= sdCardSecs) return false;

        memcpy(buf, sdCardData + SD_SECTOR_SIZE * secNum, SD_SECTOR_SIZE);
        return true;
    }

    extern "C" bool prvSdSectorW(uint32_t secNum, const void* buf) {
        if (secNum >= sdCardSecs) return false;

        memcpy(sdCardData + SD_SECTOR_SIZE * secNum, buf, SD_SECTOR_SIZE);
        return true;
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

        sdCardSecs = sdCardSize / SD_SECTOR_SIZE;
        if (static_cast<uint64_t>(sdCardSecs) >> 32) {
            fprintf(stderr, "SD card too big: %llu sectors\n", (unsigned long long)sdCardSecs);
            exit(-5);
        }

        sdCardData = (uint8_t*)malloc(sdCardSize);

        fseek(cardFile, 0, SEEK_SET);
        size_t bytesRead = fread(sdCardData, 1, sdCardSize, cardFile);

        if (bytesRead != sdCardSize) {
            fprintf(stderr, "failed to read sd card image %lu %lu\n", bytesRead, sdCardSize);
            exit(4);
        }

        fclose(cardFile);
    }

#ifndef __EMSCRIPTEN__
    void audioCallback(void* userdata, uint8_t* stream, int len) {
        size_t samplesRemaining = len / 4;
        size_t samplesPending = audioQueuePendingSamples(audioQueue);

        if (audioBuffering && samplesPending > 44100 / 60 * 2) audioBuffering = false;

        if (!audioBuffering && samplesPending < 44100 / 60) {
            audioBuffering = true;

            cout << "audio underrun" << endl;
        }

        if (!audioBackpressure && samplesPending > 44100 / 60 * 4) audioBackpressure = true;
        if (audioBackpressure && samplesPending < 44100 / 60 * 3) audioBackpressure = false;

        if (!audioBuffering) {
            samplesRemaining -= audioQueuePopChunk(audioQueue, reinterpret_cast<uint32_t*>(stream),
                                                   samplesRemaining);
        }

        if (samplesRemaining > 0) {
            memset(stream + (len / 4 - samplesRemaining) * 4, 0, samplesRemaining * 4);
        }
    }

    void initSdl(struct DeviceDisplayConfiguration displayConfiguration, int scale,
                 SDL_Window** window, SDL_Renderer** renderer) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
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

        SDL_AudioSpec audioSpecRequested = {.freq = 44100,
                                            .format = AUDIO_S16,
                                            .channels = 2,
                                            .silence = 0,
                                            .samples = 512,
                                            .padding = 0,
                                            .size = 0,
                                            .callback = audioCallback,
                                            .userdata = nullptr};

        SDL_AudioSpec audioSpecActual;

        SDL_AudioDeviceID audioDevice =
            SDL_OpenAudioDevice(nullptr, 0, &audioSpecRequested, &audioSpecActual, 0);
        if (audioDevice == 0) {
            cout << "failed to open audio device" << endl;
        } else {
            cout << "audio device open" << endl;
        }

        SDL_PauseAudioDevice(audioDevice, 0);
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

uint64_t EMSCRIPTEN_KEEPALIVE getTimestampUsec() { return timestampUsec(); }

void EMSCRIPTEN_KEEPALIVE keyDown(int key) {
    if (!soc) return;

    socKeyDown(soc, (enum KeyId)key);
}

void EMSCRIPTEN_KEEPALIVE keyUp(int key) {
    if (!soc) return;

    socKeyUp(soc, (enum KeyId)key);
}
}
#endif

void run(uint8_t* rom, uint32_t romLen, uint8_t* nand, size_t nandLen, int gdbPort) {
    if (static_cast<uint64_t>(sdCardSecs) >> 32) {
        fprintf(stderr, "SD card too big: %llu sectors\n", (unsigned long long)sdCardSecs);
        exit(-5);
    }

    soc = socInit(rom, romLen, sdCardSecs, prvSdSectorR, prvSdSectorW, nand, nandLen, gdbPort,
                  deviceGetSocRev());

    audioQueue = audioQueueCreate(44100 / 10);
    socSetAudioQueue(soc, audioQueue);

    mainLoop = make_unique<MainLoop>(soc, 100000000);

#ifndef __EMSCRIPTEN__
    socSetPcmOutputEnabled(soc, true);

    constexpr int SCALE = 2;

    DeviceDisplayConfiguration displayConfiguration;
    deviceGetDisplayConfiguration(&displayConfiguration);

    SDL_Window* window;
    SDL_Renderer* renderer;

    initSdl(displayConfiguration, SCALE, &window, &renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);
    SdlRenderer sdlRenderer(window, renderer, soc, SCALE);
    SdlEventHandler sdlEventHandler(soc, SCALE);

    uint64_t lastSpeedDump = timestampUsec();

    while (true) {
        uint64_t now = timestampUsec();

        socSetPcmSuspended(soc, audioBackpressure);

        mainLoop->Cycle(now);
        sdlRenderer.Draw();
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
    int c;

    while ((c = getopt(argc, argv, "g:s:r:n:hx")) != -1) switch (c) {
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

    run(rom, romLen, nand, nandLen, gdbPort);
}
#endif

#ifdef __EMSCRIPTEN__

int main() {}

extern "C" EMSCRIPTEN_KEEPALIVE void webMain(uint8_t* rom, int romLen, uint8_t* nand, int nandLen,
                                             uint8_t* sd, int sdLen) {
    if (sd) {
        if (sdLen % SD_SECTOR_SIZE) {
            fprintf(stderr, "SD card image not a multiple of %u bytes\n", (unsigned)SD_SECTOR_SIZE);
            goto sdSetupComplete;
        }

        sdCardSecs = sdLen / SD_SECTOR_SIZE;
        if (static_cast<uint64_t>(sdCardSecs) >> 32) {
            fprintf(stderr, "SD card too big: %llu sectors\n", (unsigned long long)sdCardSecs);
            goto sdSetupComplete;
        }

        fprintf(stderr, "using %u bytes of SD\n", sdLen);
        sdCardData = sd;
    }

sdSetupComplete:

    fprintf(stderr, "using %u bytes of NOR\n", romLen);
    fprintf(stderr, "using %u bytes of NAND\n", nandLen);

    run(rom, (uint32_t)romLen, nand, (size_t)nandLen, 0);
}

#endif
