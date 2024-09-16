#include "sdcard.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t sectorsTotal = 0;
static bool sdCardDirty = false;

static uint8_t* data = NULL;
static uint32_t* dirtyPages = NULL;

static size_t dirtyPagesSize = 0;

void sdCardInitializeWithData(size_t sectors, void* buf) {
    if (dirtyPages) free(dirtyPages);

    size_t dirtyPagesSize4 = sectors / (16 * 32);
    if ((dirtyPagesSize4 * 16 * 32) < sectors) dirtyPagesSize4++;

    data = buf;
    sectorsTotal = sectors;
    dirtyPagesSize = dirtyPagesSize4 * 4;
    sdCardDirty = false;

    dirtyPages = malloc(dirtyPagesSize);
    memset(dirtyPages, 0, dirtyPagesSize);
}

void sdCardInitialize(size_t sectors) {
    uint8_t* buf = malloc(sectors * SD_SECTOR_SIZE);
    memset(buf, 0, sectors * SD_SECTOR_SIZE);

    sdCardInitializeWithData(sectors, buf);
}

bool sdCardRead(uint32_t sector, void* buf) {
    if (sector >= sectorsTotal) return false;

    memcpy(buf, data + SD_SECTOR_SIZE * sector, SD_SECTOR_SIZE);

    return true;
}

bool sdCardWrite(uint32_t sector, const void* buf) {
    if (sector >= sectorsTotal) return false;

    memcpy(data + SD_SECTOR_SIZE * sector, buf, SD_SECTOR_SIZE);

    const uint32_t page = sector >> 4;
    dirtyPages[page / 32] |= (1u << (page % 32));

    sdCardDirty = true;
    return true;
}

size_t sdCardSectorCount() { return sectorsTotal; }

struct Buffer sdCardData() {
    return (struct Buffer){.size = sectorsTotal * SD_SECTOR_SIZE, .data = data};
}

struct Buffer sdCardDirtyPages() {
    return (struct Buffer){.size = dirtyPagesSize, .data = dirtyPages};
}

bool sdCardIsDirty() { return sdCardDirty; }

void sdCardSetDirty(bool isDirty) {
    sdCardDirty = isDirty;

    if (!isDirty) memset(dirtyPages, 0, dirtyPagesSize);
}
