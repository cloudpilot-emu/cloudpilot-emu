#include "UnzipContext.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string>

#include "FSFixture.h"
#include "FatfsDelegate.h"
#include "VfsTest.h"
#include "zip.h"

using namespace std;

namespace {
    class UnzipContextTest : public VfsTest {
       protected:
        void SetUp() override {
            zip = zip_stream_open(nullptr, 0, 1, 'w');
            FSFixture::CreateAndMount();
        }

        void TearDown() override {
            if (zip) {
                zip_stream_close(zip);
                zip = nullptr;
            }

            if (zipData) {
                free(zipData);
                zipData = 0;
                zipSize = 0;
            }

            FSFixture::UnmountAndRelease();
        }

       protected:
        void AddZipEntry(const string& path, const string& data) {
            zip_entry_open(zip, path.c_str());
            zip_entry_write(zip, data.c_str(), data.size());
            zip_entry_close(zip);
        }

        void AddZipEntryZeroFilled(const string& path, size_t size) {
            constexpr size_t BUFFER_SIZE = 1024;

            unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(BUFFER_SIZE);
            memset(buffer.get(), 0, BUFFER_SIZE);

            zip_entry_open(zip, path.c_str());

            size_t bytesWritten = 0;
            while (bytesWritten < size) {
                const size_t bytesToWrite = min(BUFFER_SIZE, size - bytesWritten);

                zip_entry_write(zip, buffer.get(), bytesToWrite);
                bytesWritten += bytesToWrite;
            }

            zip_entry_close(zip);
        }

        void FinishZip() {
            zip_stream_copy(zip, &zipData, &zipSize);
            zip_stream_close(zip);
            zip = nullptr;
        }

        UnzipContext::State RunUntilInterruption(UnzipContext& unzipContext) {
            do {
                unzipContext.Continue();
            } while (unzipContext.GetState() == static_cast<int>(UnzipContext::State::more));

            return static_cast<UnzipContext::State>(unzipContext.GetState());
        }

       protected:
        zip_t* zip{nullptr};

        void* zipData{nullptr};
        ssize_t zipSize{0};
    };

    TEST_F(UnzipContextTest, itWritesPlainFiles) {
        AddZipEntry("/foo.txt", "Hello");
        AddZipEntry("bar.txt", "world!");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        while (context.GetState() == static_cast<int>(UnzipContext::State::more))
            context.Continue();

        ASSERT_EQ(context.GetState(), static_cast<int>(UnzipContext::State::done));
        ASSERT_EQ(context.GetEntriesTotal(), 2);
        ASSERT_EQ(context.GetEntriesSuccess(), 2);

        AssertFileExistsWithContent("foo.txt", "Hello");
        AssertFileExistsWithContent("bar.txt", "world!");
    }

    TEST_F(UnzipContextTest, itCreatesDirectoriesAsNecessary) {
        AddZipEntry("/foo/bar.txt", "Hello");
        AddZipEntry("/foo/bar/baz.txt", "world!");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);

        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);
        ASSERT_EQ(context.GetEntriesTotal(), 2);

        AssertFileExistsWithContent("/foo/bar.txt", "Hello");
        AssertFileExistsWithContent("/foo/bar/baz.txt", "world!");
    }

    TEST_F(UnzipContextTest, itUnpacksToNestedDirectories) {
        AddZipEntry("/bom/baz.txt", "Hello world!");
        FinishZip();

        f_mkdir("/foo/bar");

        UnzipContext context(10, "/foo/bar", zipData, zipSize);

        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);
        AssertFileExistsWithContent("/foo/bar/bom/baz.txt", "Hello world!");
    }

    TEST_F(UnzipContextTest, itHandlesMultipleSlashesAndBackslashes) {
        AddZipEntry("\\foo///bar.txt", "Hello");
        AddZipEntry("/foo\\\\bar/baz.txt", "world!");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);

        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);
        ASSERT_EQ(context.GetEntriesTotal(), 2);

        AssertFileExistsWithContent("/foo/bar.txt", "Hello");
        AssertFileExistsWithContent("/foo/bar/baz.txt", "world!");
    }

    TEST_F(UnzipContextTest, itHandlesOutOfSpaceGracefully) {
        constexpr size_t FILE_SIZE = 1024 * 1024;

        AddZipEntryZeroFilled("/blob1", FILE_SIZE);
        AddZipEntryZeroFilled("/blob2", FILE_SIZE);
        AddZipEntryZeroFilled("/blob3", FILE_SIZE);
        AddZipEntryZeroFilled("/blob4", FILE_SIZE);
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);

        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::cardFull);
        ASSERT_EQ(context.GetEntriesSuccess(), 3);
        AssertFileExistsWithSize("/blob1", FILE_SIZE);
        AssertFileExistsWithSize("/blob2", FILE_SIZE);
        AssertFileExistsWithSize("/blob3", FILE_SIZE);
        AssertFileDoesNotExist("/blob4");
    }

    TEST_F(UnzipContextTest, itDoesNotOverwriteAnExistingFileIfNotRequested) {
        FSFixture::CreateFile("/foo.txt", "Hello");
        AddZipEntry("/foo.txt", "world!");
        AddZipEntry("/bar.txt", "foobar");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::collision);
        ASSERT_EQ(string(context.GetCurrentEntry()), "/foo.txt");
        ASSERT_EQ(string(context.GetCollisionPath()), "/foo.txt");

        context.Continue();
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertFileExistsWithContent("/foo.txt", "Hello");
        AssertFileExistsWithContent("/bar.txt", "foobar");
        ASSERT_EQ(context.GetEntriesSuccess(), 1);
    }

    TEST_F(UnzipContextTest, itOverwritesAnExistingFileIfRequested) {
        FSFixture::CreateFile("/foo.txt", "Hello");
        AddZipEntry("/foo.txt", "world!");
        AddZipEntry("/bar.txt", "foobar");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::collision);

        context.ContinueWithOverwrite();
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertFileExistsWithContent("/foo.txt", "world!");
        AssertFileExistsWithContent("/bar.txt", "foobar");
        ASSERT_EQ(context.GetEntriesSuccess(), 2);
    }

    TEST_F(UnzipContextTest, itDoesNotOverwriteAnExistingDirectoryIfNotRequested) {
        f_mkdir("/foo.txt");
        AddZipEntry("/foo.txt", "world!");
        AddZipEntry("/bar.txt", "foobar");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::collisionWithDirectory);
        ASSERT_EQ(string(context.GetCurrentEntry()), "/foo.txt");
        ASSERT_EQ(string(context.GetCollisionPath()), "/foo.txt");

        context.Continue();
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertDirectoryExists("/foo.txt");
        AssertFileExistsWithContent("/bar.txt", "foobar");
        ASSERT_EQ(context.GetEntriesSuccess(), 1);
    }

    TEST_F(UnzipContextTest, itOverwritesAnExistingDirectoryIfRequested) {
        f_mkdir("/foo.txt");
        f_mkdir("/foo.txt/bar");
        FSFixture::CreateFile("/foo.txt/bar/hanni", "nanni");

        AddZipEntry("/foo.txt", "world!");
        AddZipEntry("/bar.txt", "foobar");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::collisionWithDirectory);

        context.ContinueWithOverwrite();
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertFileExistsWithContent("/foo.txt", "world!");
        AssertFileExistsWithContent("/bar.txt", "foobar");
        ASSERT_EQ(context.GetEntriesSuccess(), 2);
    }

    TEST_F(UnzipContextTest, itDoesNotCreatePreexistingPartsOfTheHierarchy) {
        f_mkdir("/foo");
        FSFixture::CreateFile("/foo/bar.txt", "hulpe");

        AddZipEntry("/foo/bar/baz.txt", "wolpe");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertFileExistsWithContent("/foo/bar/baz.txt", "wolpe");
        AssertFileExistsWithContent("/foo/bar.txt", "hulpe");
    }

    TEST_F(UnzipContextTest, filesWithinTheHierarchyCauseCollision) {
        f_mkdir("/foo");
        FSFixture::CreateFile("/foo/bar", "hulpe");

        AddZipEntry("/foo/bar/baz.txt", "wolpe");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::collision);
        ASSERT_EQ(string(context.GetCurrentEntry()), "/foo/bar/baz.txt");
        ASSERT_EQ(string(context.GetCollisionPath()), "/foo/bar");
    }

    TEST_F(UnzipContextTest, filesWithinTheHierarchyCauseCollisionAndAreOverwrittenOnRequest) {
        f_mkdir("/foo");
        FSFixture::CreateFile("/foo/bar", "hulpe");

        AddZipEntry("/foo/bar/baz.txt", "wolpe");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::collision);

        context.ContinueWithOverwrite();
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertFileExistsWithContent("/foo/bar/baz.txt", "wolpe");
    }

    TEST_F(UnzipContextTest, anInvalidZipArchiveTerminatesWithZipfileError) {
        unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(1024);
        memset(buffer.get(), 0, 1024);

        UnzipContext context(10, "/", buffer.get(), 1024);

        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::zipfileError);
    }

    TEST_F(UnzipContextTest, entriesWithInvalidPathAreSkiped) {
        class MockFatfsDelegate : public FatfsDelegate {
           public:
            virtual FRESULT f_open(FIL* fp, const TCHAR* path, BYTE mode) {
                return string(path) == "/invalid.txt" ? FR_INVALID_NAME
                                                      : FatfsDelegate::f_open(fp, path, mode);
            }
        };

        AddZipEntry("/foo.txt", "Hello");
        AddZipEntry("/invalid.txt", "world!");
        AddZipEntry("/baz.txt", "friend");
        FinishZip();

        MockFatfsDelegate fatfsDelegate;
        UnzipContext context(10, "/", zipData, zipSize, fatfsDelegate);

        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::invalidEntry);
        ASSERT_EQ(string(context.GetCurrentEntry()), "/invalid.txt");

        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertFileExistsWithContent("/foo.txt", "Hello");
        AssertFileExistsWithContent("/baz.txt", "friend");
        AssertFileDoesNotExist("/invalid.txt");
    }

    TEST_F(UnzipContextTest, unknownErrorsAreMappedToIoError) {
        class MockFatfsDelegate : public FatfsDelegate {
           public:
            virtual FRESULT f_open(FIL* fp, const TCHAR* path, BYTE mode) {
                return string(path) == "/invalid.txt" ? FR_DISK_ERR
                                                      : FatfsDelegate::f_open(fp, path, mode);
            }
        };

        AddZipEntry("/foo.txt", "Hello");
        AddZipEntry("/invalid.txt", "world!");
        FinishZip();

        MockFatfsDelegate fatfsDelegate;
        UnzipContext context(10, "/", zipData, zipSize, fatfsDelegate);

        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::ioError);
        ASSERT_EQ(string(context.GetCurrentEntry()), "/invalid.txt");
    }
}  // namespace
