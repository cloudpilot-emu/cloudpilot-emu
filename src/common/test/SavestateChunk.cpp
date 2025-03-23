#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>

#include "savestate/Chunk.h"

namespace {

    TEST(SavestateChunk, DeSerializationU8) {
        uint32_t buffer[1];
        Chunk chunk(1, buffer);

        chunk.Put8(66);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(chunk.Get8(), static_cast<uint8_t>(66));
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationS8) {
        uint32_t buffer[1];
        Chunk chunk(1, buffer);

        chunk.Put8(static_cast<int8_t>(-66));
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(static_cast<int8_t>(chunk.Get8()), static_cast<int8_t>(-66));
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationU16) {
        uint32_t buffer[1];
        Chunk chunk(1, buffer);

        chunk.Put16(0xfa4e);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(chunk.Get16(), static_cast<uint16_t>(0xfa4e));
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationS16) {
        uint32_t buffer[1];
        Chunk chunk(1, buffer);

        chunk.Put16(static_cast<int16_t>(-0x0fae));
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(static_cast<int16_t>(chunk.Get16()), static_cast<int16_t>(-0x0fae));
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationU32) {
        uint32_t buffer[1];
        Chunk chunk(1, buffer);

        chunk.Put32(0x0fab1234);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(chunk.Get32(), static_cast<uint32_t>(0x0fab1234));
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationS32) {
        uint32_t buffer[1];
        Chunk chunk(1, buffer);

        chunk.Put32(static_cast<int32_t>(-0x0fab1234));
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(static_cast<int32_t>(chunk.Get32()), static_cast<int32_t>(-0x0fab1234));
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationU64) {
        uint32_t buffer[2];
        Chunk chunk(2, buffer);

        chunk.Put64(0x0fab12340fab1234);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(chunk.Get64(), static_cast<uint64_t>(0x0fab12340fab1234));
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationS64) {
        uint32_t buffer[2];
        Chunk chunk(2, buffer);

        chunk.Put64(static_cast<int64_t>(-0x0fab12340fab1234));
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(static_cast<int64_t>(chunk.Get64()), static_cast<int64_t>(-0x0fab12340fab1234));
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationBool) {
        uint32_t buffer[2];
        Chunk chunk(2, buffer);

        chunk.PutBool(false);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_FALSE(chunk.GetBool());
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationDouble) {
        uint32_t buffer[2];
        Chunk chunk(2, buffer);

        chunk.PutDouble(1.25);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(chunk.GetDouble(), 1.25);
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationBuffer) {
        uint32_t buffer[3];
        Chunk chunk(3, buffer);
        const char* fixture = "12345abcde";

        chunk.PutBuffer(const_cast<char*>(fixture), 11);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        char retrieved[11];
        chunk.GetBuffer(retrieved, 11);
        ASSERT_STREQ(retrieved, fixture);
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, ItDeSerializesMutlipleValuesCorrectly) {
        uint32_t buffer[4];
        Chunk chunk(4, buffer);

        chunk.Put8(1);
        chunk.Put8(2);
        chunk.Put8(3);
        chunk.Put8(4);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(chunk.Get8(), 1);
        ASSERT_EQ(chunk.Get8(), 2);
        ASSERT_EQ(chunk.Get8(), 3);
        ASSERT_EQ(chunk.Get8(), 4);
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, DeSerializationString) {
        uint32_t buffer[4];
        Chunk chunk(4, buffer);

        chunk.PutString("Hulpe", 15);
        ASSERT_FALSE(chunk.HasError());

        chunk.Reset();

        ASSERT_EQ(chunk.GetString(15), "Hulpe");
        ASSERT_FALSE(chunk.HasError());
    }

    TEST(SavestateChunk, ItPadsStringToMaxLengthAndAlignment) {
        uint32_t buffer[8];
        memset(buffer, 0, 32);
        Chunk chunk(8, buffer);

        chunk.PutString("Hulpe", 15);
        chunk.Put32(0x12345678);

        chunk.Reset();

        ASSERT_EQ(*(buffer + 4), 0x12345678u);
        ASSERT_EQ(chunk.GetString(15), "Hulpe");
        ASSERT_EQ(chunk.Get32(), 0x12345678u);
    }

    TEST(SavestateChunk, ItErrorsIfStringExceedsMaxLength) {
        uint32_t buffer[4];
        Chunk chunk(4, buffer);

        chunk.PutString("Hulpe", 4);
        ASSERT_TRUE(chunk.HasError());
    }

    TEST(SavestateChunk, ItErrorsIfTheBufferOverflows) {
        uint32_t buffer[2];
        Chunk chunk(2, buffer);

        chunk.Put8(1);
        chunk.Put8(2);
        chunk.Put8(3);

        ASSERT_TRUE(chunk.HasError());
    }

    namespace AddsPaddingForBuffer {
        struct Params {
            const char* fixture;
            size_t paddingSize;
        };

        class AddsPaddingForBuffer : public testing::TestWithParam<Params> {};

        TEST_P(AddsPaddingForBuffer, AddsPaddingAndDeserializesCorrectly) {
            uint32_t buffer[5];
            Chunk chunk(5, buffer);
            memset(buffer, 0, 20);

            Params params = GetParam();
            size_t fixtureLen = strlen(params.fixture) + 1;

            chunk.PutBuffer(const_cast<char*>(params.fixture), fixtureLen);
            chunk.Put32(0x12345678);

            ASSERT_FALSE(chunk.HasError());
            ASSERT_EQ(*(buffer + params.paddingSize), 0x12345678u);

            chunk.Reset();

            char retrieved[20];
            chunk.GetBuffer(retrieved, fixtureLen);
            ASSERT_STREQ(retrieved, params.fixture);
            ASSERT_EQ(chunk.Get32(), 0x12345678u);
            ASSERT_FALSE(chunk.HasError());
        }

        INSTANTIATE_TEST_SUITE_P(ParameterRun, AddsPaddingForBuffer,
                                 testing::Values(Params{"1234567", 2}, Params{"12345678", 3},
                                                 Params{"123456789", 3}, Params{"123456789a", 3},
                                                 Params{"123456789ab", 3},
                                                 Params{"123456789abc", 4}));
    }  // namespace AddsPaddingForBuffer

}  // namespace
