#include "SessionImage.h"

namespace {
    constexpr uint32 MAGIC = 0x03012015;
    constexpr uint32 VERSION = 0x01;
    constexpr uint32 VERSION_MASK = 0x80000000;

    void put32(uint8* buffer, uint32 value) {
        buffer[0] = value & 0xff;
        buffer[1] = (value >> 8) & 0xff;
        buffer[2] = (value >> 16) & 0xff;
        buffer[3] = (value >> 24) & 0xff;
    }

    uint32 get32(uint8* buffer) {
        return buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
    }
}  // namespace

SessionImageSerializer& SessionImageSerializer::SetRomImage(size_t size, void* image) {
    romImage = image;
    romSize = size;

    return *this;
}

SessionImageSerializer& SessionImageSerializer::SetRamImage(size_t size, void* image) {
    ramImage = image;
    ramSize = size;

    return *this;
}

SessionImageSerializer& SessionImageSerializer::SetSavestate(size_t size, void* image) {
    savestate = image;
    savestateSize = size;

    return *this;
}

SessionImageSerializer& SessionImageSerializer::SetMetadata(size_t size, void* data) {
    metadata = data;
    metadataSize = size;

    return *this;
}

pair<size_t, unique_ptr<uint8[]>> SessionImageSerializer::Serialize(string deviceId) const {
    const size_t size = 28 + deviceId.size() + romSize + ramSize + savestateSize + metadataSize;
    auto image = make_unique<uint8[]>(size);
    uint8* buffer = image.get();

    put32(buffer, MAGIC);
    put32(buffer + 4, VERSION | VERSION_MASK);
    put32(buffer + 8, deviceId.size());
    put32(buffer + 12, metadataSize);
    put32(buffer + 16, romSize);
    put32(buffer + 20, ramSize);
    put32(buffer + 24, savestateSize);
    buffer += 28;

    memcpy(buffer, deviceId.c_str(), deviceId.size());
    buffer += deviceId.size();

    memcpy(buffer, metadata, metadataSize);
    buffer += metadataSize;

    memcpy(buffer, romImage, romSize);
    buffer += romSize;

    memcpy(buffer, ramImage, ramSize);
    buffer += ramSize;

    memcpy(buffer, savestate, savestateSize);

    return pair(size, std::move(image));
}

bool SessionImage::IsValid() const { return buffer != nullptr; }

SessionImage SessionImage::Deserialize(size_t size, uint8* buffer) {
    if (size < 28) return SessionImage();

    if (get32(buffer) != MAGIC) return SessionImage();
    if (get32(buffer + 4) != (VERSION | VERSION_MASK)) return SessionImage();

    size_t deviceIdSize = get32(buffer + 8);
    size_t metadataSize = get32(buffer + 12);
    size_t romSize = get32(buffer + 16);
    size_t ramSize = get32(buffer + 20);
    size_t savestateSize = get32(buffer + 24);

    buffer += 28;

    if (size != (28 + deviceIdSize + metadataSize + romSize + ramSize + savestateSize) ||
        deviceIdSize > 16)
        return SessionImage();

    SessionImage image;

    char deviceId[deviceIdSize + 1];
    memset(deviceId, 0, deviceIdSize + 1);
    memcpy(deviceId, buffer, deviceIdSize);
    buffer += deviceIdSize;

    image.deviceId = deviceId;
    image.buffer = buffer;

    image.metadataSize = metadataSize;
    if (metadataSize > 0) image.metadata = buffer;
    buffer += metadataSize;

    image.romSize = romSize;
    if (romSize > 0) image.romImage = buffer;
    buffer += romSize;

    image.ramSize = ramSize;
    if (ramSize > 0) image.ramImage = buffer;
    buffer += ramSize;

    image.savestateSize = savestateSize;
    if (savestateSize > 0) image.savestate = buffer;

    return image;
}

string SessionImage::GetDeviceId() const { return deviceId; }

pair<size_t, void*> SessionImage::GetRomImage() const { return pair(romSize, romImage); }

pair<size_t, void*> SessionImage::GetRamImage() const { return pair(ramSize, ramImage); }

pair<size_t, void*> SessionImage::GetMetadata() const { return pair(metadataSize, metadata); }

pair<size_t, void*> SessionImage::GetSavestate() const { return pair(savestateSize, savestate); }
