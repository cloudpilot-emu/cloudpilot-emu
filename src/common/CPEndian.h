#ifndef _CPENDIAN_H_
#define _CPENDIAN_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __APPLE__

    #include <libkern/OSByteOrder.h>

    #define htobe16(x) OSSwapHostToBigInt16(x)
    #define htole16(x) OSSwapHostToLittleInt16(x)
    #define be16toh(x) OSSwapBigToHostInt16(x)
    #define le16toh(x) OSSwapLittleToHostInt16(x)

    #define htobe32(x) OSSwapHostToBigInt32(x)
    #define htole32(x) OSSwapHostToLittleInt32(x)
    #define be32toh(x) OSSwapBigToHostInt32(x)
    #define le32toh(x) OSSwapLittleToHostInt32(x)

    #define htobe64(x) OSSwapHostToBigInt64(x)
    #define htole64(x) OSSwapHostToLittleInt64(x)
    #define be64toh(x) OSSwapBigToHostInt64(x)
    #define le64toh(x) OSSwapLittleToHostInt64(x)

    #define __LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
    #define __BIG_ENDIAN __ORDER_BIG_ENDIAN__

    #ifdef __BIG_ENDIAN__
        #define __BYTE_ORDER __ORDER_BIG_ENDIAN__
    #else
        #define __BYTE_ORDER __ORDER_LITTLE_ENDIAN__
    #endif

#else
    #include <endian.h>
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define bswap32(x) __builtin_bswap32(x)
    #define bswap16(x) __builtin_bswap16(x)
#elif defined(_MSC_VER)
    #include <stdlib.h>
    #define bswap32(x) _byteswap_ulong(x)
    #define bswap16(x) _byteswap_ushort(x)
#else
static inline uint32_t bswap32(uint32_t x) {
    return (((x & 0xff) << 24) | ((x & 0xff00) << 8) | (((x & 0xff0000) >> 8)) | ((x >> 24)));
}

static inline uint16_t bswap16(uint16_t x) { return (x >> 8) | ((x & 0xff) << 8); }
#endif

#endif  // _CPENDTIAN_H_
