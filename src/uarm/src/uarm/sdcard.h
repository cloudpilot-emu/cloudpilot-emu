#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SD_SECTOR_SIZE 512

#ifdef __cplusplus
extern "C" {
#endif

void sdCardInitialize(size_t sectors);
void sdCardInitializeWithData(size_t sectors, void* data);

bool sdCardRead(uint32_t sector, void* data);
bool sdCardWrite(uint32_t sector, const void* data);

size_t sdCardSectorCount();
void* sdCardData();

#ifdef __cplusplus
}
#endif

#endif  // _SD_CARD_H_