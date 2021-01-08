// clang-format off
#include <gtest/gtest.h>
// clang-format on

#include "Savestate.h"
#include "SavestateLoader.h"
#include "ChunkHelper.h"

namespace {
    struct MockCpu68k {
        uint32 x{0};
        int16 y{0};
        double z{0};

        template <typename T>
        void Save(T& savestate) {
            typename T::chunkT* chunk = savestate.GetChunk(ChunkType::cpu68k);
            if (!chunk) return;

            SaveChunkHelper helper(*chunk);
            DoSaveLoad(helper);
        }

        void Load(SavestateLoader& loader) {
            Chunk* chunk = loader.GetChunk(ChunkType::cpu68k);
            if (!chunk) return;

            LoadChunkHelper helper(*chunk);
            DoSaveLoad(helper);
        }

        template <typename T>
        void DoSaveLoad(T& helper) {
            helper.Do32(x).Do16(y).DoDouble(z);
        }

        bool operator==(const MockCpu68k other) const {
            return (x == other.x) && (y == other.y) && (z == other.z);
        }
    };

    struct MockRegsEZ {
        uint8 x{0};
        uint8 y{0};
        uint8 z{0};
        bool b{false};

        template <typename T>
        void Save(T& savestate) {
            typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsEZ);
            if (!chunk) return;

            SaveChunkHelper helper(*chunk);
            DoSaveLoad(helper);
        }

        void Load(SavestateLoader& loader) {
            Chunk* chunk = loader.GetChunk(ChunkType::regsEZ);
            if (!chunk) return;

            LoadChunkHelper helper(*chunk);
            DoSaveLoad<LoadChunkHelper>(helper);
        }

        template <typename T>
        void DoSaveLoad(T& helper) {
            uint8 padding = 0;

            helper.Do8(x, y, z, padding).DoBool(b);
        }

        bool operator==(const MockRegsEZ other) const {
            return (x == other.x) && (y == other.y) && (z == other.z) && (b == other.b);
        }
    };

    struct Root {
        MockCpu68k cpu68k;
        MockRegsEZ regsEZ;

        template <typename T>
        void Save(T& savestate) {
            cpu68k.Save(savestate);
            regsEZ.Save(savestate);
        }

        void Load(SavestateLoader& loader) {
            cpu68k.Load(loader);
            regsEZ.Load(loader);
        }
    };

    TEST(Savestate, DeSerializationTest) {
        MockRegsEZ regsEZ{1, 55, 66, true};
        MockCpu68k cpu68k{0x12345678, -17, 3.5};
        Root root{cpu68k, regsEZ};

        Savestate savestate;
        ASSERT_TRUE(savestate.Save(root));
        ASSERT_EQ(savestate.GetSize(), 44u);

        Root loadRoot;
        SavestateLoader loader;

        ASSERT_TRUE(loader.Load(savestate.GetBuffer(), savestate.GetSize(), loadRoot));
        ASSERT_EQ(loadRoot.cpu68k, cpu68k);
        ASSERT_EQ(loadRoot.regsEZ, regsEZ);
    }
}  // namespace
