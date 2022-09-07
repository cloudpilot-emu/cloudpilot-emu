#ifndef _EM_CRC_H_
#define _EM_CRC_H_

#include <EmCommon.h>

namespace crc {
    uint8 sdCRC7(const uint8* data, size_t size);
    uint16 sdCRC16(const uint8* data, size_t size);
}  // namespace crc

#endif  // _EM_CRC_H_
