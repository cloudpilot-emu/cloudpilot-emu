#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>

#include "savestate/ChunkHelper.h"

namespace {
    class ChunkMock {
       public:
        MOCK_METHOD(void, Put8, (uint8_t), ());
        MOCK_METHOD(void, Put16, (uint16_t), ());
        MOCK_METHOD(void, Put32, (uint32_t), ());
        MOCK_METHOD(void, Put64, (uint64_t), ());
        MOCK_METHOD(void, PutDouble, (double), ());
        MOCK_METHOD(void, PutBool, (bool), ());
        MOCK_METHOD(void, PutBuffer, (void*, size_t), ());
        MOCK_METHOD(void, PutBuffer16, (uint16_t*, size_t), ());
        MOCK_METHOD(void, PutBuffer32, (uint32_t*, size_t), ());
        MOCK_METHOD(void, PutBuffer64, (uint64_t*, size_t), ());
    };

    class SaveChunkHelperTest : public ::testing::Test {
       protected:
        using helperT = SaveChunkHelper<ChunkMock>;

        ChunkMock mock;
        uint16_t fixture16{0};
        uint32_t fixture32{0};
        uint64_t fixture64{0};
        helperT helper{mock};
    };

    TEST_F(SaveChunkHelperTest, Do8) {
        EXPECT_CALL(mock, Put8(22)).Times(1);
        helper.Do8(22);
    }

    TEST_F(SaveChunkHelperTest, Do16) {
        EXPECT_CALL(mock, Put16(0x1234)).Times(1);
        helper.Do16(0x1234);
    }

    TEST_F(SaveChunkHelperTest, Do32) {
        EXPECT_CALL(mock, Put32(0x12345678)).Times(1);
        helper.Do32(0x12345678);
    }

    TEST_F(SaveChunkHelperTest, Do64) {
        EXPECT_CALL(mock, Put64(0x123456789abcdef1)).Times(1);
        helper.Do64(0x123456789abcdef1);
    }

    TEST_F(SaveChunkHelperTest, DoBool) {
        EXPECT_CALL(mock, PutBool(true)).Times(1);
        helper.DoBool(true);
    }

    TEST_F(SaveChunkHelperTest, DoDouble) {
        EXPECT_CALL(mock, PutDouble(3.14)).Times(1);
        helper.DoDouble(3.14);
    }

    TEST_F(SaveChunkHelperTest, DoBuffer) {
        EXPECT_CALL(mock, PutBuffer(&mock, 66)).Times(1);
        helper.DoBuffer(&mock, 66);
    }

    TEST_F(SaveChunkHelperTest, DoBuffer16) {
        EXPECT_CALL(mock, PutBuffer16(&fixture16, 66)).Times(1);
        helper.DoBuffer16(&fixture16, 66);
    }

    TEST_F(SaveChunkHelperTest, DoBuffer32) {
        EXPECT_CALL(mock, PutBuffer32(&fixture32, 66)).Times(1);
        helper.DoBuffer32(&fixture32, 66);
    }

    TEST_F(SaveChunkHelperTest, DoBuffer64) {
        EXPECT_CALL(mock, PutBuffer64(&fixture64, 66)).Times(1);
        helper.DoBuffer64(&fixture64, 66);
    }

    TEST_F(SaveChunkHelperTest, Do8Pack) {
        EXPECT_CALL(mock, Put32(0x12345678)).Times(1);
        helper.Do(helperT::Pack8() << 0x12 << 0x34 << 0x56 << 0x78);
    }

    TEST_F(SaveChunkHelperTest, Do16Pack) {
        EXPECT_CALL(mock, Put32(0x12345678)).Times(1);
        helper.Do(helperT::Pack16() << 0x1234 << 0x5678);
    }

    TEST_F(SaveChunkHelperTest, DoBoolPack) {
        EXPECT_CALL(mock, Put32(0b11010));
        helper.Do(helperT::BoolPack() << true << true << false << true << false);
    }
}  // namespace
