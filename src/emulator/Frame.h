#ifndef _FRAME_H_
#define _FRAME_H_

#include <memory>

#include "EmCommon.h"

struct Frame {
    Frame(size_t bufferSize);

    uint8 bpp{0};

    uint32 lineWidth{0};
    uint32 lines{0};
    uint8 margin{0};
    uint32 bytesPerLine{0};

    uint8* GetBuffer();
    size_t GetBufferSize() const;

   private:
    const unique_ptr<uint8[]> buffer;
    const size_t bufferSize;

    Frame(const Frame&) = delete;
    Frame(Frame&&) = delete;

    Frame& operator=(const Frame&) = delete;
    Frame& operator=(Frame&&) = delete;
};

#endif  // _FRAME_H_
