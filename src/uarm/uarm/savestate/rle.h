#ifndef _RLE_
#define _RLE_

#include <cstdint>
#include <memory>

#include "buffer.h"

bool rle_decode(struct Buffer source, std::unique_ptr<uint8_t[]>& dataUncompressed,
                size_t& lenUncompressed);

const char* rle_getLastError();

#endif  // _RLE_