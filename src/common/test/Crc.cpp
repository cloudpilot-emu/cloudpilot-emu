#include <gtest/gtest.h>

#include "CPCrc.h"

namespace {
    TEST(SDCRC7, itCalculatesSdCRC7) {
        const uint8_t fixture[] = {17, 0, 0, 9, 0};

        ASSERT_EQ(crc::sdCRC7(fixture, sizeof(fixture)), 51);
    }

    TEST(SDCRC16, itCalculatesSDCRC16) {
        uint8_t fixture[512];
        memset(fixture, 0xff, 512);

        ASSERT_EQ(crc::sdCRC16(fixture, 512), 0x7fa1);
    }

    TEST(CRC32, itCalculatesCRC32) {
        const char* fixture = "123456789";

        ASSERT_EQ(crc::CRC32(reinterpret_cast<const uint8_t*>(fixture), 9), 0xcbf43926);
    }
}  // namespace
