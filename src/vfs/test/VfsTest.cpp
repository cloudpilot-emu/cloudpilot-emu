#include "VfsTest.h"

#include "fatfs/ff.h"

using namespace std;

void VfsTest::AssertFileExistsWithContent(const string& path, const string& content) {
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

void VfsTest::AssertFileExistsWithSize(const string& path, size_t size) {
    FILINFO fileInfo;

    ASSERT_EQ(f_stat(path.c_str(), &fileInfo), FR_OK);
    ASSERT_EQ(fileInfo.fattrib & AM_DIR, 0);
    ASSERT_EQ(fileInfo.fsize, size);
}

void VfsTest::AssertFileDoesNotExist(const string& path) {
    FILINFO fileInfo;

    ASSERT_EQ(f_stat(path.c_str(), &fileInfo), FR_NO_FILE);
}

void VfsTest::AssertDirectoryExists(const string& path) {
    FILINFO fileInfo;

    ASSERT_EQ(f_stat(path.c_str(), &fileInfo), FR_OK);
    ASSERT_EQ(fileInfo.fattrib & AM_DIR, AM_DIR);
}
