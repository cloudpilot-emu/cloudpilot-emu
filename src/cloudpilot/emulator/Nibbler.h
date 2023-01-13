#ifndef _NIBBLER_H_
#define _NIBBLER_H_

#include <cstddef>

template <int bpp, bool wordswap = false>
class Nibbler {
   public:
    Nibbler() = default;

    [[gnu::always_inline]] inline void reset(const uint8* row, int offset) {
        next = row + offset / nibblesPerByte;

        if constexpr (wordswap)
            current = *((uint8*)((long)(next++) ^ 1l));
        else
            current = *(next++);

        nextNibble = offset % nibblesPerByte;
        current <<= (bpp * nextNibble);
    }

    [[gnu::always_inline]] inline void skipBytes(const size_t bytes) { next += bytes; }

    [[gnu::always_inline]] inline uint8 nibble() {
        if (nextNibble >= nibblesPerByte) {
            if constexpr (wordswap)
                current = *((uint8*)((long)(next++) ^ 1l));
            else
                current = *(next++);

            nextNibble = 0;
        }

        uint8 value = (current & (0xff << (8 - bpp))) >> (8 - bpp);
        current <<= bpp;
        nextNibble++;

        return value;
    }

   private:
    uint8 current;
    const uint8* next;
    int nextNibble;

    static constexpr int nibblesPerByte = 8 / bpp;
};

#endif  // _NIBBLER_H_
