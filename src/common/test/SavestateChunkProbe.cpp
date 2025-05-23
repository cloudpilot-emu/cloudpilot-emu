#include <gtest/gtest.h>

#include <cstddef>

#include "savestate/ChunkProbe.h"

namespace {

    TEST(SavestateChunkProbe, U8IsFourBytes) {
        ChunkProbe chunk;
        chunk.Put8(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(1));
    }

    TEST(SavestateChunkProbe, U16IsFourBytes) {
        ChunkProbe chunk;
        chunk.Put16(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(1));
    }

    TEST(SavestateChunkProbe, U32IsFourBytes) {
        ChunkProbe chunk;
        chunk.Put32(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(1));
    }

    TEST(SavestateChunkProbe, U64IsEightBytes) {
        ChunkProbe chunk;
        chunk.Put64(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(2));
    }

    TEST(SavestateChunkProbe, BoolIsFourBytes) {
        ChunkProbe chunk;
        chunk.PutBool(false);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(1));
    }

    TEST(SavestateChunkProbe, DoubleIsEightBytes) {
        ChunkProbe chunk;
        chunk.PutDouble(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(2));
    }

    TEST(SavestateChunkProbe, BufferPads) {
        ChunkProbe chunk;

        chunk.PutBuffer(nullptr, 8);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(2));

        chunk.PutBuffer(nullptr, 9);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(5));

        chunk.PutBuffer(nullptr, 10);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(8));

        chunk.PutBuffer(nullptr, 11);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(11));

        chunk.PutBuffer(nullptr, 12);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(14));

        chunk.PutBuffer(nullptr, 13);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(18));
    }

    TEST(SavestateChunkProbe, Buffer16Pads) {
        ChunkProbe chunk;

        chunk.PutBuffer16(nullptr, 1);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(1));

        chunk.PutBuffer16(nullptr, 2);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(2));

        chunk.PutBuffer16(nullptr, 3);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(4));

        chunk.PutBuffer16(nullptr, 4);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(6));

        chunk.PutBuffer16(nullptr, 5);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(9));
    }

    TEST(SavestateChunkProbe, Buffer32IsFourBytes) {
        ChunkProbe chunk;

        chunk.PutBuffer32(nullptr, 1);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(1));

        chunk.PutBuffer32(nullptr, 2);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(3));

        chunk.PutBuffer32(nullptr, 3);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(6));

        chunk.PutBuffer32(nullptr, 4);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(10));
    }

    TEST(SavestateChunkProbe, Buffer64IsEightBytes) {
        ChunkProbe chunk;

        chunk.PutBuffer64(nullptr, 1);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(2));

        chunk.PutBuffer64(nullptr, 2);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(6));

        chunk.PutBuffer64(nullptr, 3);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(12));

        chunk.PutBuffer64(nullptr, 4);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(20));
    }

    TEST(SavestateChunkProbe, StringIsBufferWithMaxlengthPlusOne) {
        ChunkProbe chunk;

        chunk.PutString("abc", 15);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(4));

        chunk.PutString("abce", 15);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(8));

        chunk.PutString("abce", 16);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(13));
    }

}  // namespace
