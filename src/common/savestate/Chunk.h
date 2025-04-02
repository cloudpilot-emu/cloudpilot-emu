#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <cstddef>
#include <cstdint>
#include <string>

class Chunk {
   public:
    static constexpr bool isProbe{false};

   public:
    Chunk(size_t size, uint32_t* buffer);
    Chunk(Chunk&&) = default;

    void Reset();

    void Put8(uint8_t value);
    void Put16(uint16_t value);
    void Put32(uint32_t value);
    void Put64(uint64_t value);
    void PutBool(bool value);
    void PutDouble(double value);
    void PutBuffer(void* buffer, size_t size);
    void PutBuffer16(uint16_t* buffer, size_t size);
    void PutBuffer32(uint32_t* buffer, size_t size);
    void PutBuffer64(uint64_t* buffer, size_t size);
    void PutString(const std::string& str, size_t maxLength);

    uint8_t Get8();
    uint16_t Get16();
    uint32_t Get32();
    uint64_t Get64();
    bool GetBool();
    double GetDouble();
    void GetBuffer(void* buffer, size_t size);
    void GetBuffer16(uint16_t* buffer, size_t size);
    void GetBuffer32(uint32_t* buffer, size_t size);
    void GetBuffer64(uint64_t* buffer, size_t size);
    std::string GetString(size_t maxLength);

    bool HasError() const;

   private:
    bool AssertOkForSize(size_t size);

   private:
    size_t chunkSize{0};
    bool error{false};

    uint32_t* buffer{nullptr};
    uint32_t* next{nullptr};

   private:
    Chunk() = delete;
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;
    Chunk& operator=(Chunk&&) = delete;
};

#endif  // _CHUNK_H_
