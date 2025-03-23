// clang-format off
#include <gtest/gtest.h>
// clang-format on

#include "savestate/Savestate.h"

#include <cstdint>
#include <string>

#include "savestate/ChunkHelper.h"
#include "savestate/SavestateLoader.h"

using namespace std;

namespace {
    enum class ChunkType { cpu68k, regsEZ };

    namespace SavestateDeSerialisation {
        struct MockCpu68k {
            uint32_t x{0};
            int16_t y{0};
            double z{0};
            string str{"hello world"};

            template <typename T>
            void Save(T& savestate) {
                typename T::chunkT* chunk = savestate.GetChunk(ChunkType::cpu68k);
                if (!chunk) return;

                SaveChunkHelper helper(*chunk);
                DoSaveLoad(helper);
            }

            void Load(SavestateLoader<ChunkType>& loader) {
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
            uint8_t x{0};
            uint8_t y{0};
            uint8_t z{0};
            bool b1{false}, b2{true};

            template <typename T>
            void Save(T& savestate) {
                typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsEZ);
                if (!chunk) return;

                SaveChunkHelper helper(*chunk);
                DoSaveLoad(helper);
            }

            void Load(SavestateLoader<ChunkType>& loader) {
                Chunk* chunk = loader.GetChunk(ChunkType::regsEZ);
                if (!chunk) return;

                LoadChunkHelper helper(*chunk);
                DoSaveLoad(helper);
            }

            template <typename T>
            void DoSaveLoad(T& helper) {
                helper.Do(typename T::Pack8() << x << y << z)
                    .Do(typename T::BoolPack() << b1 << b2);
            }

            bool operator==(const MockRegsEZ other) const {
                return (x == other.x) && (y == other.y) && (z == other.z) && (b1 == other.b1) &&
                       (b2 == other.b2);
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

            void Load(SavestateLoader<ChunkType>& loader) {
                cpu68k.Load(loader);
                regsEZ.Load(loader);
            }
        };

        TEST(SavestateDeSerialisation, WorksOnce) {
            MockRegsEZ regsEZ{1, 55, 66, true};
            MockCpu68k cpu68k{0x12345678, -17, 3.5};
            MockRoot root{cpu68k, regsEZ};

            Savestate<ChunkType> savestate;
            ASSERT_TRUE(savestate.Save(root));
            ASSERT_EQ(savestate.GetSize(), 60u);

            MockRoot loadRoot;
            SavestateLoader<ChunkType> loader;

            ASSERT_TRUE(loader.Load(savestate.GetBuffer(), savestate.GetSize(), loadRoot));
            ASSERT_EQ(loadRoot.cpu68k, cpu68k);
            ASSERT_EQ(loadRoot.regsEZ, regsEZ);
        }

        TEST(SavestateDeSerialisation, WorksTwice) {
            MockRegsEZ regsEZ{1, 55, 66, true};
            MockCpu68k cpu68k{0x12345678, -17, 3.5};
            MockRoot root{cpu68k, regsEZ};

            Savestate<ChunkType> savestate;
            ASSERT_TRUE(savestate.Save(root));
            ASSERT_EQ(savestate.GetSize(), 60u);

            MockRoot loadRoot;
            SavestateLoader<ChunkType> loader;

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
            Savestate<ChunkType> savestate;
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
            Savestate<ChunkType> savestate;
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
            Savestate<ChunkType> savestate;
            Mock1 mock1;
            Mock2 mock2;

            ASSERT_TRUE(savestate.Save(mock1));
            ASSERT_FALSE(savestate.Save(mock2));
        }
    }  // namespace NotifyErrorRaisesAnError

}  // namespace
