#include <gtest/gtest.h>

#include "ChunkProbe.h"

namespace {

    TEST(SavestateChunkProbe, U8IsFourBytes) {
        ChunkProbe chunk;
        chunk.Put8(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(4));
    }

    TEST(SavestateChunkProbe, U16IsFourBytes) {
        ChunkProbe chunk;
        chunk.Put16(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(4));
    }

    TEST(SavestateChunkProbe, U32IsFourBytes) {
        ChunkProbe chunk;
        chunk.Put32(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(4));
    }

    TEST(SavestateChunkProbe, U64IsEightBytes) {
        ChunkProbe chunk;
        chunk.Put64(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(8));
    }

    TEST(SavestateChunkProbe, BoolIsFourBytes) {
        ChunkProbe chunk;
        chunk.PutBool(false);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(4));
    }

    TEST(SavestateChunkProbe, DoubleIsEightBytes) {
        ChunkProbe chunk;
        chunk.PutDouble(0);

        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(8));
    }

    TEST(SavestateChunkProbe, BufferPads) {
        ChunkProbe chunk;

        chunk.PutBuffer(nullptr, 8);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(8));

        chunk.PutBuffer(nullptr, 9);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(20));

        chunk.PutBuffer(nullptr, 10);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(32));

        chunk.PutBuffer(nullptr, 11);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(44));

        chunk.PutBuffer(nullptr, 12);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(56));

        chunk.PutBuffer(nullptr, 13);
        ASSERT_EQ(chunk.GetSize(), static_cast<size_t>(72));
    }

}  // namespace
