#include "CardImage.h"

#include <algorithm>

CardImage::CardImage(uint8* data, size_t blocksTotal) : data(data), blocksTotal(blocksTotal) {}

size_t CardImage::Read(uint8* dest, size_t index, size_t count) {
    if (index >= blocksTotal) return 0;

    count = min(count, blocksTotal - index);

    for (size_t i = 0; i < count; i++)
        memcpy(dest + i * BLOCK_SIZE, data.get() + (i + index) * BLOCK_SIZE, BLOCK_SIZE);

    return count;
}

size_t CardImage::Write(const uint8* source, size_t index, size_t count) {
    if (index >= blocksTotal) return 0;

    count = min(count, blocksTotal - index);

    for (size_t i = 0; i < count; i++)
        memcpy(data.get() + (i + index) * BLOCK_SIZE, source + i * BLOCK_SIZE, BLOCK_SIZE);

    return count;
}

size_t CardImage::BlocksTotal() const { return blocksTotal; }

uint8* CardImage::RawData() { return data.get(); }
