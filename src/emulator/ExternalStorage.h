#ifndef _EXTERNAL_STORAGE_H_
#define _EXTERNAL_STORAGE_H_

#include <memory>
#include <unordered_map>

#include "CardImage.h"
#include "EmCommon.h"
#include "EmHAL.h"

class ExternalStorage {
   public:
    ExternalStorage() = default;

    bool HasImage(const string& key);
    CardImage* GetImage(const string& key);
    bool AddImage(const string& key, uint8* imageData, size_t size);

    bool Mount(const string& key, EmHAL::Slot slot);
    bool Unmount(EmHAL::Slot);
    bool Unmount(const string& key);
    bool IsMounted(EmHAL::Slot slot);

    EmHAL::Slot GetSlot(const string& key);
    CardImage* GetImageInSlot(EmHAL::Slot slot);
    string GetImageKeyInSlot(EmHAL::Slot slot);

    bool RemoveImage(const string& key);
    void Clear();

   private:
    struct MountedImage {
        MountedImage(const string key, CardImage& image);

        const string key;
        CardImage& image;
    };

    using image_map_t = unordered_map<string, CardImage>;

   private:
    image_map_t images;
    unique_ptr<MountedImage> slots[3];

   private:
    ExternalStorage(const ExternalStorage&) = delete;
    ExternalStorage(ExternalStorage&&) = delete;
    ExternalStorage& operator=(const ExternalStorage&) = delete;
    ExternalStorage& operator=(ExternalStorage&&) = delete;
};

extern ExternalStorage gExternalStorage;

#endif  // _EXTERNAL_STORAGE_H_
