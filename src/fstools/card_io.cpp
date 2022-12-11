#include "card_io.h"

#include "CardVolume.h"

namespace {
    CardVolume* volume = nullptr;
}

int card_get_size() { return volume->GetSize(); }

int card_open() { return volume != nullptr; }

int card_read(int offset, int size, void* buffer) {
    return volume != nullptr ? volume->Read(offset, size, reinterpret_cast<uint8_t*>(buffer)) : 0;
}

int card_is_valid_range(int offset, int size) {
    if (volume == nullptr) return 0;

    return offset >= 0 && size >= 0 && offset + size <= volume->GetSize();
}

int card_write(int offset, int size, const void* buffer) {
    return volume != nullptr ? volume->Write(offset, size, reinterpret_cast<const uint8_t*>(buffer))
                             : 0;
}

int card_close() { return volume != nullptr; }

int card_geometry_sectors() { return volume == nullptr ? 0 : volume->GetGeometrySectors(); }

int card_geometry_heads() { return volume == nullptr ? 0 : volume->GetGeometryHeads(); }

int card_geometry_start() { return volume == nullptr ? 0 : volume->GetPartitionStartSector(); }

void card_initialize(void* card_volume) { volume = reinterpret_cast<CardVolume*>(card_volume); }
