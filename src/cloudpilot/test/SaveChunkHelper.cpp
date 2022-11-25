#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ChunkHelper.h"

namespace {
    class ChunkMock {
       public:
        MOCK_METHOD(void, Put8, (uint8), ());
        MOCK_METHOD(void, Put16, (uint16), ());
        MOCK_METHOD(void, Put32, (uint32), ());
        MOCK_METHOD(void, Put64, (uint64), ());
        MOCK_METHOD(void, PutDouble, (double), ());
        MOCK_METHOD(void, PutBool, (bool), ());
        MOCK_METHOD(void, PutBuffer, (void*, size_t), ());
    };

    class SaveChunkHelperTest : public ::testing::Test {
       protected:
        using helperT = SaveChunkHelper<ChunkMock>;

        ChunkMock mock;
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
