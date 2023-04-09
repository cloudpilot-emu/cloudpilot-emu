#ifndef _GUNZIP_CONTEXT_H_
#define _GUNZIP_CONTEXT_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "zip/miniz.h"

class GunzipContext {
   public:
    static constexpr size_t DEFAULT_SLICE_SIZE = 512 * 1024;

   public:
    enum class State { more = 0, done = 1, error = -1 };

   public:
    GunzipContext(const void* data, size_t size, size_t sliceSize = DEFAULT_SLICE_SIZE);
    ~GunzipContext();

    int GetState() const;
    int Continue();

    void* GetUncompressedData();
    void* ReleaseUncompressedData();
    size_t GetUncompressedSize() const;

    const char* GetError();

   private:
    void SetError(const std::string& description);

    uint32_t Read32(size_t index);
    uint16_t Read16(size_t index);
    uint8_t Read8(size_t index);

    void ReadHeaderFooter();
    void SkipZeroTerminatedString();

    void Validate();

   private:
    struct GzipHeaderFooter {
        uint16_t magic;
        uint8_t compressionMethod;
        uint8_t flags;
        uint32_t mtime;
        uint8_t xflags;
        uint8_t os;
        uint16_t xlen;
        uint32_t crc;
        uint32_t uncompressedSize;
    };

   private:
    State state{State::more};

    const uint8_t* compressedData;
    size_t compressedSize;
    const size_t sliceSize;

    size_t payloadStart{0};
    size_t payloadSize{0};
    const uint8_t* payload;

    std::unique_ptr<uint8_t[]> uncompressedData;
    size_t uncompressedSize;

    std::string errorDescription{"no error"};
    GzipHeaderFooter headerFooter;

    z_stream zipStream;
    bool zipStreamEnd{false};
};

#endif  // _GUNZIP_CONTEXT_H_
