//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <SDL.h>
#include <SDL_image.h>
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
    #include "SdlEventHandler.h"
    #include "SdlRenderer.h"
#endif

#include "MainLoop.h"
#include "SoC.h"
#include "device.h"
#include "util.h"

using namespace std;

#ifndef EMSCRIPTEN_KEEPALIVE
    #define EMSCRIPTEN_KEEPALIVE
#endif

using namespace std;

namespace {
    constexpr uint32_t SD_SECTOR_SIZE = 512ULL;

    uint8_t* sdCardData = NULL;
    size_t sdCardSecs = 0;

    SoC* soc = nullptr;
    unique_ptr<MainLoop> mainLoop;

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

    void readSdCArd(const char* fname) {
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
    void initSdl(struct DeviceDisplayConfiguration displayConfiguration, int scale,
                 SDL_Window** window, SDL_Renderer** renderer) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
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

uint32_t EMSCRIPTEN_KEEPALIVE currentIpsMax() {
    return mainLoop ? mainLoop->GetCurrentIpsMax() : 0;
}
}
#endif

int main(int argc, char** argv) {
    uint32_t romLen = 0;
    const char* self = argv[0];
    bool noRomMode = false;
    FILE* nandFile = NULL;
    FILE* romFile = NULL;
    uint8_t* rom = NULL;
    int gdbPort = -1;
    int c;

    while ((c = getopt(argc, argv, "g:s:r:n:hx")) != -1) switch (c) {
            case 'g':  // gdb port
                gdbPort = optarg ? atoi(optarg) : -1;
                if (gdbPort < 1024 || gdbPort > 65535) usage(self);
                break;

            case 's':  // sd card
                readSdCArd(optarg);
                break;

            case 'r':  // ROM
                if (optarg) romFile = fopen(optarg, "rb");
                break;

            case 'x':  // NO_ROM mode
                noRomMode = true;
                break;

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

    fprintf(stderr, "Read %u bytes of ROM\n", romLen);

    soc = socInit((void**)&rom, &romLen, romLen ? 1 : 0, sdCardSecs, prvSdSectorR, prvSdSectorW,
                  nandFile, gdbPort, deviceGetSocRev());

    mainLoop = make_unique<MainLoop>(soc, 100000000);

#ifndef __EMSCRIPTEN__
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

    return 0;
}
