#include "ChunkProbe.h"

void ChunkProbe::Put8(uint8_t value) { size++; }

void ChunkProbe::Put16(uint16_t value) { size++; }

void ChunkProbe::Put32(uint32_t value) { size++; };

void ChunkProbe::Put64(uint64_t value) { size += 2; }

void ChunkProbe::PutBool(bool value) { size++; }

void ChunkProbe::PutDouble(double value) { size += 2; }

void ChunkProbe::PutBuffer(void* buffer, size_t size) {
    this->size = this->size + size / 4 + ((size % 4) ? 1 : 0);
}

bool ChunkProbe::HasError() const { return false; }

size_t ChunkProbe::GetSize() const { return size; }

void ChunkProbe::PutString(const std::string& str, size_t maxLength) {
    PutBuffer(nullptr, maxLength + 1);
}
