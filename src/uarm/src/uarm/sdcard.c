#include "sdcard.h"

#include <stdlib.h>
#include <string.h>

static size_t sectorsTotal = 0;

static uint8_t* data = NULL;
static uint32_t* dirtyPages = NULL;

void sdCardInitialize(size_t sectors) {
    if (data) free(data);
    if (dirtyPages) free(dirtyPages);

    size_t dirtyPagesSize = sectors >> 9;
    if ((dirtyPagesSize << 9) < sectors) dirtyPagesSize++;

    data = malloc(SD_SECTOR_SIZE * sectors);
    dirtyPages = malloc(dirtyPagesSize);

    memset(data, 0, SD_SECTOR_SIZE * sectors);
    memset(dirtyPages, 0, dirtyPagesSize);

    sectorsTotal = sectors;
}

void sdCardInitializeWithData(size_t sectors, void* buf) {
    if (dirtyPages) free(dirtyPages);

    size_t dirtyPagesSize = sectors >> 9;
    if ((dirtyPagesSize << 9) < sectors) dirtyPagesSize++;

    data = buf;
    dirtyPages = malloc(dirtyPagesSize);

    memset(dirtyPages, 0, dirtyPagesSize);

    sectorsTotal = sectors;
}

bool sdCardRead(uint32_t sector, void* buf) {
    if (sector >= sectorsTotal) return false;

    memcpy(buf, data + SD_SECTOR_SIZE * sector, SD_SECTOR_SIZE);

    return true;
}

bool sdCardWrite(uint32_t sector, const void* buf) {
    if (sector >= sectorsTotal) return false;

    memcpy(data + SD_SECTOR_SIZE * sector, buf, SD_SECTOR_SIZE);

    const uint8_t page = sector >> 4;
    dirtyPages[page / 32] |= (1 << (page % 32));

    return true;
}

size_t sdCardSectorCount() { return sectorsTotal; }

void* sdCardData() { return data; }
