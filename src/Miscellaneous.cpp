#include "Miscellaneous.h"

uint32 NextPowerOf2(uint32 n) {
    // Smear down the upper 1 bit to all bits lower than it.

    uint32 n2 = n;

    n2 |= n2 >> 1;
    n2 |= n2 >> 2;
    n2 |= n2 >> 4;
    n2 |= n2 >> 8;
    n2 |= n2 >> 16;

    // Now use itself to clear all the lower bits.

    n2 &= ~(n2 >> 1);

    // If n2 ends up being the same as what we started with, keep it.
    // Otherwise, we need to bump it by a factor of two (round up).

    if (n2 != n) n2 <<= 1;

    return n2;
}
