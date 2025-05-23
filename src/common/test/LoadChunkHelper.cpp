#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>

#include "savestate/ChunkHelper.h"

using testing::Return;

namespace {
    class ChunkMock {
       public:
        MOCK_METHOD(uint8_t, Get8, (), ());
        MOCK_METHOD(uint16_t, Get16, (), ());
        MOCK_METHOD(uint32_t, Get32, (), ());
        MOCK_METHOD(uint64_t, Get64, (), ());
        MOCK_METHOD(double, GetDouble, (), ());
        MOCK_METHOD(bool, GetBool, (), ());
        MOCK_METHOD(void, GetBuffer, (void*, size_t), ());
        MOCK_METHOD(void, GetBuffer16, (uint16_t*, size_t), ());
        MOCK_METHOD(void, GetBuffer32, (uint32_t*, size_t), ());
        MOCK_METHOD(void, GetBuffer64, (uint64_t*, size_t), ());
    };

    class LoadChunkHelperTest : public ::testing::Test {
       protected:
        using helperT = LoadChunkHelper<ChunkMock>;

        ChunkMock mock;
        uint16_t fixture16{0};
        uint32_t fixture32{0};
        uint64_t fixture64{0};
        helperT helper{mock};
    };

    TEST_F(LoadChunkHelperTest, DoU8) {
        EXPECT_CALL(mock, Get8()).Times(1).WillOnce(Return(22));
        uint8_t x;

        helper.Do8(x);

        ASSERT_EQ(x, 22);
    }

    TEST_F(LoadChunkHelperTest, DoS8) {
        EXPECT_CALL(mock, Get8()).Times(1).WillOnce(Return(-22));
        int8_t x;

        helper.Do8(x);

        ASSERT_EQ(x, -22);
    }

    TEST_F(LoadChunkHelperTest, DoU16) {
        EXPECT_CALL(mock, Get16()).Times(1).WillOnce(Return(0x1234));
        uint16_t x;

        helper.Do16(x);

        ASSERT_EQ(x, 0x1234);
    }

    TEST_F(LoadChunkHelperTest, DoS16) {
        EXPECT_CALL(mock, Get16()).Times(1).WillOnce(Return(0xf0ff));
        int16_t x;

        helper.Do16(x);

        ASSERT_EQ(x, -0x0f01);
    }

    TEST_F(LoadChunkHelperTest, DoU32) {
        EXPECT_CALL(mock, Get32()).Times(1).WillOnce(Return(0x12345678));
        uint32_t x;

        helper.Do32(x);

        ASSERT_EQ(x, 0x12345678u);
    }

    TEST_F(LoadChunkHelperTest, DoS32) {
        EXPECT_CALL(mock, Get32()).Times(1).WillOnce(Return(0xf0ffffff));
        int32_t x;

        helper.Do32(x);

        ASSERT_EQ(x, -0x0f000001);
    }

    TEST_F(LoadChunkHelperTest, DoU64) {
        EXPECT_CALL(mock, Get64()).Times(1).WillOnce(Return(0x1234567890abcdef));
        uint64_t x;

        helper.Do64(x);

        ASSERT_EQ(x, 0x1234567890abcdefu);
    }

    TEST_F(LoadChunkHelperTest, DoS64) {
        EXPECT_CALL(mock, Get64()).Times(1).WillOnce(Return(0xf0ffffffffffffff));
        int64_t x;

        helper.Do64(x);

        ASSERT_EQ(x, -0x0f00000000000001);
    }

    TEST_F(LoadChunkHelperTest, DoBool) {
        EXPECT_CALL(mock, GetBool()).Times(1).WillOnce(Return(false));
        bool x;

        helper.DoBool(x);

        ASSERT_FALSE(x);
    }

    TEST_F(LoadChunkHelperTest, DoDouble) {
        EXPECT_CALL(mock, GetDouble()).Times(1).WillOnce(Return(1.234));
        double x;

        helper.DoDouble(x);

        ASSERT_EQ(x, 1.234);
    }

    TEST_F(LoadChunkHelperTest, DoBuffer) {
        EXPECT_CALL(mock, GetBuffer(&mock, 666)).Times(1);

        helper.DoBuffer(&mock, 666);
    }

    TEST_F(LoadChunkHelperTest, DoBuffer16) {
        EXPECT_CALL(mock, GetBuffer16(&fixture16, 666)).Times(1);

        helper.DoBuffer16(&fixture16, 666);
    }

    TEST_F(LoadChunkHelperTest, DoBuffer32) {
        EXPECT_CALL(mock, GetBuffer32(&fixture32, 666)).Times(1);

        helper.DoBuffer32(&fixture32, 666);
    }

    TEST_F(LoadChunkHelperTest, DoBuffer64) {
        EXPECT_CALL(mock, GetBuffer64(&fixture64, 666)).Times(1);

        helper.DoBuffer64(&fixture64, 666);
    }

    TEST_F(LoadChunkHelperTest, DoU8Pack) {
        EXPECT_CALL(mock, Get32()).Times(1).WillOnce(Return(0x12345678));
        uint8_t x1, x2, x3, x4;

        helper.Do(helperT::Pack8() << x1 << x2 << x3 << x4);

        ASSERT_EQ(x4, 0x78);
        ASSERT_EQ(x3, 0x56);
        ASSERT_EQ(x2, 0x34);
        ASSERT_EQ(x1, 0x12);
    }

    TEST_F(LoadChunkHelperTest, DoS8Pack) {
        EXPECT_CALL(mock, Get32()).Times(1).WillOnce(Return(0x12fff078));
        int8_t x1, x2, x3, x4;

        helper.Do(helperT::Pack8() << x1 << x2 << x3 << x4);

        ASSERT_EQ(x4, 0x78);
        ASSERT_EQ(x3, -16);
        ASSERT_EQ(x2, -1);
        ASSERT_EQ(x1, 0x12);
    }

    TEST_F(LoadChunkHelperTest, DoU16Pack) {
        EXPECT_CALL(mock, Get32()).Times(1).WillOnce(Return(0x12345678));
        uint16_t x1, x2;

        helper.Do(helperT::Pack16() << x1 << x2);

        ASSERT_EQ(x2, 0x5678);
        ASSERT_EQ(x1, 0x1234);
    }

    TEST_F(LoadChunkHelperTest, DoS16Pack) {
        EXPECT_CALL(mock, Get32()).Times(1).WillOnce(Return(0x1234f0ff));
        int16_t x1, x2;

        helper.Do(helperT::Pack16() << x1 << x2);

        ASSERT_EQ(x2, -0x0f01);
        ASSERT_EQ(x1, 0x1234);
    }

    TEST_F(LoadChunkHelperTest, DoBoolPack) {
        EXPECT_CALL(mock, Get32()).Times(1).WillOnce(Return(0b100110));

        bool b1, b2, b3, b4, b5, b6;
        helper.Do(helperT::BoolPack() << b1 << b2 << b3 << b4 << b5 << b6);
    }
}  // namespace
