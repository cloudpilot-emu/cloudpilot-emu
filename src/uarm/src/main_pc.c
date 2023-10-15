//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <SDL.h>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "SoC.h"
#include "device.h"

#define SD_SECTOR_SIZE (512ULL)

struct MainLoopContext {
    uint64_t cycles_per_second;

    uint64_t real_time_musec;
    double virtual_time_musec;
};

static uint8_t* sdCardData = NULL;
static size_t sdCardSecs = 0;

uint64_t timestampMusec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

int socExtSerialReadChar(void) {
    struct timeval tv;
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

void socExtSerialWriteChar(int chr) {
    if (!(chr & 0xFF00))
        printf("%c", chr);
    else
        printf("<<~~ EC_0x%x ~~>>", chr);

    fflush(stdout);
}

static void usage(const char* self) {
    fprintf(stderr,
            "USAGE: %s {-r ROMFILE.bin | --x} [-g gdbPort] [-s SDCARD_IMG.bin] [-n NAND.bin]\n",
            self);
    exit(-1);
}

static bool prvSdSectorR(uint32_t secNum, void* buf) {
    if (secNum >= sdCardSecs) return false;

    memcpy(buf, sdCardData + SD_SECTOR_SIZE * secNum, SD_SECTOR_SIZE);
    return true;
}

static bool prvSdSectorW(uint32_t secNum, const void* buf) {
    if (secNum >= sdCardSecs) return false;

    memcpy(sdCardData + SD_SECTOR_SIZE * secNum, buf, SD_SECTOR_SIZE);
    return true;
}

static void readSdCArd(const char* fname) {
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
    if (sdCardSecs >> 32) {
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

static void mainLoop(struct SoC* soc, struct MainLoopContext* ctx, int scale) {
    const uint64_t now = timestampMusec();
    double delta = (double)now - ctx->real_time_musec;
    ctx->real_time_musec = now;

    const uint64_t cycles_emulated =
        socRun(soc, round((delta * (double)ctx->cycles_per_second) / 1000000.), scale);

    ctx->virtual_time_musec += (double)cycles_emulated * 1000000. / (double)ctx->cycles_per_second;
}

void initSdl(struct DeviceDisplayConfiguration displayConfiguration, int scale, SDL_Window** window,
             SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    if (SDL_CreateWindowAndRenderer(
            displayConfiguration.width * scale,
            (displayConfiguration.height + displayConfiguration.graffitiHeight) * scale, 0, window,
            renderer) != 0) {
        fprintf(stderr, "unabe to initialize window");
        exit(1);
    }
}

int main(int argc, char** argv) {
    uint32_t romLen = 0;
    const char* self = argv[0];
    bool noRomMode = false;
    FILE* nandFile = NULL;
    FILE* romFile = NULL;
    uint8_t* rom = NULL;
    int gdbPort = -1;
    struct SoC* soc;
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

    uint64_t now = timestampMusec();
    struct MainLoopContext ctx = {
        .cycles_per_second = 25000000, .real_time_musec = now, .virtual_time_musec = now};

    struct DeviceDisplayConfiguration displayConfiguration;
    deviceGetDisplayConfiguration(&displayConfiguration);

    SDL_Window* window;
    SDL_Renderer* renderer;
    const int scale = 2;

    initSdl(displayConfiguration, scale, &window, &renderer);

    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING,
                          displayConfiguration.width, displayConfiguration.height);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);

    while (true) {
        mainLoop(soc, &ctx, scale);

        uint32_t* frame = socGetPendingFrame(soc);
        if (frame) {
            uint8_t* pixels;
            int pitch;
            SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

            if (pitch == 4 * displayConfiguration.width) {
                memcpy(pixels, frame, 4 * displayConfiguration.width * displayConfiguration.height);
            } else {
                for (int y = 0; y < displayConfiguration.height; y++) {
                    memcpy(pixels, frame, 4 * displayConfiguration.width);
                    frame += displayConfiguration.width;
                    pixels += pitch;
                }
            }

            SDL_UnlockTexture(texture);

            SDL_Rect src = {
                .x = 0, .y = 0, .w = displayConfiguration.width, .h = displayConfiguration.height};
            SDL_Rect dest = {.x = 0, .y = 0, .w = scale * src.w, .h = scale * src.h};

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, &src, &dest);
            SDL_RenderPresent(renderer);

            socResetPendingFrame(soc);
        } else {
            usleep(timestampMusec() % (1000000 / 50));
        }
    }

    return 0;
}
