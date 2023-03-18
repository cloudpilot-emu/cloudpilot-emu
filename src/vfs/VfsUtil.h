#ifndef _VFS_UTIL_H_
#define _VFS_UTIL_H_

#include <cstdint>
#include <string>

namespace util {
    uint64_t epochMilliseconds();

    std::string normalizePath(const std::string& path);

    const char* drivePrefix(unsigned int slot);
}  // namespace util

#endif  //  _VFS_UTIL_H_
