#include "DeleteRecursiveContext.h"

#include <gtest/gtest.h>

#include "FSFixture.h"
#include "VfsTest.h"

using namespace std;

namespace {

    class DeleteRecursiveContextTest : public VfsTest {
        void SetUp() override { FSFixture::CreateAndMount(); }

        void TearDown() override { FSFixture::UnmountAndRelease(); }
    };

    TEST_F(DeleteRecursiveContextTest, itDeletesPlainFilesAndDirectories) {
        FSFixture::CreateFile("/file1.txt", "Hello");
        FSFixture::CreateFile("/file2.txt", "world");
        FSFixture::CreateFile("/file3.txt", "foo");
        f_mkdir("/dir1");
        f_mkdir("/dir2");

        DeleteRecursiveContext context(10);
        context.AddFile("/file1.txt").AddFile("/file2.txt").AddDirectory("/dir1");

        while (context.GetState() == static_cast<int>(DeleteRecursiveContext::State::initial) ||
               context.GetState() == static_cast<int>(DeleteRecursiveContext::State::more))
            context.Continue();

        ASSERT_EQ(context.GetState(), static_cast<int>(DeleteRecursiveContext::State::done));
        AssertFileDoesNotExist("/file1.txt");
        AssertFileDoesNotExist("/file2.txt");
        AssertFileDoesNotExist("/dir1");
        AssertFileExistsWithContent("/file3.txt", "foo");
        AssertDirectoryExists("/dir2");
    }

    TEST_F(DeleteRecursiveContextTest, itRecursivelyDeletesDirectories) {
        f_mkdir("/dir1/dir2");
        f_mkdir("/dir1/dir3");
        FSFixture::CreateFile("/dir1/file1", "hello");
        FSFixture::CreateFile("/dir1/dir2/file2", "world");

        DeleteRecursiveContext context(10);
        context.AddDirectory("/dir1");

        while (context.GetState() == static_cast<int>(DeleteRecursiveContext::State::initial) ||
               context.GetState() == static_cast<int>(DeleteRecursiveContext::State::more))
            context.Continue();

        AssertFileDoesNotExist("/dir1");
    }

    TEST_F(DeleteRecursiveContextTest, itStopsWithErrorIfDeleteFails) {
        class MockFatfsDelegate : public FatfsDelegate {
           public:
            FRESULT f_unlink(const TCHAR* path) override {
                if (string(path) == "/file2.txt") return FR_DISK_ERR;

                return FatfsDelegate::f_unlink(path);
            }
        };

        FSFixture::CreateFile("/file1.txt", "Hello");
        FSFixture::CreateFile("/file2.txt", "world");

        MockFatfsDelegate fatfsDelegate;
        DeleteRecursiveContext context(10, fatfsDelegate);
        context.AddFile("/file1.txt").AddFile("/file2.txt");

        while (context.GetState() == static_cast<int>(DeleteRecursiveContext::State::initial) ||
               context.GetState() == static_cast<int>(DeleteRecursiveContext::State::more))
            context.Continue();

        ASSERT_EQ(context.GetState(), static_cast<int>(DeleteRecursiveContext::State::error));
        ASSERT_EQ(context.GetFailingPath(), string("/file2.txt"));
    }

}  // namespace
