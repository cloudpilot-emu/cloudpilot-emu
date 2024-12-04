#include "rle.h"

#include <cstdint>
#include <cstring>
#include <memory>

using namespace std;

namespace {
    constexpr size_t MAX_BLOCK_SIZE = 128 * 1024 * 1024;

    constexpr uint8_t BLOCK_TYPE_COPY = 0;
    constexpr uint8_t BLOCK_TYPE_REPEAT = 1;

    const char* lastError = nullptr;
}  // namespace

bool rle_decode(struct Buffer source, std::unique_ptr<uint8_t[]>& dataUncompressed,
                size_t& lenUncompressed) {
    if (source.size < 4) {
        lastError = "bad block size";
        return false;
    }

    auto data = reinterpret_cast<const uint8_t*>(source.data);
    const size_t size = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

    if (size > MAX_BLOCK_SIZE) {
        lastError = "block too large";
        return false;
    }

    size_t cursor = 0;
    size_t cursorCompressed = 4;
    auto decompressed = make_unique<uint8_t[]>(size);

    while (cursor < size) {
        if (cursorCompressed >= source.size) {
            lastError = "not enough blocks";
            return false;
        }

        uint8_t blockType = data[cursorCompressed++];

        size_t blockLen = 0;
        uint8_t shift = 0;

        while (true) {
            if (cursorCompressed >= source.size) {
                lastError = "unable to decode block size";
                return false;
            }

            if (shift > 28) {
                lastError = "invalid block size";
                return false;
            }

            blockLen |= ((data[cursorCompressed] & 0x7f) << shift);
            shift += 7;

            if ((data[cursorCompressed++] & 0x80) == 0x00) break;
        }

        if (cursor + blockLen > size) {
            lastError = "block too long";
            return false;
        }

        switch (blockType) {
            case BLOCK_TYPE_COPY:
                if (cursorCompressed + blockLen > source.size) {
                    lastError = "copy block cut short";
                    return false;
                }

                memcpy(decompressed.get() + cursor, data + cursorCompressed, blockLen);
                cursorCompressed += blockLen;
                break;

            case BLOCK_TYPE_REPEAT:
                if (cursorCompressed >= source.size) {
                    lastError = "repeat block cut short";
                    return false;
                }

                memset(decompressed.get() + cursor, data[cursorCompressed++], blockLen);

                break;
        }

        cursor += blockLen;
    }

    lenUncompressed = size;
    dataUncompressed = std::move(decompressed);

    return true;
}

const char* rle_getLastError() { return lastError; }
