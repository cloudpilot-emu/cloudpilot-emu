#ifndef _ENCODING_H_
#define _ENCODING_H_

#include <cstddef>
#include <string>

std::string Utf8ToIsolatin1(const std::string& input, size_t maxLen = 0xffff);
std::string Isolatin1ToUtf8(const std::string& input);

#endif  // _ENCODING_H_