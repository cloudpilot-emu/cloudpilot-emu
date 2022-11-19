#ifndef _CARD_IMAGE_H_
#define _CARD_IMAGE_H_

#include <EmCommon.h>

#include <memory>

class CardImage {
   public:
    // Those two values cannot be changed --- they're here for documentation only!
    constexpr static size_t BLOCK_SIZE = 512;
    constexpr static size_t DIRTY_PAGE_SIZE = 8192;

   public:
    CardImage(uint8* data, size_t blocksTotal);

    size_t Read(uint8* dest, size_t index, size_t count = 1);
    size_t Write(const uint8* source, size_t index, size_t count = 1);
    size_t BlocksTotal() const;

    uint8* RawData();
    uint8* DirtyPages();

   private:
    unique_ptr<uint8[]> data;
    unique_ptr<uint8[]> dirtyPages;
    size_t blocksTotal;
};

#endif  // _CARD_IMAGE_H_
