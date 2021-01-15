#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "EmCommon.h"

class Chunk {
   public:
    static constexpr bool isProbe{false};

   public:
    Chunk(size_t size, void* buffer);
    Chunk(Chunk&&) = default;

    void Reset();

    void Put8(uint8 value);
    void Put16(uint16 value);
    void Put32(uint32 value);
    void Put64(uint64 value);
    void PutBool(bool value);
    void PutDouble(double value);
    void PutBuffer(void* buffer, size_t size);
    void PutString(const string& str, size_t maxLength);

    uint8 Get8();
    uint16 Get16();
    uint32 Get32();
    uint64 Get64();
    bool GetBool();
    double GetDouble();
    void GetBuffer(void* buffer, size_t size);
    string GetString(size_t maxLength);

    bool HasError() const;

   private:
    bool AssertOkForSize(size_t size);

   private:
    size_t chunkSize{0};
    bool error{false};

    void* buffer{nullptr};
    void* next{nullptr};

   private:
    Chunk() = delete;
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;
    Chunk& operator=(Chunk&&) = delete;
};

#endif  // _CHUNK_H_
