#include "SessionImage.h"

#include "miniz.h"

namespace {
    constexpr uint32 MAGIC = 0x20150103;
    constexpr uint32 VERSION = 0x03;
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

const char* SessionImage::GetDeviceId() const { return deviceId.c_str(); }

SessionImage& SessionImage::SetDeviceId(const string deviceId) {
    this->deviceId = deviceId;

    return *this;
}

SessionImage& SessionImage::SetDeviceId(const char* deviceId) {
    this->deviceId = deviceId;

    return *this;
}

void* SessionImage::GetRomImage() const { return romImage; }

size_t SessionImage::GetRomImageSize() const { return romSize; }

SessionImage& SessionImage::SetRomImage(void* image, size_t size) {
    romImage = image;
    romSize = size;

    return *this;
}

void* SessionImage::GetMemoryImage() const { return ramImage; }

size_t SessionImage::GetMemoryImageSize() const { return ramSize; }

SessionImage& SessionImage::SetMemoryImage(void* image, size_t size) {
    ramImage = image;
    ramSize = size;

    return *this;
}

void* SessionImage::GetMetadata() const { return metadata; }

size_t SessionImage::GetMetadataSize() const { return metadataSize; }

SessionImage& SessionImage::SetMetadata(void* metadata, size_t size) {
    this->metadata = metadata;
    metadataSize = size;

    return *this;
}

void* SessionImage::GetSavestate() const { return savestate; }

size_t SessionImage::GetSavestateSize() const { return savestateSize; }

SessionImage& SessionImage::SetSavestate(void* savestate, size_t size) {
    this->savestate = savestate;
    savestateSize = size;

    return *this;
}

uint32 SessionImage::GetFramebufferSize() const { return framebufferSize; }

SessionImage& SessionImage::SetFramebufferSize(size_t framebufferSize) {
    this->framebufferSize = framebufferSize;

    return *this;
}

uint32 SessionImage::GetVersion() const { return version; }

void SessionImage::Serialize() {
    version = VERSION;

    const size_t uncompressedSize =
        24 + deviceId.size() + romSize + ramSize + savestateSize + metadataSize;

    auto uncompressed = make_unique<uint8[]>(uncompressedSize);
    uint8* uncompressedPtr = uncompressed.get();

    put32(uncompressedPtr, deviceId.size());
    put32(uncompressedPtr + 4, metadataSize);
    put32(uncompressedPtr + 8, romSize);
    put32(uncompressedPtr + 12, ramSize);
    put32(uncompressedPtr + 16, framebufferSize);
    put32(uncompressedPtr + 20, savestateSize);
    uncompressedPtr += 24;

    if (deviceId.size() > 0) memcpy(uncompressedPtr, deviceId.c_str(), deviceId.size());
    uncompressedPtr += deviceId.size();

    if (metadataSize > 0) memcpy(uncompressedPtr, metadata, metadataSize);
    uncompressedPtr += metadataSize;

    if (romSize > 0) memcpy(uncompressedPtr, romImage, romSize);
    uncompressedPtr += romSize;

    if (ramSize > 0) memcpy(uncompressedPtr, ramImage, ramSize);
    uncompressedPtr += ramSize;

    if (savestateSize > 0) memcpy(uncompressedPtr, savestate, savestateSize);

    mz_ulong compressedSize = uncompressedSize;
    serializationBuffer = make_unique<uint8[]>(compressedSize + 12);
    uint8* serializationBufferPtr = serializationBuffer.get();

    put32(serializationBufferPtr, MAGIC);
    put32(serializationBufferPtr + 4, VERSION | VERSION_MASK);
    put32(serializationBufferPtr + 8, uncompressedSize);
    serializationBufferPtr += 12;

    mz_compress(serializationBufferPtr, &compressedSize, uncompressed.get(), uncompressedSize);

    serizalizedImageSize = compressedSize + 12;
}

void* SessionImage::GetSerializedImage() const { return serializationBuffer.get(); }

size_t SessionImage::GetSerializedImageSize() const { return serizalizedImageSize; }

bool SessionImage::Deserialize(void* _buffer, size_t size) {
    uint8* buffer = static_cast<uint8*>(_buffer);

    if (size < 16) return false;
    if (get32(buffer) != MAGIC) return false;

    version = get32(buffer + 4);

    if (!(version & VERSION_MASK)) return DeserializeLegacyImage(buffer, size);
    version &= ~VERSION_MASK;

    if (version > VERSION) return false;

    if (version > 2) {
        uint32 uncompressedSize = get32(buffer + 8);
        deserializationBuffer = make_unique<uint8[]>(uncompressedSize);

        mz_ulong realUncompressedSize = uncompressedSize;
        if (uncompress(deserializationBuffer.get(), &realUncompressedSize, buffer + 12,
                       size - 12) != MZ_OK)
            return false;

        if (realUncompressedSize != uncompressedSize) return false;

        buffer = deserializationBuffer.get();
        size = uncompressedSize;
    } else {
        buffer += 8;
        size -= 8;
    }

    if (size < (version >= 2 ? 24 : 20)) return false;

    size_t deviceIdSize = get32(buffer);
    metadataSize = get32(buffer + 4);
    romSize = get32(buffer + 8);
    ramSize = get32(buffer + 12);
    framebufferSize = (version >= 2) ? get32(buffer + 16) : 0;
    savestateSize = get32(buffer + (version >= 2 ? 20 : 16));

    buffer += (version >= 2 ? 24 : 20);

    if (size != ((version >= 2 ? 24 : 20) + deviceIdSize + metadataSize + romSize + ramSize +
                 savestateSize) ||
        deviceIdSize > 16)
        return false;

    deviceId = string(reinterpret_cast<const char*>(buffer), deviceIdSize);
    buffer += deviceIdSize;

    metadata = metadataSize > 0 ? buffer : nullptr;
    buffer += metadataSize;

    romImage = romSize > 0 ? buffer : nullptr;
    buffer += romSize;

    ramImage = ramSize > 0 ? buffer : nullptr;
    buffer += ramSize;

    savestate = savestateSize > 0 ? buffer : nullptr;

    return true;
}

bool SessionImage::DeserializeLegacyImage(void* _buffer, size_t size) {
    uint8* buffer = static_cast<uint8*>(_buffer);

    size_t romNameSize = get32(buffer + 4);
    romSize = get32(buffer + 8);
    ramSize = get32(buffer + 12);

    buffer += 16;

    if (size != (16 + romNameSize + romSize + ramSize)) return false;

    buffer += romNameSize;
    deviceId = "PalmV";

    romImage = romSize > 0 ? buffer : nullptr;
    buffer += romSize;

    ramImage = ramSize > 0 ? buffer : nullptr;
    buffer += ramSize;

    return true;
}
