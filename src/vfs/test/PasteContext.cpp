#include "PasteContext.h"

#include <gtest/gtest.h>

#include "FSFixture.h"
#include "VfsTest.h"

namespace {

    class PasteContextTest : public VfsTest {
        void SetUp() override {
            FSFixture::CreateAndMount(0);
            FSFixture::CreateAndMount(1);
        }

        void TearDown() override {
            FSFixture::UnmountAndRelease(0);
            FSFixture::UnmountAndRelease(1);
        }

       protected:
        void RunUntilInterruption(PasteContext& context) {
            do {
                context.Continue();
            } while (context.GetState() == static_cast<int>(PasteContext::State::more));
        }
    };

    TEST_F(PasteContextTest, itCopiesFilesAndDirectories) {
        FSFixture::CreateFile("1:/foo.txt", "Hello");
        FSFixture::CreateFile("1:/bar.txt", "world");
        f_mkdir("1:/baz");

        PasteContext context(10, "/", "1:/");
        context.AddFile("foo.txt").AddFile("bar.txt").AddDirectory("baz");

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/foo.txt", "Hello");
        AssertFileExistsWithContent("0:/bar.txt", "world");
        AssertDirectoryExists("0:/baz");
    }

    TEST_F(PasteContextTest, itCopiesFilesAndDirectoriesWithrefix) {
        f_mkdir("1:/prefix");
        f_mkdir("1:/prefix/baz");
        FSFixture::CreateFile("1:/prefix/foo.txt", "Hello");
        FSFixture::CreateFile("1:/prefix/bar.txt", "world");

        PasteContext context(10, "/hanni", "1:/prefix");
        context.AddFile("foo.txt").AddFile("bar.txt").AddDirectory("baz");

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/hanni/foo.txt", "Hello");
        AssertFileExistsWithContent("0:/hanni/bar.txt", "world");
        AssertDirectoryExists("0:/hanni/baz");
    }

    TEST_F(PasteContextTest, itCopiesDirectoriesRecursively) {
        f_mkdir("1:/foo");
        f_mkdir("1:/foo/bar");
        FSFixture::CreateFile("1:/foo/hello.txt", "Hello");
        FSFixture::CreateFile("1:/foo/bar/world.txt", "world");

        PasteContext context(10, "/", "1:/");
        context.AddDirectory("foo");

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/foo/hello.txt", "Hello");
        AssertFileExistsWithContent("0:/foo/bar/world.txt", "world");
    }

    TEST_F(PasteContextTest, itCopiesDirectoriesRecursivelyWithPrefix) {
        f_mkdir("1:/prefix");
        f_mkdir("1:/prefix/foo");
        f_mkdir("1:/prefix/foo/bar");
        FSFixture::CreateFile("1:/prefix/foo/hello.txt", "Hello");
        FSFixture::CreateFile("1:/prefix/foo/bar/world.txt", "world");

        PasteContext context(10, "/destination", "1:/prefix");
        context.AddDirectory("foo");

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/destination/foo/hello.txt", "Hello");
        AssertFileExistsWithContent("0:/destination/foo/bar/world.txt", "world");
    }

    TEST_F(PasteContextTest, itRemovesFilesFromCutSelection) {
        f_mkdir("1:/prefix");
        f_mkdir("1:/prefix/foo");
        f_mkdir("1:/prefix/foo/bar");
        FSFixture::CreateFile("1:/prefix/foo/hello.txt", "Hello");
        FSFixture::CreateFile("1:/prefix/foo/bar/world.txt", "world");

        PasteContext context(10, "/destination", "1:/prefix");
        context.AddDirectory("foo").SetDeleteAfterCopy(true);

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/destination/foo/hello.txt", "Hello");
        AssertFileExistsWithContent("0:/destination/foo/bar/world.txt", "world");
        AssertFileDoesNotExist("1:/prefix/foo");
    }

}  // namespace
