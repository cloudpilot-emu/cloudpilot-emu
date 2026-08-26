#ifndef _SESSION_FILE5_H_
#define _SESSION_FILE5_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>

struct mz_stream_s;

class SessionFile5 {
   public:
    explicit SessionFile5() = default;

    static bool IsSessionFile(size_t size, const void* data);

    int32_t GetDeviceType() const;
    int32_t GetDisplayMode() const;
    SessionFile5& SetDeviceType(int32_t deviceId);
    SessionFile5& SetDisplayMode(int32_t displayMode);

    const void* GetMetadata() const;
    size_t GetMetadataSize() const;
    SessionFile5& SetMetadata(size_t size, const void* data);

    const void* GetNor() const;
    size_t GetNorSize() const;
    SessionFile5& SetNor(size_t size, const void* data);

    const void* GetNand() const;
    size_t GetNandSize() const;
    SessionFile5& SetNand(size_t size, const void* data);

    const void* GetMemory() const;
    size_t GetMemorySize() const;
    SessionFile5& SetMemory(size_t size, const void* data);

    const void* GetSavestate() const;
    size_t GetSavestateSize() const;
    SessionFile5& SetSavestate(size_t size, const void* data);

    size_t GetRamSize();
    SessionFile5& SetRamSize(uint32_t size);

    bool Serialize();
    const void* GetSerializedSession() const;
    size_t GetSerializedSessionSize() const;

    uint32_t GetVersion() const;

    bool Deserialize(size_t size, const void* data);

   private:
    bool Write32(uint32_t data);
    uint32_t Read32(bool& success);

    bool AppendToCompressionStream(size_t size, const uint8_t* data, mz_stream_s& stream);
    bool GrowBuffer(mz_stream_s& stream);
    bool Flush(mz_stream_s& stream);

    bool Deserialize_v0();
    bool Deserialize_v1_v2_v3_v4(uint32_t version);

    void MigrateV2Memory();

   private:
    int32_t deviceType{0};
    int32_t displayMode{0};

    size_t metadataSize{0};
    const uint8_t* metadata{nullptr};

    size_t norSize{0};
    const uint8_t* nor{nullptr};

    size_t nandSize{0};
    const uint8_t* nand{nullptr};

    size_t memorySize{0};
    const uint8_t* memory{nullptr};

    size_t savestateSize{0};
    const uint8_t* savestate{nullptr};

    size_t serializedSessionSize{0};
    const uint8_t* serializedSession{nullptr};

    size_t bufferSize{0};
    std::unique_ptr<uint8_t[]> buffer;

    std::unique_ptr<uint8_t[]> migratedMemory;

    uint32_t ramSize{0};

    uint8_t* cursor;
    const uint8_t* ccursor;

    uint32_t version{0};

   private:
    SessionFile5(const SessionFile5&) = delete;
    SessionFile5(SessionFile5&&) = delete;
    SessionFile5& operator=(const SessionFile5&) = delete;
    SessionFile5& operator=(SessionFile5&&) = delete;
};

#endif  // _SESSION_FILE5_H_
