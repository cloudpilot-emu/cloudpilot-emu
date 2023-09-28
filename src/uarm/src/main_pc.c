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

static FILE* mSdCard = NULL;

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
    return fseeko(mSdCard, SD_SECTOR_SIZE * secNum, SEEK_SET) == 0 &&
           fread(buf, 1, SD_SECTOR_SIZE, mSdCard) == SD_SECTOR_SIZE;
}

static bool prvSdSectorW(uint32_t secNum, const void* buf) {
    return fseeko(mSdCard, SD_SECTOR_SIZE * secNum, SEEK_SET) == 0 &&
           fwrite(buf, 1, SD_SECTOR_SIZE, mSdCard) == SD_SECTOR_SIZE;
}

int main(int argc, char** argv) {
    uint32_t romLen = 0, sdSecs = 0;
    const char* self = argv[0];
    bool noRomMode = false;
    FILE* nandFile = NULL;
    FILE* romFile = NULL;
    uint8_t* rom = NULL;
    int gdbPort = -1;
    uint64_t sdSize;
    struct SoC* soc;
    int c;

    while ((c = getopt(argc, argv, "g:s:r:n:hx")) != -1) switch (c) {
            case 'g':  // gdb port
                gdbPort = optarg ? atoi(optarg) : -1;
                if (gdbPort < 1024 || gdbPort > 65535) usage(self);
                break;

            case 's':  // sd card
                if (optarg) mSdCard = fopen(optarg, "r+b");
                if (!mSdCard) usage(self);

                fseeko(mSdCard, 0, SEEK_END);
                sdSize = ftello(mSdCard);
                if (sdSize % SD_SECTOR_SIZE) {
                    fprintf(stderr, "SD card image not a multiple of %u bytes\n",
                            (unsigned)SD_SECTOR_SIZE);
                    exit(-4);
                }
                sdSize /= SD_SECTOR_SIZE;
                if (sdSize >> 32) {
                    fprintf(stderr, "SD card too big: %llu sectors\n", (unsigned long long)sdSize);
                    exit(-5);
                }
                sdSecs = sdSize;
                fprintf(stderr, "opened %lu-sector sd card image\n", (long)sdSecs);
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

    soc = socInit((void**)&rom, &romLen, romLen ? 1 : 0, sdSecs, prvSdSectorR, prvSdSectorW,
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
