// clang-format off
#include <gtest/gtest.h>
// clang-format on

#include "SavestateProbe.h"

namespace {

    TEST(SavestateProbe, ItMapsTheRequestedChunkCorrectly) {
        SavestateProbe probe;

        ChunkProbe* chunkCpu68K = probe.GetChunk(ChunkType::cpu68k);
        ASSERT_TRUE(chunkCpu68K);

        ChunkProbe* chunkRegsEZ = probe.GetChunk(ChunkType::regsEZ);
        ASSERT_TRUE(chunkRegsEZ);

        chunkCpu68K->Put32(0);
        chunkCpu68K->Put32(1);

        chunkRegsEZ->PutDouble(1.0);
        chunkRegsEZ->Put32(3);

        auto& map = probe.GetChunkMap();

        ASSERT_EQ(map.size(), 2u);
        ASSERT_TRUE(map.find(ChunkType::cpu68k) != map.end());
        ASSERT_TRUE(map.find(ChunkType::regsEZ) != map.end());

        ASSERT_EQ(map.at(ChunkType::cpu68k).GetSize(), 8u);
        ASSERT_EQ(map.at(ChunkType::regsEZ).GetSize(), 12u);
    }

}  // namespace
