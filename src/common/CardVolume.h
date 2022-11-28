#ifndef _CARD_VOLUME_H_
#define _CARD_VOLUME_H_

#include <cstdint>
#include <string>

#include "CardImage.h"

class CardVolume {
   public:
    enum class Type : uint8_t { partition, bigFloppy, invalid };

   public:
    explicit CardVolume(CardImage& image);

    Type GetType() const;
    const std::string& InvalidReason() const;

   private:
    void Identify();

    uint8_t Read8(size_t addr) const;
    uint16_t Read16(size_t addr) const;
    uint32_t Read32(size_t addr) const;

    bool ReadPartition(uint8_t index);

   private:
    uint8_t* imageData;
    size_t imageSize{0};

    Type type{Type::invalid};
    std::string invalidReason{""};

    size_t firstByte{0};
    size_t firstSector{0};
};

#endif  // _CARD_VOLUME_H_
