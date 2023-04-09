#ifndef _GZIP_CONTEXT_H_
#define _GZIP_CONTEXT_H_

#include <cstddef>
#include <cstdint>
#include <string>

#include "zip/miniz.h"

class GzipContext {
   public:
    enum class State { initial = 0, more = 1, done = 2, error = -1 };

    static constexpr size_t DEFAULT_SLICE_SIZE = 512 * 1024;

   public:
    GzipContext(size_t uncompressedSize, const void* uncompressedData,
                size_t sliceSize = DEFAULT_SLICE_SIZE);

    ~GzipContext();

    GzipContext& SetFilename(const char* filename);
    GzipContext& SetMtime(uint32_t mtime);

    int Continue();
    int GetState() const;

    uint8_t* GetGzipData();
    size_t GetGzipSize() const;

   private:
    void SetError(const std::string& error);

    void Grow();

    void Write8(uint8_t value);
    void Write32(uint32_t value);

    void WriteHeader();
    void WriteFooter();

   private:
    State state{State::initial};
    const size_t sliceSize;
    std::string error;

    size_t uncompressedSize;
    const uint8_t* uncompressedData;

    size_t gzipIndex{0};
    size_t gzipBufferSize;
    uint8_t* gzipData{nullptr};

    std::string filename;
    uint32_t mtime{0};

    z_stream zipStream;
    bool zipStreamEnd{false};

   private:
    GzipContext(const GzipContext&);
    GzipContext(GzipContext&&);
    GzipContext& operator=(const GzipContext&);
    GzipContext& operator=(GzipContext&&);
};

#endif  // _GZIP_CONTEXT_H_
