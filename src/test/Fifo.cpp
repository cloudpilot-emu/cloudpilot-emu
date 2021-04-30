#include <gmock/gmock.h>
#include <gtest/gtest.h>

// clang-format off
#include "Fifo.h"
// clang-format on

#include "ChunkHelper.h"
#include "ChunkType.h"
#include "Savestate.h"
#include "SavestateLoader.h"

namespace {
    TEST(FifoTest, isEmptyOnCreation) {
        Fifo fifo(3);

        ASSERT_EQ(fifo.Size(), 0u);
    }

    TEST(FifoTest, popRetrievesTheLastEntry) {
        Fifo fifo(3);

        fifo.Push(1u);

        ASSERT_EQ(fifo.Size(), 1u);
        ASSERT_EQ(fifo.Pop(), 1u);
        ASSERT_EQ(fifo.Size(), 0u);
    }

    TEST(FifoTest, consecutivePushesAndPopsWork) {
        Fifo fifo(3);

        fifo.Push(1u);
        ASSERT_EQ(fifo.Pop(), 1u);

        fifo.Push(2u);
        ASSERT_EQ(fifo.Pop(), 2u);

        fifo.Push(3u);
        ASSERT_EQ(fifo.Pop(), 3u);

        fifo.Push(4u);
        ASSERT_EQ(fifo.Pop(), 4u);

        ASSERT_EQ(fifo.Size(), 0u);
    }

    TEST(FifoTest, theOldestElementIsDiscardedOnOverflow) {
        Fifo fifo(3);

        fifo.Push(1u);
        fifo.Push(2u);
        fifo.Push(3u);
        fifo.Push(4u);

        ASSERT_EQ(fifo.Pop(), 2u);
        ASSERT_EQ(fifo.Pop(), 3u);
        ASSERT_EQ(fifo.Pop(), 4u);
        ASSERT_EQ(fifo.Size(), 0u);
    }

    TEST(FifoTest, popReturnsZeroWhenEmpty) {
        Fifo fifo(3);

        ASSERT_EQ(fifo.Pop(), 0u);
    }

    class MockRoot {
       public:
        Fifo fifo{3};

        template <typename T>
        void Save(T& savestate) {
            auto* chunk = savestate.GetChunk(ChunkType::cpu68k);
            SaveChunkHelper helper(*chunk);

            fifo.DoSaveLoad(helper);
        }

        void Load(SavestateLoader& loader) {
            auto* chunk = loader.GetChunk(ChunkType::cpu68k);
            LoadChunkHelper helper(*chunk);

            fifo.DoSaveLoad(helper);
        }
    };

    TEST(FifoTest, deSearializesCorrectly) {
        Savestate savestate;
        MockRoot root;

        root.fifo.Push(1u);
        root.fifo.Push(2u);
        root.fifo.Push(3u);

        savestate.Save(root);

        MockRoot deserializedRoot;
        SavestateLoader loader;

        ASSERT_TRUE(loader.Load(savestate.GetBuffer(), savestate.GetSize(), deserializedRoot));

        ASSERT_EQ(deserializedRoot.fifo.Pop(), 1u);
        ASSERT_EQ(deserializedRoot.fifo.Pop(), 2u);
        ASSERT_EQ(deserializedRoot.fifo.Pop(), 3u);
        ASSERT_EQ(deserializedRoot.fifo.Size(), 0u);
    }

}  // namespace
