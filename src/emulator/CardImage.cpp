#include "CardImage.h"

#include <algorithm>

CardImage::CardImage(uint8* data, size_t blocksTotal) : data(data), blocksTotal(blocksTotal) {
    const size_t pageCount = (blocksTotal >> 4) + ((blocksTotal % 16 != 0) > 0 ? 1 : 0);
    const size_t dirtyPageBufferSize = (pageCount >> 3) + ((pageCount % 8) > 0 ? 1 : 0);

    dirtyPages = make_unique<uint8[]>(dirtyPageBufferSize);
    memset(dirtyPages.get(), 0, dirtyPageBufferSize);
}

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

    for (size_t block = index; block < index + count; block++) {
        memcpy(data.get() + block * BLOCK_SIZE, source + (block - index) * BLOCK_SIZE, BLOCK_SIZE);

        const size_t page = block >> 4;
        dirtyPages[page >> 3] |= 1 << (page & 0x07);
    }

    return count;
}

size_t CardImage::BlocksTotal() const { return blocksTotal; }

uint8* CardImage::RawData() { return data.get(); }

uint8* CardImage::DirtyPages() { return dirtyPages.get(); }
