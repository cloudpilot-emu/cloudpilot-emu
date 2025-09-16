#include "rle.h"

#include <cstdint>
#include <cstring>
#include <memory>

using namespace std;

namespace {
    constexpr size_t MAX_CHUNK_SIZE = 128 * 1024 * 1024;

    constexpr uint8_t BLOCK_TYPE_COPY = 0;
    constexpr uint8_t BLOCK_TYPE_REPEAT = 1;

    const char* lastError = nullptr;
}  // namespace

bool rle_decode_chunk(size_t sourceSize, const uint8_t* sourceData, size_t destSize,
                      uint8_t* destBuffer) {
    bool success = true;
    const size_t size = rle_get_chunk_size(sourceSize, sourceData, success);

    if (size > destSize) {
        lastError = "not enough space for chunk";
        return false;
    }

    if (!success) return false;

    size_t cursor = 0;
    size_t cursorCompressed = 4;

    while (cursor < size) {
        if (cursorCompressed >= sourceSize) {
            lastError = "not enough blocks";
            return false;
        }

        uint8_t blockType = sourceData[cursorCompressed++];

        size_t blockLen = 0;
        uint8_t shift = 0;

        while (true) {
            if (cursorCompressed >= sourceSize) {
                lastError = "unable to decode block size";
                return false;
            }

            if (shift > 28) {
                lastError = "invalid block size";
                return false;
            }

            blockLen |= ((sourceData[cursorCompressed] & 0x7f) << shift);
            shift += 7;

            if ((sourceData[cursorCompressed++] & 0x80) == 0x00) break;
        }

        if (cursor + blockLen > size) {
            lastError = "block too long";
            return false;
        }

        switch (blockType) {
            case BLOCK_TYPE_COPY:
                if (cursorCompressed + blockLen > sourceSize) {
                    lastError = "copy block cut short";
                    return false;
                }

                memcpy(destBuffer + cursor, sourceData + cursorCompressed, blockLen);
                cursorCompressed += blockLen;
                break;

            case BLOCK_TYPE_REPEAT:
                if (cursorCompressed >= sourceSize) {
                    lastError = "repeat block cut short";
                    return false;
                }

                memset(destBuffer + cursor, sourceData[cursorCompressed++], blockLen);

                break;
        }

        cursor += blockLen;
    }

    return true;
}

size_t rle_get_chunk_size(size_t sourceSize, const uint8_t* sourceData, bool& success) {
    if (sourceSize < 4) {
        lastError = "bad chunk size";
        success = false;

        return 0;
    }

    const size_t size =
        sourceData[0] | (sourceData[1] << 8) | (sourceData[2] << 16) | (sourceData[3] << 24);

    if (size > MAX_CHUNK_SIZE) {
        lastError = "chunk too large";
        success = false;

        return 0;
    }

    return size;
}

const char* rle_getLastError() { return lastError; }
