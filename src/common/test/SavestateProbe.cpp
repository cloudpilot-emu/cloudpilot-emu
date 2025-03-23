// clang-format off
#include <gtest/gtest.h>
// clang-format on

#include "savestate/SavestateProbe.h"

#include "Logging.h"

namespace {
    enum class ChunkType { cpu68k, regsEZ };

    TEST(SavestateProbe, ItMapsTheRequestedChunkCorrectly) {
        SavestateProbe<ChunkType> probe;

        ChunkProbe* chunkCpu68K = probe.GetChunk(ChunkType::cpu68k);
        ASSERT_TRUE(chunkCpu68K);

        ChunkProbe* chunkRegsEZ = probe.GetChunk(ChunkType::regsEZ);
        ASSERT_TRUE(chunkRegsEZ);

        chunkCpu68K->Put32(0);
        chunkCpu68K->Put32(1);

        chunkRegsEZ->PutDouble(1.0);
        chunkRegsEZ->Put32(3);

        ASSERT_FALSE(probe.HasError());

        auto& map = probe.GetChunkMap();

        ASSERT_EQ(map.size(), 2u);
        ASSERT_TRUE(map.find(ChunkType::cpu68k) != map.end());
        ASSERT_TRUE(map.find(ChunkType::regsEZ) != map.end());

        ASSERT_EQ(map.at(ChunkType::cpu68k).GetSize(), 2u);
        ASSERT_EQ(map.at(ChunkType::regsEZ).GetSize(), 3u);
    }

    TEST(SavestateProbe, RequestingAChunkTwiceGeneratesAnError) {
        SavestateProbe<ChunkType> probe;

        ASSERT_TRUE(probe.GetChunk(ChunkType::cpu68k));
        ASSERT_FALSE(probe.HasError());

        ASSERT_FALSE(probe.GetChunk(ChunkType::cpu68k));
        ASSERT_TRUE(probe.HasError());
    }

    TEST(SavestateProbe, NotifyErrorGeneratesAnError) {
        SavestateProbe<ChunkType> probe;

        probe.NotifyError();

        ASSERT_TRUE(probe.HasError());
    }

}  // namespace
