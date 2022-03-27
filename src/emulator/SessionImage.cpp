#include "SessionImage.h"

#include "miniz.h"

namespace {
    constexpr uint32 MAGIC = 0x20150103;
    constexpr uint32 VERSION = 0x03;
    constexpr uint32 VERSION_MASK = 0x80000000;
    constexpr size_t COMPRESSED_GROW_LIMIT = 128 * 1024 * 1024;

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

bool SessionImage::Serialize() {
    constexpr size_t HEADER_SIZE = 12;

#define STREAM_OUT(ptr, size)                                                                   \
    stream.next_in = reinterpret_cast<const unsigned char*>(ptr);                               \
    stream.avail_in = size;                                                                     \
                                                                                                \
    int deflateResult;                                                                          \
    do {                                                                                        \
        deflateResult = deflate(&stream, Z_NO_FLUSH);                                           \
                                                                                                \
        if (deflateResult == Z_BUF_ERROR &&                                                     \
            (stream.avail_out + stream.total_out + HEADER_SIZE) != COMPRESSED_GROW_LIMIT) {     \
            size_t growTo = min(((stream.total_out + stream.avail_out + HEADER_SIZE) * 3) / 2,  \
                                COMPRESSED_GROW_LIMIT);                                         \
            unique_ptr<uint8[]> newBuffer = make_unique<uint8[]>(growTo);                       \
                                                                                                \
            memcpy(newBuffer.get(), serializationBuffer.get(), HEADER_SIZE + stream.total_out); \
                                                                                                \
            stream.next_out = newBuffer.get() + stream.total_out + HEADER_SIZE;                 \
            stream.avail_out = growTo - stream.total_out - HEADER_SIZE;                         \
                                                                                                \
            serializationBuffer.swap(newBuffer);                                                \
            serializationBufferPtr = serializationBuffer.get();                                 \
                                                                                                \
            deflateResult = Z_OK;                                                               \
        }                                                                                       \
    } while (stream.avail_in != 0 && deflateResult == Z_OK);                                    \
                                                                                                \
    if (deflateResult != Z_OK) {                                                                \
        deflateEnd(&stream);                                                                    \
                                                                                                \
        return false;                                                                           \
    }

    version = VERSION;

    const size_t uncompressedSize =
        24 + deviceId.size() + romSize + ramSize + savestateSize + metadataSize;

    // Assume a compression ratio of at least 2 and make sure that the buffer will actually grow
    const size_t initialBufferSize = min(static_cast<size_t>(1024), uncompressedSize / 2);

    serializationBuffer = make_unique<uint8[]>(initialBufferSize + HEADER_SIZE);
    uint8* serializationBufferPtr = serializationBuffer.get();

    put32(serializationBufferPtr, MAGIC);
    put32(serializationBufferPtr + 4, VERSION | VERSION_MASK);
    put32(serializationBufferPtr + 8, uncompressedSize);

    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    if (deflateInit(&stream, MZ_DEFAULT_COMPRESSION) != Z_OK) {
        return false;
    }

    stream.next_out = serializationBufferPtr + HEADER_SIZE;
    stream.avail_out = initialBufferSize;

    uint8 header[24];

    put32(header, deviceId.size());
    put32(header + 4, metadataSize);
    put32(header + 8, romSize);
    put32(header + 12, ramSize);
    put32(header + 16, framebufferSize);
    put32(header + 20, savestateSize);

    STREAM_OUT(header, 24);

    if (deviceId.size() > 0) {
        STREAM_OUT(deviceId.c_str(), deviceId.size());
    }

    if (metadataSize > 0) {
        STREAM_OUT(metadata, metadataSize);
    }

    if (romSize > 0) {
        STREAM_OUT(romImage, romSize);
    }

    if (ramSize > 0) {
        STREAM_OUT(ramImage, ramSize);
    }

    if (savestateSize > 0) {
        STREAM_OUT(savestate, savestateSize);
    }

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&stream);
        return false;
    }

    serizalizedImageSize = stream.total_out + HEADER_SIZE;

    deflateEnd(&stream);
    return true;

#undef STREAM_OUT
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
