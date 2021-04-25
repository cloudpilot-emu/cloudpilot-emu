#include "Fifo.h"

Fifo::Fifo(uint32 capacity) : capacity(capacity) {
    buffer = new uint32[capacity];
    memset(buffer, 0, sizeof(uint32) * capacity);
}

Fifo::~Fifo() { delete[] buffer; }

void Fifo::Push(uint32 value) {
    buffer[(start + size) % capacity] = value;

    if (size < capacity)
        size++;
    else
        start = (start + 1) % capacity;
}

uint32 Fifo::Pop() {
    if (size == 0) return 0;

    const uint32 value = buffer[start];
    start = (start + 1) % capacity;

    size--;

    return value;
}

uint32 Fifo::Size() const { return size; }

void Fifo::Clear() {
    size = 0;
    start = 0;
}
