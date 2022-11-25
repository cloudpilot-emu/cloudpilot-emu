#ifndef _CHUNK_PROBE_H_
#define _CHUNK_PROBE_H_

#include "EmCommon.h"

class ChunkProbe {
   public:
    static constexpr bool isProbe{true};

   public:
    ChunkProbe() = default;

    void Put8(uint8 value);
    void Put16(uint16 value);
    void Put32(uint32 value);
    void Put64(uint64 value);
    void PutBool(bool value);
    void PutDouble(double value);
    void PutBuffer(void* buffer, size_t size);
    void PutString(const string& str, size_t maxLength);

    bool HasError() const;

    size_t GetSize() const;

   private:
    size_t size{0};
};

#endif  // _CHUNK_PROBE_H_
