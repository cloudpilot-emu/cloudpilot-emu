#include "GunzipContext.h"

#include "CPCrc.h"

using namespace std;

namespace {
    constexpr uint16_t MAGIC_GZ = 0x8b1f;

    constexpr uint8_t COMPRESSION_TYPE_DEFLATE = 8;

    constexpr size_t HEADER_SIZE = 10;
    constexpr size_t FOOTER_SIZE = 8;

    constexpr uint8_t FLAG_FHCRC = 2;
    constexpr uint8_t FLAG_FEXTRA = 4;
    constexpr uint8_t FLAG_FNAME = 8;
    constexpr uint8_t FLAG_FCOMMENT = 16;

    constexpr size_t DECOMPRESSED_SIZE_LIMIT = 512 * 1024 * 1024;
}  // namespace

GunzipContext::GunzipContext(const void* data, size_t size, size_t sliceSize)
    : compressedData(static_cast<const uint8_t*>(data)),
      compressedSize(size),
      sliceSize(sliceSize) {
    memset(&zipStream, 0, sizeof(zipStream));
    ReadHeaderFooter();

    if (state == State::error) return;
    if (headerFooter.magic != MAGIC_GZ) {
        SetError("bad magic");
        return;
    }

    if (headerFooter.compressionMethod != COMPRESSION_TYPE_DEFLATE) {
        SetError("invalid compression method");
        return;
    }

    if (headerFooter.uncompressedSize >= DECOMPRESSED_SIZE_LIMIT) {
        SetError("uncompressed size too large");
        return;
    }

    uncompressedSize = headerFooter.uncompressedSize;
    uncompressedData = make_unique<uint8_t[]>(uncompressedSize);
    memset(uncompressedData.get(), 0, uncompressedSize);

    payload = compressedData + payloadStart;
    payloadSize = size - payloadStart - FOOTER_SIZE;

    if (mz_inflateInit2(&zipStream, -MZ_DEFAULT_WINDOW_BITS) != MZ_OK) {
        SetError("failed to initialize deflate stream");
        return;
    }

    zipStream.next_in = payload;
    zipStream.next_out = uncompressedData.get();
    zipStream.avail_out = headerFooter.uncompressedSize;
}

GunzipContext::~GunzipContext() { mz_inflateEnd(&zipStream); }

int GunzipContext::GetState() const { return static_cast<int>(state); }

int GunzipContext::Continue() {
    if (state != State::more) return GetState();

    if (zipStreamEnd) {
        state = State::done;
        Validate();

        return GetState();
    }

    const ssize_t remainingInput = payloadSize - (zipStream.next_in - payload);
    int flush;

    if (remainingInput <= 0) {
        SetError("no more data to decompress");
        return GetState();
    }

    if (static_cast<size_t>(remainingInput) <= sliceSize) {
        zipStream.avail_in = remainingInput;
        flush = MZ_FINISH;
    } else {
        zipStream.avail_in = sliceSize;
        flush = MZ_NO_FLUSH;
    }

    switch (mz_inflate(&zipStream, flush)) {
        case MZ_OK:
            break;

        case MZ_STREAM_END:
            zipStreamEnd = true;
            break;

        default:
            SetError("inflate failed");
            break;
    }

    return GetState();
}

void* GunzipContext::GetUncompressedData() { return uncompressedData.get(); }

void* GunzipContext::ReleaseUncompressedData() { return uncompressedData.release(); }

size_t GunzipContext::GetUncompressedSize() { return uncompressedSize; }

const char* GunzipContext::GetError() { return errorDescription.c_str(); }

void GunzipContext::SetError(const std::string& description) {
    if (state == State::error) return;

    state = State::error;
    errorDescription = description;
}

uint32_t GunzipContext::Read32(size_t index) {
    if (index + 3 >= compressedSize) {
        SetError("attempt to read beyond input");
        return 0;
    }

    uint32_t result = compressedData[index++];
    result |= compressedData[index++] << 8;
    result |= compressedData[index++] << 16;
    result |= compressedData[index] << 24;

    return result;
}

uint16_t GunzipContext::Read16(size_t index) {
    if (index + 1 >= compressedSize) {
        SetError("attempt to read beyond input");
        return 0;
    }

    uint32_t result = compressedData[index++];
    result |= compressedData[index] << 8;

    return result;
}

uint8_t GunzipContext::Read8(size_t index) {
    if (index >= compressedSize) {
        SetError("attempt to read beyond input");
        return 0;
    }

    return compressedData[index];
}

void GunzipContext::ReadHeaderFooter() {
    if (uncompressedSize <= HEADER_SIZE + FOOTER_SIZE) return SetError("not enough input");

    headerFooter.magic = Read16(0);
    headerFooter.compressionMethod = Read8(2);
    headerFooter.flags = Read8(3);
    headerFooter.mtime = Read32(4);
    headerFooter.xflags = Read8(8);
    headerFooter.os = Read8(9);
    headerFooter.xlen = Read16(10);
    headerFooter.uncompressedSize = Read32(compressedSize - 4);
    headerFooter.crc = Read32(compressedSize - 8);

    payloadStart = HEADER_SIZE;

    if (state == State::error) return;

    if (headerFooter.flags & FLAG_FEXTRA) payloadStart += (2 + headerFooter.xlen);
    if (headerFooter.flags & FLAG_FNAME) SkipZeroTerminatedString();
    if (headerFooter.flags & FLAG_FCOMMENT) SkipZeroTerminatedString();
    if (headerFooter.flags & FLAG_FHCRC) payloadStart += 2;

    if (payloadStart + 8 >= compressedSize) SetError("input too short");
}

void GunzipContext::SkipZeroTerminatedString() {
    if (state == State::error) return;

    while (payloadStart < compressedSize && compressedData[payloadStart] != 0) payloadStart++;

    if (payloadStart < compressedSize) payloadStart++;
}

void GunzipContext::Validate() {
    const uint32_t crc = crc::CRC32(uncompressedData.get(), uncompressedSize);

    if (crc != headerFooter.crc) SetError("CRC mismatch");
}
