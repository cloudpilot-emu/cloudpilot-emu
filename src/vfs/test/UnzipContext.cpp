#include "UnzipContext.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string>

#include "FSFixture.h"

using namespace std;

#include "zip.h"

namespace {
    class UnzipContextTest : public ::testing::Test {
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

        void AssertFileExistsWithContent(const string& path, const string& content) {
            FILINFO fileInfo;

            ASSERT_EQ(f_stat(path.c_str(), &fileInfo), FR_OK);
            ASSERT_EQ(fileInfo.fattrib & AM_DIR, 0);
            ASSERT_EQ(fileInfo.fsize, content.length());

            FIL file;
            ASSERT_EQ(f_open(&file, path.c_str(), FA_READ), FR_OK);

            unique_ptr<uint8_t[]> data = make_unique<uint8_t[]>(fileInfo.fsize);

            UINT bytesRead;
            ASSERT_EQ(f_read(&file, data.get(), fileInfo.fsize, &bytesRead), FR_OK);
            ASSERT_EQ(f_close(&file), FR_OK);

            ASSERT_EQ(bytesRead, fileInfo.fsize);
            ASSERT_EQ(memcmp(data.get(), content.c_str(), content.length()), 0);
        }

        void AssertFileExistsWithSize(const string& path, size_t size) {
            FILINFO fileInfo;

            ASSERT_EQ(f_stat(path.c_str(), &fileInfo), FR_OK);
            ASSERT_EQ(fileInfo.fattrib & AM_DIR, 0);
            ASSERT_EQ(fileInfo.fsize, size);
        }

        void AssertFileDoesNotExist(const string& path) {
            FILINFO fileInfo;

            ASSERT_EQ(f_stat(path.c_str(), &fileInfo), FR_NO_FILE);
        }

        void AssertDirectoryExists(const string& path) {
            FILINFO fileInfo;

            ASSERT_EQ(f_stat(path.c_str(), &fileInfo), FR_OK);
            ASSERT_EQ(fileInfo.fattrib & AM_DIR, AM_DIR);
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

        context.Continue();
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertFileExistsWithContent("/foo.txt", "Hello");
        AssertFileExistsWithContent("/bar.txt", "foobar");
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
    }

    TEST_F(UnzipContextTest, itDoesNotOverwriteAnExistingDirectoryIfNotRequested) {
        f_mkdir("/foo.txt");
        AddZipEntry("/foo.txt", "world!");
        AddZipEntry("/bar.txt", "foobar");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::collisionWithDirectory);

        context.Continue();
        ASSERT_EQ(RunUntilInterruption(context), UnzipContext::State::done);

        AssertDirectoryExists("/foo.txt");
        AssertFileExistsWithContent("/bar.txt", "foobar");
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
    }
}  // namespace
