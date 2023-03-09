#include "VfsUtil.h"

#include <chrono>
#include <regex>

using namespace std;

namespace {
    regex REGEX_MULTI_SLASH("/{2,}|\\\\+");
}

uint64_t util::epochMilliseconds() {
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch())
        .count();
}

std::string util::normalizePath(const std::string& path) {
    return regex_replace(path, REGEX_MULTI_SLASH, "/");
}
