#ifndef _FRAME_H_
#define _FRAME_H_

#include "EmCommon.h"

struct Frame {
    Frame(size_t bufferSize);
    ~Frame();

    bool lcdOn{false};

    uint8 bpp{0};

    uint32 lineWidth{0};
    uint32 lines{0};
    uint32 margin{0};

    uint8* buffer{nullptr};
    const size_t bufferSize;

   private:
    Frame(const Frame&) = delete;
    Frame(Frame&&) = delete;

    Frame& operator=(const Frame&) = delete;
    Frame& operator=(Frame&&) = delete;
};

#endif  // _FRAME_H_
