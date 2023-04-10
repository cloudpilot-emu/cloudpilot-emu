#include "GzipContext.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "CPCrc.h"

using namespace std;

namespace {
    constexpr size_t MINIMUM_GZIP_BUFFER_SIZE = 512 * 1024;
    constexpr size_t MAXIMUM_GROW_FACTOR = 5;
    constexpr size_t MAX_FILENAME_LENGTH = 256;
    constexpr size_t MAX_GZIP_SIZE = 512 * 1024 * 1024;

    constexpr int COMPRESSION_LEVEL = 1;
}  // namespace

GzipContext::GzipContext(const void* uncompressedData, size_t uncompressedSize, size_t sliceSize)
    : sliceSize(sliceSize),
      uncompressedSize(uncompressedSize),
      uncompressedData(static_cast<const uint8_t*>(uncompressedData)),
      gzipBufferSize(min(max(uncompressedSize >> 2, MINIMUM_GZIP_BUFFER_SIZE), MAX_GZIP_SIZE)) {
    memset(&zipStream, 0, sizeof(zipStream));

    if (uncompressedSize == 0) {
        SetError("empty input");
        return;
    }

    gzipData = static_cast<uint8_t*>(malloc(gzipBufferSize));
    if (!gzipData) {
        SetError("malloc failed");
        return;
    }

    if (mz_deflateInit2(&zipStream, COMPRESSION_LEVEL, MZ_DEFLATED, -MZ_DEFAULT_WINDOW_BITS,
                        COMPRESSION_LEVEL, MZ_DEFAULT_STRATEGY) != MZ_OK) {
        SetError("failed to initialize deflate stream");
        return;
    }

    zipStream.next_in = this->uncompressedData;
}

GzipContext::~GzipContext() {
    if (gzipData) free(gzipData);
    mz_deflateEnd(&zipStream);
}

GzipContext& GzipContext::SetFilename(const char* filename) {
    if (state == State::initial && strlen(filename) <= MAX_FILENAME_LENGTH)
        this->filename = filename;

    return *this;
}

GzipContext& GzipContext::SetMtime(uint32_t mtime) {
    if (state == State::initial) this->mtime = mtime;

    return *this;
}

int GzipContext::Continue() {
    if (state == State::initial) {
        state = State::more;
        WriteHeader();

        return static_cast<int>(state);
    }

    if (state != State::more) return static_cast<int>(state);

    if (zipStreamEnd) {
        state = State::done;
        WriteFooter();

        return static_cast<int>(state);
    }

    zipStream.next_out = gzipData + gzipIndex;
    zipStream.avail_out = gzipBufferSize - gzipIndex;

    const ssize_t remainingInput = uncompressedSize - (zipStream.next_in - uncompressedData);
    int flush;

    if (remainingInput < 0) {
        SetError("internal: invalid input stream");
        return static_cast<int>(state);
    }

    if (static_cast<size_t>(remainingInput) <= sliceSize) {
        zipStream.avail_in = remainingInput;
        flush = MZ_FINISH;
    } else {
        zipStream.avail_in = sliceSize;
        flush = MZ_NO_FLUSH;
    }

    const int deflateResult = deflate(&zipStream, flush);
    gzipIndex = zipStream.next_out - gzipData;

    switch (deflateResult) {
        case MZ_BUF_ERROR:
            Grow();
            break;

        case MZ_STREAM_END:
            zipStreamEnd = true;
            break;

        case MZ_OK:
            break;

        default:
            SetError("deflate failed");
            break;
    }

    return static_cast<int>(state);
}

int GzipContext::GetState() const { return static_cast<int>(state); }

uint8_t* GzipContext::GetGzipData() { return gzipData; }

size_t GzipContext::GetGzipSize() const { return gzipIndex; }

const char* GzipContext::GetError() { return error.c_str(); }

void GzipContext::SetError(const std::string& error) {
    if (state == State::error) return;

    this->error = error;
    state = State::error;
}

void GzipContext::Grow() {
    const size_t growStep = max(uncompressedSize >> 2, MINIMUM_GZIP_BUFFER_SIZE);

    if (gzipBufferSize >= min(MAXIMUM_GROW_FACTOR * growStep, MAX_GZIP_SIZE)) {
        SetError("failed to grow buffer; limit reached");
        return;
    }

    gzipBufferSize = (gzipBufferSize / growStep + 1) * growStep;
    uint8_t* gzipDataNew = static_cast<uint8_t*>(realloc(gzipData, gzipBufferSize));
    if (!gzipDataNew) {
        SetError("ralloc failed");
        return;
    }

    gzipData = gzipDataNew;
}

void GzipContext::Write8(uint8_t value) {
    if (state == State::error) return;
    if (gzipIndex >= gzipBufferSize) Grow();
    if (gzipIndex >= gzipBufferSize) {
        SetError("not enough space left");
        return;
    }

    gzipData[gzipIndex++] = value;
}

void GzipContext::Write32(uint32_t value) {
    if (state == State::error) return;
    if (gzipIndex + 3 >= gzipBufferSize) Grow();
    if (gzipIndex + 3 >= gzipBufferSize) {
        SetError("not enough space left");
        return;
    }

    gzipData[gzipIndex++] = value;
    gzipData[gzipIndex++] = value >> 8;
    gzipData[gzipIndex++] = value >> 16;
    gzipData[gzipIndex++] = value >> 24;
}

void GzipContext::WriteHeader() {
    if (state == State::error) return;

    Write8(0x1f);                                 // MAGIC 0
    Write8(0x8b);                                 // MAGIC 1
    Write8(0x08);                                 // CM
    Write8(filename.length() > 0 ? 0x08 : 0x00);  // FLG = FNAME
    Write32(mtime);                               // MTIME
    Write8(0x04);                                 // XFL = fastest compression
    Write8(0x03);                                 // OS = UNIX

    if (filename.length() > 0) {
        for (size_t i = 0; i < filename.length(); i++) gzipData[gzipIndex + i] = filename[i];

        gzipData[gzipIndex + filename.length()] = 0;
        gzipIndex += filename.length() + 1;
    }
}

void GzipContext::WriteFooter() {
    if (state == State::error) return;

    Write32(crc::CRC32(uncompressedData, uncompressedSize));
    Write32(uncompressedSize);
}
