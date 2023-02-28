#include "UnzipContext.h"

#include <gtest/gtest.h>

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

        void FinishZip() {
            zip_stream_copy(zip, &zipData, &zipSize);
            zip_stream_close(zip);
            zip = nullptr;
        }

        void AssertFile(const string& path, const string& content) {
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

        AssertFile("foo.txt", "Hello");
        AssertFile("bar.txt", "world!");
    }

    TEST_F(UnzipContextTest, itCreatesDirectoriesAsNecessary) {
        AddZipEntry("/foo/bar.txt", "Hello");
        AddZipEntry("/foo/bar/baz.txt", "world!");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        while (context.GetState() == static_cast<int>(UnzipContext::State::more))
            context.Continue();

        ASSERT_EQ(context.GetState(), static_cast<int>(UnzipContext::State::done));
        ASSERT_EQ(context.GetEntriesTotal(), 2);

        AssertFile("/foo/bar.txt", "Hello");
        AssertFile("/foo/bar/baz.txt", "world!");
    }

    TEST_F(UnzipContextTest, itHandlesMultipleSlashesAndBackslashes) {
        AddZipEntry("/foo///bar.txt", "Hello");
        AddZipEntry("/foo\\bar/baz.txt", "world!");
        FinishZip();

        UnzipContext context(10, "/", zipData, zipSize);
        while (context.GetState() == static_cast<int>(UnzipContext::State::more))
            context.Continue();

        ASSERT_EQ(context.GetState(), static_cast<int>(UnzipContext::State::done));
        ASSERT_EQ(context.GetEntriesTotal(), 2);

        AssertFile("/foo/bar.txt", "Hello");
        AssertFile("/foo/bar/baz.txt", "world!");
    }
}  // namespace
