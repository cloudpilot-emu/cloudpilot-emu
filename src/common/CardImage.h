#ifndef _CARD_IMAGE_H_
#define _CARD_IMAGE_H_

#include <cstdint>
#include <memory>

class CardImage {
   public:
    // Those two values cannot be changed --- they're here for documentation only!
    constexpr static size_t BLOCK_SIZE = 512;
    constexpr static size_t DIRTY_PAGE_SIZE = 8192;

   public:
    CardImage(uint8_t* data, size_t blocksTotal);

    size_t Read(uint8_t* dest, size_t index, size_t count = 1);
    size_t Write(const uint8_t* source, size_t index, size_t count = 1);
    size_t BlocksTotal() const;

    bool WriteByteRange(const uint8_t* source, size_t offset, size_t count);
    bool ReadByteRange(uint8_t* destination, size_t offset, size_t count);

    void MarkRangeDirty(size_t offset, size_t count);

    uint8_t* RawData();
    uint8_t* DirtyPages();

   private:
    std::unique_ptr<uint8_t[]> data;
    std::unique_ptr<uint8_t[]> dirtyPages;
    size_t blocksTotal;
};

#endif  // _CARD_IMAGE_H_
