#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "buffer.h"

#define SD_SECTOR_SIZE 512
#define SD_CARD_ID_MAX_LEN 32

#ifdef __cplusplus
extern "C" {
#endif

void sdCardInitialize(size_t sectors, const char* id);
void sdCardInitializeWithData(size_t sectors, void* buf, const char* id);

void sdCardRekey(const char* id);

void sdCardReset();

bool sdCardInitialized();

bool sdCardRead(uint32_t sector, void* data);
bool sdCardWrite(uint32_t sector, const void* data);

size_t sdCardSectorCount();

struct Buffer sdCardData();
struct Buffer sdCardDirtyPages();

bool sdCardIsDirty();
void sdCardSetDirty(bool isDirty);

const char* sdCardGetId();

#ifdef __cplusplus
}
#endif

#endif  // _SD_CARD_H_