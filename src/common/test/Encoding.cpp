#include <gmock/gmock.h>
#include <gtest/gtest.h>

// clang-format off
#include "encoding.h"
// clang-format on

namespace {
    TEST(Utf8ToIsolatin1Test, itConvertsUTF8CharsToIsolatin1) {
        ASSERT_EQ(Utf8ToIsolatin1("\xc3\xa4\xc3\xb6\xc3\xbc"), "\xe4\xf6\xfc");
    }

    TEST(Utf8ToIsolatin1Test, itHonorsMaxlength) {
        ASSERT_EQ(Utf8ToIsolatin1("\xc3\xa4\xc3\xb6\xc3\xbc", 2), "\xe4\xf6");
    }

    TEST(Utf8ToIsolatin1Test, itReplacesExoticCharctersWithUnderscore) {
        ASSERT_EQ(Utf8ToIsolatin1("\xf0\x9f\x98\xc3\xa4\xc3\xb6\xc3\xbc"), "_\xe4\xf6\xfc");
    }

    TEST(Isolatin1ToUtf8, itConvertsIsolatin1ToUTF8) {
        ASSERT_EQ(Isolatin1ToUtf8("\xe4\xf6\xfc"), "\xc3\xa4\xc3\xb6\xc3\xbc");
    }

}  // namespace
