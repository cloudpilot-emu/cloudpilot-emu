// clang-format off
#include <gtest/gtest.h>
// clang-format on

#include "SavestateLoader.h"

#include "Savestate.h"

namespace {
    struct Mock {
        template <typename T>
        void Save(T& savestate) {
            typename T::chunkT* chunkCpu68k = savestate.GetChunk(ChunkType::cpu68k);
            typename T::chunkT* chunkRegsEZ = savestate.GetChunk(ChunkType::regsEZ);
            if (!chunkCpu68k || !chunkRegsEZ) return;

            chunkCpu68k->Put32(0);
            chunkRegsEZ->Put64(0);
        }

        void Load(SavestateLoader& loader) {}
    };

    class SavestateLoaderTest : public ::testing::Test {
       public:
        SavestateLoaderTest() { savestate.Save(mock); }

       protected:
        Mock mock;
        Savestate savestate;
        SavestateLoader loader;
    };

    TEST_F(SavestateLoaderTest, ItFailsIfBufferIsTooSmallForHeader) {
        ASSERT_FALSE(loader.Load(savestate.GetBuffer(), 3, mock));
    }

    TEST_F(SavestateLoaderTest, ItFailsIfBufferIsTooSmallForToc) {
        ASSERT_FALSE(loader.Load(savestate.GetBuffer(), 16, mock));
    }

    TEST_F(SavestateLoaderTest, ItFailsIfBufferIsTooSmallForChunk1) {
        ASSERT_FALSE(loader.Load(savestate.GetBuffer(), 22, mock));
    }

    TEST_F(SavestateLoaderTest, ItFailsIfBufferIsTooSmallForChunk2) {
        ASSERT_FALSE(loader.Load(savestate.GetBuffer(), 30, mock));
    }

    TEST_F(SavestateLoaderTest, ItFailsIfBufferIsTooLarge) {
        ASSERT_FALSE(loader.Load(savestate.GetBuffer(), 33, mock));
    }

    TEST_F(SavestateLoaderTest, ItSucceedsOtherwise) {
        ASSERT_TRUE(loader.Load(savestate.GetBuffer(), 32, mock));
    }
}  // namespace
