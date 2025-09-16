#ifndef _RLE_
#define _RLE_

#include <cstdint>
#include <memory>

#include "buffer.h"

bool rle_decode_chunk(size_t sourceSize, const uint8_t* sourceData, size_t destSize,
                      uint8_t* destBuffer);

size_t rle_get_chunk_size(size_t sourceSize, const uint8_t* sourceData, bool& success);

const char* rle_getLastError();

#endif  // _RLE_