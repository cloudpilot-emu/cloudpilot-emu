#ifndef _VFS_TEST_H_
#define _VFS_TEST_H_

#include <gtest/gtest.h>

#include <string>

class VfsTest : public ::testing::Test {
   protected:
    void AssertFileExistsWithContent(const std::string& path, const std::string& content);
    void AssertFileExistsWithSize(const std::string& path, size_t size);
    void AssertFileDoesNotExist(const std::string& path);
    void AssertDirectoryExists(const std::string& path);
};

#endif  // _VFS_TEST_H_
