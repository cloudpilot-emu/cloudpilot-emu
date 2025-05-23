#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace util {
    bool ReadFile(std::optional<std::string> file, std::unique_ptr<uint8_t[]>& buffer, size_t& len);

    bool WriteFile(const std::string& file, const uint8_t* buffer, size_t len);
}  // namespace util

#endif  // _FILE_UTIL_H_