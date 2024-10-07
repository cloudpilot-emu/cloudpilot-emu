#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace util {
    bool ReadFile(std::string file, std::unique_ptr<uint8_t[]>& buffer, size_t& len);
}

#endif  // _FILE_UTIL_H_