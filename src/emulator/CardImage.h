#ifndef _CARD_IMAGE_H_
#define _CARD_IMAGE_H_

#include <EmCommon.h>

class CardImage {
   public:
    constexpr static size_t BLOCK_SIZE = 512;

   public:
    CardImage(uint8* data, size_t blocksTotal);
    CardImage(CardImage&&) = default;
    CardImage& operator=(CardImage&&) = default;

    size_t Read(uint8* dest, size_t index, size_t count = 1);
    size_t Write(const uint8* source, size_t index, size_t count = 1);

   private:
    uint8* data;
    size_t blocksTotal;

   private:
    CardImage(const CardImage&) = delete;
    CardImage& operator=(const CardImage&) = delete;
};

#endif  // _CARD_IMAGE_H_
