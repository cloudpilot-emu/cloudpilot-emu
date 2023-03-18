
#include <gtest/gtest.h>

#include <string>

#include "VfsUtil.h"

using namespace std;

namespace {
    TEST(NormalizePath, itAddsLeadingSlash) {
        ASSERT_EQ(util::normalizePath("foo/bar"), string("/foo/bar"));
    }

    TEST(NormalizePath, itReplacesMultipleSlashes) {
        ASSERT_EQ(util::normalizePath("//foo///bar///"), string("/foo/bar/"));
    }

    TEST(NormalizePath, itReplaceBackslashesWithSlashes) {
        ASSERT_EQ(util::normalizePath("\\//\\/foo\\//bar"), string("/foo/bar"));
    }

    TEST(NormalizePath, itPreservesDrivePrefix) {
        ASSERT_EQ(util::normalizePath("1:/foo///bar"), string("1:/foo/bar"));
    }
}  // namespace
