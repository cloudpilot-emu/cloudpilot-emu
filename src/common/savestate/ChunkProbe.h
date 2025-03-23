#ifndef _CHUNK_PROBE_H_
#define _CHUNK_PROBE_H_

#include <cstddef>
#include <cstdint>
#include <string>

class ChunkProbe {
   public:
    static constexpr bool isProbe{true};

   public:
    ChunkProbe() = default;

    void Put8(uint8_t value);
    void Put16(uint16_t value);
    void Put32(uint32_t value);
    void Put64(uint64_t value);
    void PutBool(bool value);
    void PutDouble(double value);
    void PutBuffer(void* buffer, size_t size);
    void PutString(const std::string& str, size_t maxLength);

    bool HasError() const;

    size_t GetSize() const;

   private:
    size_t size{0};
};

#endif  // _CHUNK_PROBE_H_
