#include "SessionFile.h"

#include <algorithm>
#include <cstring>
#include <iostream>

#include "CPCrc.h"
#include "Defer.h"
#include "rle.h"
#include "rom_info.h"
#include "zip/miniz.h"

using namespace std;

namespace {
    constexpr uint32_t MAGIC = 0x19800819;
    constexpr uint32_t CURRENT_VERSION = 1;

    constexpr size_t SIZE_HEADER = 12;  // 4 byte magic + 4 byte version + 4 byte device ID
    constexpr size_t SIZE_TOC = 5 * 4;

    constexpr size_t BUFFER_MAX_SIZE = 128 * 1024 * 1024;
    constexpr size_t BUFFER_MIN_SIZE = 1024;
}  // namespace

uint32_t SessionFile::GetDeviceId() const { return deviceId; }

SessionFile& SessionFile::SetDeviceId(uint32_t deviceId) {
    this->deviceId = deviceId;

    return *this;
}

const uint8_t* SessionFile::GetMetadata() const { return metadata; }

size_t SessionFile::GetMetadataSize() const { return metadataSize; }

SessionFile& SessionFile::SetMetadata(size_t size, const uint8_t* data) {
    metadataSize = size;
    metadata = data;

    return *this;
}

const uint8_t* SessionFile::GetNor() const { return nor; }

size_t SessionFile::GetNorSize() const { return norSize; }

SessionFile& SessionFile::SetNor(size_t size, const uint8_t* data) {
    norSize = size;
    nor = data;

    return *this;
}

const uint8_t* SessionFile::GetNand() const { return nand; }

size_t SessionFile::GetNandSize() const { return nandSize; }

SessionFile& SessionFile::SetNand(size_t size, const uint8_t* data) {
    nandSize = size;
    nand = data;

    return *this;
}

const uint8_t* SessionFile::GetRam() const { return ram; }

size_t SessionFile::GetRamSize() const { return ramSize; }

SessionFile& SessionFile::SetRam(size_t size, const uint8_t* data) {
    ramSize = size;
    ram = data;

    return *this;
}

const uint8_t* SessionFile::GetSavestate() const { return savestate; }

size_t SessionFile::GetSavestateSize() const { return savestateSize; }

SessionFile& SessionFile::SetSavestate(size_t size, const uint8_t* data) {
    savestateSize = size;
    savestate = data;

    return *this;
}

bool SessionFile::Serialize() {
    serializedSessionSize = 0;
    serializedSession = nullptr;

    const size_t sizeUncompressed = metadataSize + norSize + nandSize + ramSize + savestateSize;

    bufferSize =
        max(static_cast<size_t>(BUFFER_MIN_SIZE), SIZE_HEADER + SIZE_TOC + sizeUncompressed / 2);
    buffer = make_unique<uint8_t[]>(bufferSize);
    cursor = buffer.get();

    bool success = true;

    success &= Write32(MAGIC);
    success &= Write32(CURRENT_VERSION);
    success &= Write32(deviceId);

    success &= Write32(metadataSize);
    success &= Write32(norSize);
    success &= Write32(nandSize);
    success &= Write32(ramSize);
    success &= Write32(savestateSize);

    if (!success) {
        cerr << "failed to write header" << endl;
        return false;
    }

    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return false;
    }

    Defer cleanupDeflate([&]() { deflateEnd(&stream); });

    stream.next_out = cursor;
    stream.avail_out = bufferSize - (cursor - buffer.get());

    if (!AppendToCompressionStream(metadataSize, metadata, stream)) {
        cerr << "failed to write metadata" << endl;
        return false;
    }

    if (!AppendToCompressionStream(norSize, nor, stream)) {
        cerr << "failed to write NOR" << endl;
        return false;
    }

    if (!AppendToCompressionStream(nandSize, nand, stream)) {
        cerr << "failed to write NAND" << endl;
        return false;
    }

    if (!AppendToCompressionStream(ramSize, ram, stream)) {
        cerr << "failed to write RAM" << endl;
        return false;
    }

    if (!AppendToCompressionStream(savestateSize, savestate, stream)) {
        cerr << "failed to write savestate" << endl;
        return false;
    }

    if (!Flush(stream)) {
        cerr << "failed to flush" << endl;
        return false;
    }

    serializedSession = buffer.get();
    serializedSessionSize = cursor - buffer.get();

    return true;
}

const uint8_t* SessionFile::GetSerializedSession() const { return serializedSession; }

size_t SessionFile::GetSerializedSessionSize() const { return serializedSessionSize; }

bool SessionFile::Deserialize(size_t size, const uint8_t* data) {
    metadataSize = norSize = nandSize = ramSize = savestateSize = 0;
    metadata = nor = nand = ram = savestate = nullptr;

    serializedSession = data;
    serializedSessionSize = size;

    ccursor = serializedSession;

    bool success = true;

    const uint32_t magic = Read32(success);
    const uint32_t version = Read32(success);

    if (!success) {
        cerr << "unable to read header" << endl;
        return false;
    }

    if (magic != MAGIC) {
        cerr << "bad magic" << endl;
    }

    switch (version) {
        case 0:
            return Deserialize_v0();

        case 1:
            return Deserialize_v1();

        default:
            cerr << "unsupported session version " << version << endl;
            return false;
    }
}

bool SessionFile::Write32(uint32_t data) {
    if (!buffer || cursor - buffer.get() + 4 > static_cast<ssize_t>(bufferSize)) return false;

    *(cursor++) = data;
    *(cursor++) = (data >> 8);
    *(cursor++) = (data >> 16);
    *(cursor++) = (data >> 24);

    return true;
}

uint32_t SessionFile::Read32(bool& success) {
    if (!serializedSession ||
        ccursor - serializedSession + 4 > static_cast<ssize_t>(serializedSessionSize)) {
        success = false;
        return 0;
    }

    uint32_t result = *(ccursor++);
    result |= (*(ccursor++) << 8);
    result |= (*(ccursor++) << 16);
    result |= (*(ccursor++) << 24);

    return result;
}

bool SessionFile::AppendToCompressionStream(size_t size, const uint8_t* data, mz_stream& stream) {
    if (size == 0) return true;

    stream.next_in = reinterpret_cast<const unsigned char*>(data);
    stream.avail_in = size;

    int deflateResult;
    do {
        const auto outBytesBefore = stream.total_out;
        deflateResult = deflate(&stream, Z_NO_FLUSH);
        cursor += stream.total_out - outBytesBefore;

        if (deflateResult == Z_BUF_ERROR && GrowBuffer(stream)) deflateResult = Z_OK;
    } while (stream.avail_in != 0 && deflateResult == Z_OK);

    return deflateResult == Z_OK;
}

bool SessionFile::GrowBuffer(mz_stream& stream) {
    if (bufferSize == BUFFER_MAX_SIZE) return false;

    size_t newBufferSize = min((bufferSize * 3) / 2, BUFFER_MAX_SIZE);
    unique_ptr<uint8_t[]> newBuffer = make_unique<uint8_t[]>(newBufferSize);

    memcpy(newBuffer.get(), buffer.get(), cursor - buffer.get());
    cursor = newBuffer.get() + (cursor - buffer.get());
    bufferSize = newBufferSize;

    buffer.swap(newBuffer);

    stream.next_out = cursor;
    stream.avail_out = bufferSize - (cursor - buffer.get());

    return true;
}

bool SessionFile::Flush(mz_stream& stream) {
    int deflateResult;

    do {
        const auto outBytesBefore = stream.total_out;
        deflateResult = deflate(&stream, Z_FINISH);
        cursor += stream.total_out - outBytesBefore;
    } while ((deflateResult == Z_BUF_ERROR || deflateResult == Z_OK) && GrowBuffer(stream));

    return deflateResult == Z_STREAM_END;
}

bool SessionFile::Deserialize_v0() {
    bool success = true;

    metadataSize = Read32(success);

    const uint32_t sizeNorCompressed = Read32(success);
    const uint32_t crcNor = Read32(success);

    const uint32_t sizeRamCompressed = Read32(success);
    const uint32_t crcRam = Read32(success);

    const uint32_t sizeNandCompressed = Read32(success);
    const uint32_t crcNand = Read32(success);

    if (!success) {
        cerr << "unable to read v0 toc" << endl;
        return false;
    }

    if (ccursor - serializedSession + metadataSize + sizeNorCompressed + sizeRamCompressed +
            sizeNandCompressed !=
        serializedSessionSize) {
        cerr << "session image v0 size mismatch" << endl;
        return false;
    }

    const uint8_t* cc = ccursor + metadataSize;
    size_t remaining = serializedSessionSize - (cc - serializedSession);

    norSize = rle_get_chunk_size(remaining, cc, success);
    remaining -= sizeNorCompressed;
    cc += sizeNorCompressed;

    if (!success) {
        cerr << "bad v0 nor chunk: " << rle_getLastError() << endl;
        return false;
    }

    ramSize = rle_get_chunk_size(remaining, cc, success);
    remaining -= sizeRamCompressed;
    cc += sizeRamCompressed;

    if (!success) {
        cerr << "bad v0 ram chunk: " << rle_getLastError() << endl;
        return false;
    }

    nandSize = rle_get_chunk_size(remaining, cc, success);

    if (!success) {
        cerr << "bad v0 nand chunk: " << rle_getLastError() << endl;
        return false;
    }

    bufferSize = metadataSize + norSize + nandSize + ramSize;
    if (bufferSize > BUFFER_MAX_SIZE) {
        cout << "v0 image: bad image size" << endl;
        return false;
    }

    buffer = make_unique<uint8_t[]>(bufferSize);
    cursor = buffer.get();

    memcpy(cursor, ccursor, metadataSize);
    metadata = cursor;
    cursor += metadataSize;
    ccursor += metadataSize;

    if (!rle_decode_chunk(sizeNorCompressed, ccursor, bufferSize - (cursor - buffer.get()),
                          cursor)) {
        cerr << "failed to decode v0 nor: " << rle_getLastError() << endl;
        return false;
    }

    nor = cursor;
    cursor += norSize;
    ccursor += sizeNorCompressed;

    if (!rle_decode_chunk(sizeRamCompressed, ccursor, bufferSize - (cursor - buffer.get()),
                          cursor)) {
        cerr << "failed to decode v0 ram: " << rle_getLastError() << endl;
        return false;
    }

    ram = cursor;
    cursor += ramSize;
    ccursor += sizeRamCompressed;

    if (!rle_decode_chunk(sizeNandCompressed, ccursor, bufferSize - (cursor - buffer.get()),
                          cursor)) {
        cerr << "failed to decode v0 nand: " << rle_getLastError() << endl;
        return false;
    }

    nand = cursor;

    if (crc::CRC32(nor, norSize) != crcNor) {
        cerr << "session image v0: nor crc mismatch" << endl;
        return false;
    }

    if (crc::CRC32(ram, ramSize) != crcRam) {
        cerr << "session image v0: ram crc mismatch" << endl;
        return false;
    }

    if (crc::CRC32(nand, nandSize) != crcNand) {
        cerr << "session image v0: nand crc mismatch" << endl;
        return false;
    }

    RomInfo info(nor, norSize);
    if (!info.IsValid()) {
        cerr << "session image v0: unable to determine device type" << endl;
        return false;
    }

    deviceId = info.GetDeviceType();

    return true;
}

bool SessionFile::Deserialize_v1() {
    bool success = true;

    deviceId = Read32(success);

    metadataSize = Read32(success);
    norSize = Read32(success);
    nandSize = Read32(success);
    ramSize = Read32(success);
    savestateSize = Read32(success);

    if (!success) {
        cerr << "failed to read v1 toc" << endl;
        return false;
    }

    bufferSize = metadataSize + norSize + nandSize + ramSize + savestateSize;
    if (bufferSize > BUFFER_MAX_SIZE) {
        cout << "v1 image: bad image size" << endl;
        return false;
    }

    buffer = make_unique<uint8_t[]>(bufferSize);

    mz_ulong destLen = bufferSize;

    const auto uncompressResult = uncompress(buffer.get(), &destLen, ccursor,
                                             serializedSessionSize - (ccursor - serializedSession));

    if (uncompressResult != Z_OK) {
        cerr << "failed to uncompress v1 image" << endl;
        return false;
    }

    if (destLen != bufferSize) {
        cerr << "v1 image: bad size" << endl;
        return false;
    }

    cursor = buffer.get();

    metadata = cursor;
    cursor += metadataSize;

    nor = cursor;
    cursor += norSize;

    nand = cursor;
    cursor += nandSize;

    ram = cursor;
    cursor += ramSize;

    savestate = cursor;

    return true;
}
bool SessionFile::IsSessionFile(size_t size, uint8_t* data) {
    if (size < 8) return false;

    const uint32_t magic = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
    if (magic != MAGIC) return false;

    const uint32_t version = data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] << 24);
    if (version > 1) return false;

    return true;
}
