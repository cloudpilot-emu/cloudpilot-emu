#ifndef _CHUNK_HELPER_H_
#define _CHUNK_HELPER_H_

#include "Chunk.h"
#include "EmCommon.h"

template <typename T>
class SaveChunkHelper {
   public:
    SaveChunkHelper(T& t);
    inline SaveChunkHelper<T>& Do8(uint8 value);
    inline SaveChunkHelper<T>& Do8(uint8 v1, uint8 v2, uint8 v3, uint8 v4);
    inline SaveChunkHelper<T>& Do16(uint16 value);
    inline SaveChunkHelper<T>& Do16(uint16 v1, uint16 v2);
    inline SaveChunkHelper<T>& Do32(uint32 value);
    inline SaveChunkHelper<T>& Do64(uint64 value);
    inline SaveChunkHelper<T>& DoBool(bool value);
    inline SaveChunkHelper<T>& DoDouble(double value);
    inline SaveChunkHelper<T>& DoBuffer(void* buffer, size_t size);

   private:
    T& t;
};

class LoadChunkHelper {
   public:
    LoadChunkHelper(Chunk& t);

    LoadChunkHelper& Do8(uint8& value);
    LoadChunkHelper& Do8(uint8& v1, uint8& v2, uint8& v3, uint8& v4);
    LoadChunkHelper& Do16(uint16& value);
    LoadChunkHelper& Do16(uint16& v1, uint16& v2);
    LoadChunkHelper& Do32(uint32& value);
    LoadChunkHelper& Do64(uint64& value);
    LoadChunkHelper& Do8(int8& value);
    LoadChunkHelper& Do8(int8& v1, int8& v2, int8& v3, int8& v4);
    LoadChunkHelper& Do16(int16& value);
    LoadChunkHelper& Do16(int16& v1, int16& v2);
    LoadChunkHelper& Do32(int32& value);
    LoadChunkHelper& Do64(int64& value);
    LoadChunkHelper& DoBool(bool& value);
    LoadChunkHelper& DoDouble(double& value);
    LoadChunkHelper& DoBuffer(void* buffer, size_t size);

   private:
    Chunk& t;
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <typename T>
SaveChunkHelper<T>::SaveChunkHelper(T& t) : t(t) {}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::Do8(uint8 value) {
    t.Put8(value);

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::Do8(uint8 v1, uint8 v2, uint8 v3, uint8 v4) {
    t.Put32(v1 | (v2 << 8) | (v3 << 16) | (v4 << 24));

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::Do16(uint16 value) {
    t.Put16(value);

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::Do16(uint16 v1, uint16 v2) {
    t.Put32(v1 | (v2 << 16));

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::Do32(uint32 value) {
    t.Put32(value);

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::Do64(uint64 value) {
    t.Put64(value);

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::DoBool(bool value) {
    t.PutBool(value);

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::DoDouble(double value) {
    t.PutDouble(value);

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::DoBuffer(void* buffer, size_t size) {
    t.PutBuffer(buffer, size);

    return *this;
}

#endif  // _CHUNK_HELPER_H_
