#ifndef _SESSION_IMAGE_H_
#define _SESSION_IMAGE_H_

#include <memory>
#include <utility>

#include "EmCommon.h"

struct mz_stream_s;

class SessionImage {
   public:
    SessionImage() = default;

    const char* GetDeviceId() const;
    SessionImage& SetDeviceId(const string deviceId);
    SessionImage& SetDeviceId(const char* deviceId);

    void* GetRomImage() const;
    size_t GetRomImageSize() const;
    SessionImage& SetRomImage(void* image, size_t size);

    void* GetMemoryImage() const;
    size_t GetMemoryImageSize() const;
    SessionImage& SetMemoryImage(void* image, size_t size);

    void* GetMetadata() const;
    size_t GetMetadataSize() const;
    SessionImage& SetMetadata(void* metadata, size_t size);

    void* GetSavestate() const;
    size_t GetSavestateSize() const;
    SessionImage& SetSavestate(void* savestate, size_t size);

    uint32 GetFramebufferSize() const;
    SessionImage& SetFramebufferSize(size_t framebufferSize);

    uint32 GetVersion() const;

    bool Serialize();
    void* GetSerializedImage() const;
    size_t GetSerializedImageSize() const;

    bool Deserialize(void* buffer, size_t size);

   private:
    bool DeserializeLegacyImage(void* buffer, size_t size);

    bool AppendToSerializationStream(mz_stream_s& stream, void* buffer, size_t size);

    uint32 version;

    void *romImage{nullptr}, *ramImage{nullptr}, *savestate{nullptr}, *metadata{nullptr};
    size_t romSize{0}, ramSize{0}, savestateSize{0}, metadataSize{0}, framebufferSize{0};

    string deviceId;
    size_t serizalizedImageSize{0};

    unique_ptr<uint8[]> serializationBuffer;
    unique_ptr<uint8[]> deserializationBuffer;
};

#endif  // _SESSION_IMAGE_
