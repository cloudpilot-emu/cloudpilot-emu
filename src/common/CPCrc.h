#ifndef _CRC_H_
#define _CRC_H_

#include <cstddef>
#include <cstdint>

namespace crc {
    uint8_t sdCRC7(const uint8_t* data, size_t size);
    uint16_t sdCRC16(const uint8_t* data, size_t size);
    uint32_t CRC32(const uint8_t* data, size_t size);
}  // namespace crc

#endif  // _CRC_H_
