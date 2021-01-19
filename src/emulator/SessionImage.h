#ifndef _SESSION_IMAGE_H_
#define _SESSION_IMAGE_H_

#include <memory>
#include <utility>

#include "EmCommon.h"

class SessionImageSerializer {
   public:
    SessionImageSerializer() = default;

    SessionImageSerializer& SetRomImage(size_t size, void* image);

    SessionImageSerializer& SetRamImage(size_t size, void* image);

    SessionImageSerializer& SetSavestate(size_t size, void* image);

    SessionImageSerializer& SetMetadata(size_t size, void* data);

    pair<size_t, unique_ptr<uint8[]>> Serialize(string deviceId) const;

   private:
    void *romImage{nullptr}, *ramImage{nullptr}, *savestate{nullptr}, *metadata{nullptr};
    size_t romSize{0}, ramSize{0}, savestateSize{0}, metadataSize{0};
};

class SessionImage {
   public:
    static SessionImage Deserialize(size_t size, uint8* buffer);

    bool IsValid() const;

    string GetDeviceId() const;
    pair<size_t, void*> GetRomImage() const;
    pair<size_t, void*> GetRamImage() const;
    pair<size_t, void*> GetMetadata() const;
    pair<size_t, void*> GetSavestate() const;

   private:
    SessionImage() = default;

    void *buffer{nullptr}, *romImage{nullptr}, *ramImage{nullptr}, *savestate{nullptr},
        *metadata{nullptr};
    size_t romSize{0}, ramSize{0}, savestateSize{0}, metadataSize{0};
    string deviceId;
};

#endif  // _SESSION_IMAGE_
