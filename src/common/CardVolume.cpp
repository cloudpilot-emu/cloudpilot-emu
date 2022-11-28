#include "CardVolume.h"

#include <iostream>

using namespace std;

namespace {
    constexpr uint16_t MAGIC_BOOT_SIGNATURE = 0xaa55;
}

CardVolume::CardVolume(CardImage& image)
    : imageData(image.RawData()), imageSize(image.BlocksTotal() * 512) {
    Identify();
}

CardVolume::Type CardVolume::GetType() const { return type; }

const std::string& CardVolume::InvalidReason() const { return invalidReason; }

void CardVolume::Identify() {
    if (imageSize < 512) {
        invalidReason = "image too small";
        return;
    }

    if (Read16(0x01fe) != MAGIC_BOOT_SIGNATURE) {
        invalidReason = "no boot signature";
        return;
    }

    type = (ReadPartition(0) || ReadPartition(1) || ReadPartition(2) || ReadPartition(3))
               ? Type::partition
               : Type::bigFloppy;
}

uint8_t CardVolume::Read8(size_t addr) const { return addr >= imageSize ? 0 : imageData[addr]; }

uint16_t CardVolume::Read16(size_t addr) const {
    return addr >= imageSize - 1 ? 0 : (imageData[addr] | (imageData[addr + 1] << 8));
}

uint32_t CardVolume::Read32(size_t addr) const {
    return addr >= imageSize - 4 ? 0
                                 : (imageData[addr] | (imageData[addr + 1] << 8) |
                                    (imageData[addr + 2] << 16) | (imageData[addr + 3] << 24));
}

bool CardVolume::ReadPartition(uint8_t index) {
    size_t base = 0x01be + index * 16;

    uint8_t partitionType = Read8(base + 0x04);
    firstSector = Read32(base + 0x08);
    uint32_t sizeSectors = Read32(base + 0x0c);

    switch (partitionType) {
        case 0x01:
        case 0x04:
        case 0x06:
            break;

        default:
            return false;
    }

    if (sizeSectors == 0 || (firstSector + sizeSectors) * 512 - 1 >= imageSize) return false;
    firstByte = firstSector * 512;

    if (Read16(firstByte + 0x01fe) != MAGIC_BOOT_SIGNATURE) return false;

    uint32_t sectorsTotalCrosscheck = Read16(firstByte + 0x13);
    if (sectorsTotalCrosscheck == 0) sectorsTotalCrosscheck = Read32(firstByte + 0x20);

    return sectorsTotalCrosscheck == sizeSectors;
}
