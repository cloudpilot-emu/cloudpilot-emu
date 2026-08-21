#ifndef _NIBBLER_H_
#define _NIBBLER_H_

#include <cstddef>
#include <cstdint>

template <int bpp, bool wordswap = false, bool reverseOrder = false>
class Nibbler {
   public:
    Nibbler() = default;

    [[gnu::always_inline]] inline void reset(const uint8_t* row, int offset) {
        next = row + offset / nibblesPerByte;

        if constexpr (wordswap)
            current = *((uint8_t*)((long)(next++) ^ 1l));
        else
            current = *(next++);

        nextNibble = offset % nibblesPerByte;
        current <<= (bpp * nextNibble);
    }

    [[gnu::always_inline]] inline void skipBytes(const size_t bytes) { next += bytes; }

    [[gnu::always_inline]] inline uint8_t nibble() {
        if (nextNibble >= nibblesPerByte) {
            if constexpr (wordswap)
                current = *((uint8_t*)((long)(next++) ^ 1l));
            else
                current = *(next++);

            nextNibble = 0;
        }

        uint8_t value;
        if constexpr (reverseOrder) {
            value = current & (0xff >> (8 - bpp));
            current >>= bpp;
        } else {
            value = current >> (8 - bpp);
            current <<= bpp;
        }

        nextNibble++;

        return value;
    }

   private:
    uint8_t current;
    const uint8_t* next;
    int nextNibble;

    static constexpr int nibblesPerByte = 8 / bpp;
};

#endif  // _NIBBLER_H_
