//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "SoC.h"
#include "device.h"

#define SD_SECTOR_SIZE (512ULL)

static uint8_t* sdCardData = NULL;
static size_t sdCardSecs = 0;

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
    socRun(soc);

    return 0;
}

//////// runtime things

void* emu_alloc(uint32_t size) { return calloc(size, 1); }

void emu_free(void* ptr) { free(ptr); }

uint32_t rtcCurTime(void) {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv.tv_sec;
}

void err_str(const char* str) { fprintf(stderr, "%s", str); }
