#include <gtest/gtest.h>

#include "Chunk.h"

TEST(SavestateChunk, DeSerializationU8) {
    uint8 buffer[4];
    Chunk chunk(4, buffer);

    chunk.Put8(66);
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(chunk.Get8(), static_cast<uint8>(66));
    ASSERT_FALSE(chunk.HasError());
}

TEST(SavestateChunk, DeSerializationS8) {
    uint8 buffer[4];
    Chunk chunk(4, buffer);

    chunk.Put8(static_cast<int8>(-66));
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(static_cast<int8>(chunk.Get8()), static_cast<int8>(-66));
    ASSERT_FALSE(chunk.HasError());
}

TEST(SavestateChunk, DeSerializationU16) {
    uint8 buffer[4];
    Chunk chunk(4, buffer);

    chunk.Put16(0xfa4e);
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(chunk.Get16(), static_cast<uint16>(0xfa4e));
    ASSERT_FALSE(chunk.HasError());
}

TEST(SavestateChunk, DeSerializationS16) {
    uint8 buffer[4];
    Chunk chunk(4, buffer);

    chunk.Put16(static_cast<int16>(-0x0fae));
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(static_cast<int16>(chunk.Get16()), static_cast<int16>(-0x0fae));
    ASSERT_FALSE(chunk.HasError());
}

TEST(SavestateChunk, DeSerializationU32) {
    uint8 buffer[4];
    Chunk chunk(4, buffer);

    chunk.Put32(0x0fab1234);
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(chunk.Get32(), static_cast<uint32>(0x0fab1234));
    ASSERT_FALSE(chunk.HasError());
}

TEST(SavestateChunk, DeSerializationS32) {
    uint8 buffer[4];
    Chunk chunk(4, buffer);

    chunk.Put32(static_cast<int32>(-0x0fab1234));
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(static_cast<int32>(chunk.Get32()), static_cast<int32>(-0x0fab1234));
    ASSERT_FALSE(chunk.HasError());
}

TEST(SavestateChunk, DeSerializationU64) {
    uint8 buffer[8];
    Chunk chunk(8, buffer);

    chunk.Put64(0x0fab12340fab1234);
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(chunk.Get64(), static_cast<uint64>(0x0fab12340fab1234));
    ASSERT_FALSE(chunk.HasError());
}

TEST(SavestateChunk, DeSerializationS64) {
    uint8 buffer[8];
    Chunk chunk(8, buffer);

    chunk.Put64(static_cast<int64>(-0x0fab12340fab1234));
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(static_cast<int64>(chunk.Get64()), static_cast<int64>(-0x0fab12340fab1234));
    ASSERT_FALSE(chunk.HasError());
}

TEST(SavestateChunk, DeSerializationBool) {
    uint8 buffer[4];
    Chunk chunk(4, buffer);

    chunk.PutBool(false);
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_FALSE(chunk.GetBool());
}

TEST(SavestateChunk, DeSerializationDouble) {
    uint8 buffer[8];
    Chunk chunk(8, buffer);

    chunk.PutDouble(1.25);
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    ASSERT_EQ(chunk.GetDouble(), 1.25);
}

TEST(SavestateChunk, DeSerializationBuffer) {
    uint8 buffer[12];
    Chunk chunk(12, buffer);
    const char* fixture = "12345abcde";

    chunk.PutBuffer(const_cast<char*>(fixture), 11);
    ASSERT_FALSE(chunk.HasError());

    chunk.Reset();

    char retrieved[11];
    chunk.GetBuffer(retrieved, 11);
    ASSERT_STREQ(retrieved, fixture);
}

TEST(SavestateChunk, ItDeSerializesMutlipleValuesCorrectly) {
    uint8 buffer[16];
    Chunk chunk(16, buffer);

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
}

TEST(SavestateChunk, ItErrorsIfTheBufferOverflows) {
    uint8 buffer[8];
    Chunk chunk(8, buffer);

    chunk.Put8(1);
    chunk.Put8(2);
    chunk.Put8(3);

    ASSERT_TRUE(chunk.HasError());
}
