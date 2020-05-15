#include "Frame.h"

Frame::Frame(size_t bufferSize) : bufferSize(bufferSize) { buffer = new uint8[bufferSize]; }

Frame::~Frame() { delete[] buffer; }
