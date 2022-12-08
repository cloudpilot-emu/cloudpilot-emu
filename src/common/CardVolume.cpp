#include "CardVolume.h"

#include <cstdlib>
#include <iostream>

using namespace std;

namespace {
    constexpr uint16_t MAGIC_BOOT_SIGNATURE = 0xaa55;
}

CardVolume::CardVolume(CardImage& image)
    : image(image), imageData(image.RawData()), imageSize(image.BlocksTotal() * 512) {
    Identify();
}

CardVolume::Type CardVolume::GetType() const { return type; }

const std::string& CardVolume::InvalidReason() const { return invalidReason; }

uint32_t CardVolume::GetSize() const { return partitionSize; }

uint32_t CardVolume::GetGeometryHeads() const { return geometryHeads; }

uint32_t CardVolume::GetGeometrySectors() const { return geometrySectors; }

uint32_t CardVolume::GetSectorsPerCluster() const { return sectorsPerCluster; }

bool CardVolume::Read(uint32_t offset, uint32_t size, uint8_t* destination) {
    if (offset + size > partitionSize) return false;

    return image.ReadByteRange(destination, partitionOffset + offset, size);
}

bool CardVolume::Write(uint32_t offset, uint32_t size, const uint8_t* source) {
    if (offset + size > partitionSize) return false;

    return image.WriteByteRange(source, partitionOffset + offset, size);
}

void CardVolume::Format() {
    memset(imageData, 0, imageSize);

    CalculateGeometry();

    const AddressCHS firstSectorCHS = (AddressCHS){.cylinder = 0, .head = 0, .sector = 1};
    const AddressCHS lastSectorCHS = LBAToCHS(image.BlocksTotal() - 1);

    Write8(0x01be, 0x80);
    WriteAddressCHS(0x01bf, firstSectorCHS);
    Write8(0x01c2, 0x01);
    WriteAddressCHS(0x01c3, lastSectorCHS);
    Write32(0x01c6, 0x01);
    Write32(0x01ca, image.BlocksTotal() - 1);

    Write16(0x01fe, 0xaa55);

    image.MarkRangeDirty(0, 512);

    type = Type::partition;
    partitionOffset = 512;
    partitionSize = imageSize - 512;
}

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

    if (type == Type::bigFloppy) {
        partitionOffset = 0;
        partitionSize = imageSize;

        ReadFatParameters();
    }
}

uint8_t CardVolume::Read8(uint32_t addr) const { return addr >= imageSize ? 0 : imageData[addr]; }

uint16_t CardVolume::Read16(uint32_t addr) const {
    return addr >= imageSize - 1 ? 0 : (imageData[addr] | (imageData[addr + 1] << 8));
}

uint32_t CardVolume::Read32(uint32_t addr) const {
    return addr >= imageSize - 3 ? 0
                                 : (imageData[addr] | (imageData[addr + 1] << 8) |
                                    (imageData[addr + 2] << 16) | (imageData[addr + 3] << 24));
}

void CardVolume::Write8(uint32_t addr, uint8_t value) {
    if (addr >= imageSize) return;

    imageData[addr] = value;
}

void CardVolume::Write16(uint32_t addr, uint16_t value) {
    if (addr >= imageSize - 1) return;

    imageData[addr++] = value;
    imageData[addr] = value >> 8;
}

void CardVolume::Write32(uint32_t addr, uint32_t value) {
    if (addr >= imageSize - 3) return;

    imageData[addr++] = value;
    imageData[addr++] = value >> 8;
    imageData[addr++] = value >> 16;
    imageData[addr] = value >> 24;
}

bool CardVolume::ReadPartition(uint8_t index) {
    const uint32_t base = 0x01be + index * 16;

    const uint8_t partitionType = Read8(base + 0x04);
    const uint32_t partitionFirstSector = Read32(base + 0x08);
    const uint32_t sizeSectors = Read32(base + 0x0c);

    switch (partitionType) {
        case 0x01:
        case 0x04:
        case 0x06:
            break;

        default:
            return false;
    }

    if (sizeSectors == 0 || (partitionFirstSector + sizeSectors) * 512 > imageSize) return false;
    partitionOffset = partitionFirstSector * 512;

    if (Read16(partitionOffset + 0x01fe) != MAGIC_BOOT_SIGNATURE) return false;

    uint32_t sectorsTotalFromFat = Read16(partitionOffset + 0x13);
    if (sectorsTotalFromFat == 0) sectorsTotalFromFat = Read32(partitionOffset + 0x20);
    if (sectorsTotalFromFat == 0 || sectorsTotalFromFat > sizeSectors) return false;

    partitionSize = sizeSectors * 512;

    ReadFatParameters();

    AddressCHS startCHS = ReadAddressCHS(base + 0x01);
    AddressCHS sizeCHS = ReadAddressCHS(base + 0x05);

    return CHSToLBA(startCHS) == partitionFirstSector &&
           CHSToLBA(sizeCHS) == sizeSectors + partitionFirstSector - 1;
}

void CardVolume::ReadFatParameters() {
    geometrySectors = Read16(partitionOffset + 0x18);
    geometryHeads = Read16(partitionOffset + 0x1a);
    sectorsPerCluster = Read8(partitionOffset + 0x0d);
}

CardVolume::AddressCHS CardVolume::ReadAddressCHS(uint32_t index) {
    const uint8_t chs1 = Read8(index);
    const uint8_t chs2 = Read8(index + 1);
    const uint8_t chs3 = Read8(index + 2);

    return (AddressCHS){.head = chs1,
                        .sector = static_cast<uint8_t>(chs2 & 0x3f),
                        .cylinder = static_cast<uint16_t>(chs3 | ((chs2 & 0xc0) << 2))};
}

void CardVolume::WriteAddressCHS(uint32_t index, const AddressCHS& address) {
    Write8(index++, address.head);
    Write8(index++, (address.sector & 0x3f) | ((address.cylinder >> 2) & 0xc0));
    Write8(index, address.cylinder);
}

void CardVolume::CalculateGeometry() {
    uint32_t sectorsTotal = image.BlocksTotal();

    geometryHeads = sectorsTotal <= 32768    ? 2
                    : sectorsTotal <= 65536  ? 4
                    : sectorsTotal <= 262144 ? 8
                                             : 16;

    geometrySectors = sectorsTotal <= 4096 ? 16 : 32;
}

uint32_t CardVolume::CHSToLBA(const AddressCHS& addressCHS) const {
    return addressCHS.sector > 0
               ? ((addressCHS.cylinder * geometryHeads) + addressCHS.head) * geometrySectors +
                     (addressCHS.sector - 1)
               : 0;
}

CardVolume::AddressCHS CardVolume::LBAToCHS(uint32_t addressLBA) const {
    const uint16_t cylinder = addressLBA / (geometryHeads * geometrySectors);
    addressLBA -= cylinder * geometryHeads * geometrySectors;

    const uint8_t head = addressLBA / geometrySectors;
    addressLBA -= head * geometrySectors;

    return (AddressCHS){
        .cylinder = cylinder, .head = head, .sector = static_cast<uint8_t>(addressLBA + 1)};
}
