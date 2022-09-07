#include "ExternalStorage.h"

ExternalStorage gExternalStorage;

bool ExternalStorage::HasImage(const string& key) const { return images.find(key) != images.end(); }

CardImage* ExternalStorage::GetImage(const string& key) {
    return HasImage(key) ? &(images.at(key)) : nullptr;
}

bool ExternalStorage::AddImage(const string& key, uint8* imageData, size_t size) {
    if (size % (CardImage::BLOCK_SIZE * CardImage::BLOCK_GRANULARITY) != 0 || HasImage(key))
        return false;

    images.emplace(key, CardImage(imageData, size / CardImage::BLOCK_SIZE));

    return true;
}

bool ExternalStorage::Mount(const string& key, EmHAL::Slot slot) {
    if (IsMounted(slot) || !HasImage(key) || GetSlot(key) != EmHAL::Slot::none ||
        !EmHAL::SupportsSlot(slot))
        return false;

    slots[static_cast<uint8>(slot)] = make_unique<MountedImage>(key, images.at(key));
    EmHAL::Mount(slot, key, images.at(key));

    return true;
}

bool ExternalStorage::Unmount(EmHAL::Slot slot) {
    if (!IsMounted(slot)) return false;

    slots[static_cast<uint8>(slot)].reset();
    EmHAL::Unmount(slot);

    return true;
}

bool ExternalStorage::Unmount(const string& key) {
    if (GetSlot(key) == EmHAL::Slot::none) return false;

    return Unmount(GetSlot(key));
}

bool ExternalStorage::IsMounted(EmHAL::Slot slot) const {
    return slots[static_cast<uint8>(slot)].operator bool();
}

EmHAL::Slot ExternalStorage::GetSlot(const string& key) const {
    for (uint8 slot = 0; slot <= static_cast<uint8>(EmHAL::MAX_SLOT); slot++) {
        if (slots[slot] && slots[slot]->key == key) return static_cast<EmHAL::Slot>(slot);
    }

    return EmHAL::Slot::none;
}

CardImage* ExternalStorage::GetImageInSlot(EmHAL::Slot slot) {
    return IsMounted(slot) ? &(slots[static_cast<uint8>(slot)]->image) : nullptr;
}

string ExternalStorage::GetImageKeyInSlot(EmHAL::Slot slot) {
    return IsMounted(slot) ? slots[static_cast<uint8>(slot)]->key : "";
}

bool ExternalStorage::RemoveImage(const string& key) {
    if (!HasImage(key)) return false;

    Unmount(key);
    images.erase(key);

    return true;
}

void ExternalStorage::Clear() {
    for (uint8 slot = 0; slot <= static_cast<uint8>(EmHAL::MAX_SLOT); slot++) {
        if (slots[slot]) Unmount(static_cast<EmHAL::Slot>(slot));
    }

    images.clear();
}

ExternalStorage::MountedImage::MountedImage(const string key, CardImage& image)
    : key(key), image(image) {}
