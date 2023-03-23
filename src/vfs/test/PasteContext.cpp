#include "PasteContext.h"

#include <gtest/gtest.h>

#include "FSFixture.h"
#include "VfsTest.h"

using namespace std;

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
        context.AddFile("foo.txt").AddFile("bar.txt").AddFile("baz");

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
        context.AddFile("foo.txt").AddFile("bar.txt").AddFile("baz");

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
        context.AddFile("foo");

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

        PasteContext context(10, "0:/destination", "1:/prefix");
        context.AddFile("foo");

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
        context.AddFile("foo").SetDeleteAfterCopy(true);

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/destination/foo/hello.txt", "Hello");
        AssertFileExistsWithContent("0:/destination/foo/bar/world.txt", "world");
        AssertFileDoesNotExist("1:/prefix/foo");
    }

    TEST_F(PasteContextTest, itIgnoresMissingFiles) {
        FSFixture::CreateFile("1:/foo.txt", "Hello");
        FSFixture::CreateFile("1:/bar.txt", "world");
        f_mkdir("1:/baz");

        PasteContext context(10, "/", "1:/");
        context.AddFile("foo.txt").AddFile("bar.txt").AddFile("bazalla").AddFile("baz");

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/foo.txt", "Hello");
        AssertFileExistsWithContent("0:/bar.txt", "world");
        AssertDirectoryExists("0:/baz");
    }

    TEST_F(PasteContextTest, itOverwritesFilesAfterConfirmationAndRemovesTheOriginal) {
        FSFixture::CreateFile("1:/foo.txt", "Hello");
        FSFixture::CreateFile("1:/bar.txt", "world");
        FSFixture::CreateFile("0:/bar.txt", "I get removed");

        PasteContext context(10, "/", "1:/");
        context.AddFile("foo.txt").AddFile("bar.txt").SetDeleteAfterCopy(true);

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::collision));
        ASSERT_EQ(string(context.GetCollisionPath()), "/bar.txt");

        context.ContinueWithOverwrite();
        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/foo.txt", "Hello");
        AssertFileExistsWithContent("0:/bar.txt", "world");
        AssertFileDoesNotExist("1:/foo.txt");
        AssertFileDoesNotExist("1:/bar.txt");
    }

    TEST_F(PasteContextTest, itOverwritesDirectoriesAfterConfirmationAndRemovesTheOriginal) {
        FSFixture::CreateFile("1:/foo.txt", "Hello");
        FSFixture::CreateFile("1:/bar.txt", "world");
        f_mkdir("0:/bar.txt");
        FSFixture::CreateFile("0:/bar.txt/foo.txt", "I get removed");

        PasteContext context(10, "/", "1:/");
        context.AddFile("foo.txt").AddFile("bar.txt").SetDeleteAfterCopy(true);

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(),
                  static_cast<int>(PasteContext::State::collisionWithDirectory));
        ASSERT_EQ(string(context.GetCollisionPath()), "/bar.txt");

        context.ContinueWithOverwrite();
        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/foo.txt", "Hello");
        AssertFileExistsWithContent("0:/bar.txt", "world");
        AssertFileDoesNotExist("1:/foo.txt");
        AssertFileDoesNotExist("1:/bar.txt");
    }

    TEST_F(PasteContextTest, itPreservesFilesThatCouldNotBeCopied) {
        FSFixture::CreateFile("1:/foo.txt", "Hello");
        FSFixture::CreateFile("1:/bar.txt", "world");
        FSFixture::CreateFile("0:/bar.txt", "I won't get removed");

        PasteContext context(10, "/", "1:/");
        context.AddFile("foo.txt").AddFile("bar.txt").SetDeleteAfterCopy(true);

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::collision));
        ASSERT_EQ(string(context.GetCollisionPath()), "/bar.txt");

        context.Continue();
        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/foo.txt", "Hello");
        AssertFileExistsWithContent("0:/bar.txt", "I won't get removed");
        AssertFileDoesNotExist("1:/foo.txt");
        AssertFileExistsWithContent("1:/bar.txt", "world");
    }

    // it preserves the hierarchy of files that could not be copied

    TEST_F(PasteContextTest, itPreservesTheHierarchyOfFilesThatCouldNotBeCopied) {
        f_mkdir("1:/dir1");
        f_mkdir("1:/dir1/dir2");
        FSFixture::CreateFile("1:/foo.txt", "Hello");
        FSFixture::CreateFile("1:/dir1/bar.txt", "world");
        FSFixture::CreateFile("1:/dir1/dir2/bar.txt", "world");

        f_mkdir("0:/dir1");
        f_mkdir("0:/dir1/dir2");
        FSFixture::CreateFile("0:/dir1/dir2/bar.txt", "I won't get removed");

        PasteContext context(10, "/", "1:/");
        context.AddFile("foo.txt")
            .AddFile("dir1/bar.txt")
            .AddFile("dir1/dir2/bar.txt")
            .SetDeleteAfterCopy(true);

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::collision));
        ASSERT_EQ(string(context.GetCollisionPath()), "/dir1/dir2/bar.txt");

        context.Continue();
        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("0:/foo.txt", "Hello");
        AssertFileExistsWithContent("0:/dir1/bar.txt", "world");
        AssertFileExistsWithContent("0:/dir1/dir2/bar.txt", "I won't get removed");

        AssertFileDoesNotExist("1:/foo.txt");
        AssertFileDoesNotExist("1:/dir1/bar.txt");
        AssertFileExistsWithContent("1:/dir1/dir2/bar.txt", "world");
    }

    TEST_F(PasteContextTest, itDoesNotAttemptToCopyParentsToChildren) {
        f_mkdir("/prefix");
        f_mkdir("/prefix/foo");
        f_mkdir("/prefix/bar");
        f_mkdir("/prefix/bar/baz");
        FSFixture::CreateFile("/prefix/foo.txt", "Hello");
        FSFixture::CreateFile("/prefix/foo/bar.txt", "world");
        FSFixture::CreateFile("/prefix/bar/bazinga.txt", "bazinga");

        PasteContext context(10, "0:/prefix/bar", "0:/prefix");
        context.AddFile("foo.txt").AddFile("foo").AddFile("bar").SetDeleteAfterCopy(true);

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileDoesNotExist("/prefix/foo.txt");
        AssertFileDoesNotExist("/prefix/foo");
        AssertFileExistsWithContent("/prefix/bar/foo.txt", "Hello");
        AssertFileExistsWithContent("/prefix/bar/foo/bar.txt", "world");
        AssertFileExistsWithContent("/prefix/bar/bazinga.txt", "bazinga");
        AssertFileDoesNotExist("/prefix/bar/bar");
    }

    TEST_F(PasteContextTest, overlapDetectionCorrectlyHandlesFalseFriends) {
        f_mkdir("/prefix");
        f_mkdir("/prefix/bara");
        f_mkdir("/prefix/bar");
        FSFixture::CreateFile("/prefix/foo.txt", "Hello");
        FSFixture::CreateFile("/prefix/bar/bazinga.txt", "bazinga");

        PasteContext context(10, "/prefix/bara", "/prefix");
        context.AddFile("foo.txt").AddFile("bar").SetDeleteAfterCopy(true);

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileDoesNotExist("/prefix/foo.txt");
        AssertFileDoesNotExist("/prefix/bar");
        AssertFileExistsWithContent("/prefix/bara/foo.txt", "Hello");
        AssertFileExistsWithContent("/prefix/bara/bar/bazinga.txt", "bazinga");
    }

    TEST_F(PasteContextTest, itDoesNotTryToOverwriteTheSameFile) {
        f_mkdir("/prefix");
        FSFixture::CreateFile("/prefix/foo.txt", "Hello");

        PasteContext context(10, "0:/prefix", "0:/prefix");
        context.AddFile("foo.txt");

        RunUntilInterruption(context);
        ASSERT_EQ(context.GetState(), static_cast<int>(PasteContext::State::done));

        AssertFileExistsWithContent("/prefix/foo.txt", "Hello");
    }

}  // namespace
