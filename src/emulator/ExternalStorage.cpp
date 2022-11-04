#include "ExternalStorage.h"

#include "ChunkHelper.h"
#include "EmSPISlaveSD.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

ExternalStorage gExternalStorage;

namespace {
    constexpr uint32 SAVESTATE_VERSION = 1;
}

bool ExternalStorage::HasImage(const string& key) const { return images.find(key) != images.end(); }

CardImage* ExternalStorage::GetImage(const string& key) {
    return HasImage(key) ? images.at(key).get() : nullptr;
}

bool ExternalStorage::AddImage(const string& key, uint8* imageData, size_t size) {
    if (key.length() > MAX_KEY_LENGTH || size % (CardImage::BLOCK_SIZE) != 0 || HasImage(key))
        return false;

    images.emplace(key, make_shared<CardImage>(imageData, size / CardImage::BLOCK_SIZE));

    return true;
}

bool ExternalStorage::Mount(const string& key, EmHAL::Slot slot) {
    if (slot == EmHAL::Slot::none || IsMounted(slot) || !HasImage(key) ||
        GetSlot(key) != EmHAL::Slot::none || !EmHAL::SupportsImageInSlot(slot, *images.at(key)))
        return false;

    slots[static_cast<uint8>(slot)] = make_unique<MountedImage>(key, *images.at(key));
    EmHAL::Mount(slot, *images.at(key));

    return true;
}

bool ExternalStorage::Mount(const string& key) {
    if (!HasImage(key)) return false;

    for (auto slot : {EmHAL::Slot::sdcard, EmHAL::Slot::memorystick})
        if (EmHAL::SupportsImageInSlot(slot, *images.at(key))) return Mount(key, slot);

    return false;
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

void ExternalStorage::Remount() {
    for (uint8 slot = 0; slot <= static_cast<uint8>(EmHAL::MAX_SLOT); slot++) {
        const string& key(mountedKeysFromSavestate[slot]);

        if (key.empty()) continue;

        if (!HasImage(key)) {
            EmHAL::Unmount(static_cast<EmHAL::Slot>(slot));
            continue;
        }

        slots[slot] = make_unique<MountedImage>(key, *images.at(key));
        EmHAL::Remount(static_cast<EmHAL::Slot>(slot), *images.at(key));
    }
}

bool ExternalStorage::IsMounted(EmHAL::Slot slot) const {
    return slot != EmHAL::Slot::none && slots[static_cast<uint8>(slot)].operator bool();
}

bool ExternalStorage::IsMounted(const string& key) const {
    for (auto& slot : slots)
        if (slot && slot->key == key) return true;

    return false;
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

void ExternalStorage::RekeyImage(string oldKey, string newKey) {
    if (newKey == oldKey || !HasImage(oldKey) || newKey.length() > MAX_KEY_LENGTH) return;

    images.emplace(newKey, images.at(oldKey));
    images.erase(oldKey);

    for (auto& slot : slots)
        if (slot && slot->key == oldKey) slot->key = newKey;
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

template <typename T>
void ExternalStorage::DoSaveLoad(T& helper) {
    for (uint8 slot = 0; slot < 2; slot++)
        helper.DoString(mountedKeysFromSavestate[slot], MAX_KEY_LENGTH);
}

template <typename T>
void ExternalStorage::Save(T& savestate) {
    for (uint8 slot = 0; slot <= static_cast<uint8>(EmHAL::MAX_SLOT); slot++)
        mountedKeysFromSavestate[slot] = slots[slot] ? slots[slot]->key : "";

    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::externalStorage);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

template void ExternalStorage::Save<Savestate>(Savestate&);
template void ExternalStorage::Save<SavestateProbe>(SavestateProbe&);

void ExternalStorage::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::externalStorage);
    if (!chunk) return;

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore ExternalStorage: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}
