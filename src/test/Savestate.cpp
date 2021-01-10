// clang-format off
#include <gtest/gtest.h>
// clang-format on

#include "Savestate.h"

#include "ChunkHelper.h"
#include "SavestateLoader.h"

namespace {
    namespace SavestateDeSerialisation {
        struct MockCpu68k {
            uint32 x{0};
            int16 y{0};
            double z{0};
            string str{"hello world"};

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
                helper.Do32(x).Do16(y).DoString(str, 15).DoDouble(z);
            }

            bool operator==(const MockCpu68k other) const {
                return (x == other.x) && (y == other.y) && (z == other.z) && (str == other.str);
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
                DoSaveLoad(helper);
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

        struct MockRoot {
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

        TEST(SavestateDeSerialisation, WorksOnce) {
            MockRegsEZ regsEZ{1, 55, 66, true};
            MockCpu68k cpu68k{0x12345678, -17, 3.5};
            MockRoot root{cpu68k, regsEZ};

            Savestate savestate;
            ASSERT_TRUE(savestate.Save(root));
            ASSERT_EQ(savestate.GetSize(), 60u);

            MockRoot loadRoot;
            SavestateLoader loader;

            ASSERT_TRUE(loader.Load(savestate.GetBuffer(), savestate.GetSize(), loadRoot));
            ASSERT_EQ(loadRoot.cpu68k, cpu68k);
            ASSERT_EQ(loadRoot.regsEZ, regsEZ);
        }

        TEST(SavestateDeSerialisation, WorksTwice) {
            MockRegsEZ regsEZ{1, 55, 66, true};
            MockCpu68k cpu68k{0x12345678, -17, 3.5};
            MockRoot root{cpu68k, regsEZ};

            Savestate savestate;
            ASSERT_TRUE(savestate.Save(root));
            ASSERT_EQ(savestate.GetSize(), 60u);

            MockRoot loadRoot;
            SavestateLoader loader;

            ASSERT_TRUE(loader.Load(savestate.GetBuffer(), savestate.GetSize(), loadRoot));
            ASSERT_EQ(loadRoot.cpu68k, cpu68k);
            ASSERT_EQ(loadRoot.regsEZ, regsEZ);

            regsEZ = {2, 66, 55, false};
            cpu68k = {0x87654321, -33, -1.2};
            root = {cpu68k, regsEZ};

            ASSERT_TRUE(savestate.Save(root));
            ASSERT_TRUE(loader.Load(savestate.GetBuffer(), savestate.GetSize(), loadRoot));
            ASSERT_EQ(loadRoot.cpu68k, cpu68k);
            ASSERT_EQ(loadRoot.regsEZ, regsEZ);
        }

    }  // namespace SavestateDeSerialisation

    namespace RequestingAChunkTwiceDuringSaveGeneratesAnError {
        struct Mock {
            template <typename T>
            void Save(T& savestate) {
                savestate.GetChunk(ChunkType::cpu68k);
                savestate.GetChunk(ChunkType::cpu68k);
            }
        };

        TEST(SavestateSave, RequestingAChunkTwiceDuringSaveGeneratesAnError) {
            Savestate savestate;
            Mock mock;

            ASSERT_FALSE(savestate.Save(mock));
        };
    }  // namespace RequestingAChunkTwiceDuringSaveGeneratesAnError

    namespace ChunkErrorsArePropagated {
        struct Mock1 {
            template <typename T>
            void Save(T& savestate) {
                typename T::chunkT* chunk = savestate.GetChunk(ChunkType::cpu68k);
                if (!chunk) return;

                chunk->Put32(0);
            }
        };

        struct Mock2 {
            template <typename T>
            void Save(T& savestate) {
                typename T::chunkT* chunk = savestate.GetChunk(ChunkType::cpu68k);
                if (!chunk) return;

                chunk->Put64(0);
            }
        };

        TEST(SavestateSave, ChunkErrorsArePropagated) {
            Savestate savestate;
            Mock1 mock1;
            Mock2 mock2;

            ASSERT_TRUE(savestate.Save(mock1));
            ASSERT_FALSE(savestate.Save(mock2));
        }
    }  // namespace ChunkErrorsArePropagated

    namespace NotifyErrorRaisesAnError {
        struct Mock1 {
            template <typename T>
            void Save(T& savestate) {}
        };

        struct Mock2 {
            template <typename T>
            void Save(T& savestate) {
                savestate.NotifyError();
            }
        };

        TEST(SavestateSave, NotifyErrorRaisesAnError) {
            Savestate savestate;
            Mock1 mock1;
            Mock2 mock2;

            ASSERT_TRUE(savestate.Save(mock1));
            ASSERT_FALSE(savestate.Save(mock2));
        }
    }  // namespace NotifyErrorRaisesAnError

}  // namespace
