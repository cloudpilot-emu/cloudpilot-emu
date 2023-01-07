#include "CardImage.h"

#include <algorithm>
#include <cstring>

CardImage::CardImage(uint8_t* data, size_t blocksTotal) : data(data), blocksTotal(blocksTotal) {
    const size_t pageCount = (blocksTotal >> 4) + ((blocksTotal % 16 != 0) > 0 ? 1 : 0);
    const size_t dirtyPageBufferSize = (pageCount >> 3) + ((pageCount % 8) > 0 ? 1 : 0);

    dirtyPages = std::make_unique<uint8_t[]>(dirtyPageBufferSize);
    memset(dirtyPages.get(), 0, dirtyPageBufferSize);
}

size_t CardImage::Read(uint8_t* dest, size_t index, size_t count) {
    if (index >= blocksTotal) return 0;

    count = std::min(count, blocksTotal - index);

    for (size_t i = 0; i < count; i++)
        memcpy(dest + i * BLOCK_SIZE, data.get() + (i + index) * BLOCK_SIZE, BLOCK_SIZE);

    return count;
}

size_t CardImage::Write(const uint8_t* source, size_t index, size_t count) {
    if (index >= blocksTotal) return 0;

    count = std::min(count, blocksTotal - index);

    for (size_t block = index; block < index + count; block++) {
        memcpy(data.get() + block * BLOCK_SIZE, source + (block - index) * BLOCK_SIZE, BLOCK_SIZE);

        const size_t page = block >> 4;
        dirtyPages[page >> 3] |= 1 << (page & 0x07);
    }

    return count;
}

size_t CardImage::BlocksTotal() const { return blocksTotal; }

bool CardImage::WriteByteRange(const uint8_t* source, size_t offset, size_t count) {
    if (offset + count > blocksTotal * BLOCK_SIZE) return false;
    if (count == 0) return true;

    memcpy(data.get() + offset, source, count);

    MarkRangeDirty(offset, count);

    return true;
}

bool CardImage::ReadByteRange(uint8_t* destination, size_t offset, size_t count) {
    if (offset + count > blocksTotal * BLOCK_SIZE) return false;

    memcpy(destination, data.get() + offset, count);

    return true;
}

void CardImage::MarkRangeDirty(size_t offset, size_t count) {
    const size_t firstBlock = offset / BLOCK_SIZE;
    const size_t lastBlock = (offset + count - 1) / BLOCK_SIZE;

    for (size_t block = firstBlock; block <= lastBlock; block++) {
        const size_t page = block >> 4;
        dirtyPages[page >> 3] |= 1 << (page & 0x07);
    }
}

uint8_t* CardImage::RawData() { return data.get(); }

uint8_t* CardImage::DirtyPages() { return dirtyPages.get(); }
