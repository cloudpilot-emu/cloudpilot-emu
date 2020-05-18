#include "Frame.h"

Frame::Frame(size_t bufferSize)
    : buffer(make_unique<uint8[]>(bufferSize)), bufferSize(bufferSize) {}

uint8* Frame::GetBuffer() { return buffer.get(); }

size_t Frame::GetBufferSize() const { return bufferSize; }
