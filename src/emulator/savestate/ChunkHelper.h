#ifndef _CHUNK_HELPER_H_
#define _CHUNK_HELPER_H_

#include <vector>

#include "Chunk.h"
#include "EmCommon.h"

template <typename T>
class SaveChunkHelper {
   public:
    class BoolPack {
        friend SaveChunkHelper<T>;

       public:
        BoolPack() = default;

        friend BoolPack operator<<(BoolPack pack, bool value) {
            if (pack.size++ > 0) pack.value <<= 1;
            pack.value = (pack.value & ~0x01) | static_cast<uint32>(value);

            return pack;
        }

       private:
        uint32 value{0};
        uint8 size{0};
    };

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
    inline SaveChunkHelper<T>& DoString(const string& str, size_t maxLength);
    inline SaveChunkHelper<T>& Do(BoolPack pack);

   private:
    T& t;
};

template <typename T>
class LoadChunkHelper {
   public:
    class BoolPack {
        friend LoadChunkHelper<T>;

       public:
        BoolPack() { values.reserve(32); }

        friend BoolPack operator<<(BoolPack pack, bool& ref) {
            pack.values.push_back(&ref);

            return pack;
        }

       private:
        std::vector<bool*> values;
    };

   public:
    LoadChunkHelper(T& t);

    inline LoadChunkHelper& Do8(uint8& value);
    inline LoadChunkHelper& Do8(uint8& v1, uint8& v2, uint8& v3, uint8& v4);
    inline LoadChunkHelper& Do16(uint16& value);
    inline LoadChunkHelper& Do16(uint16& v1, uint16& v2);
    inline LoadChunkHelper& Do32(uint32& value);
    inline LoadChunkHelper& Do64(uint64& value);
    inline LoadChunkHelper& Do8(int8& value);
    inline LoadChunkHelper& Do8(int8& v1, int8& v2, int8& v3, int8& v4);
    inline LoadChunkHelper& Do16(int16& value);
    inline LoadChunkHelper& Do16(int16& v1, int16& v2);
    inline LoadChunkHelper& Do32(int32& value);
    inline LoadChunkHelper& Do64(int64& value);
    inline LoadChunkHelper& DoBool(bool& value);
    inline LoadChunkHelper& DoDouble(double& value);
    inline LoadChunkHelper& DoBuffer(void* buffer, size_t size);
    inline LoadChunkHelper<T>& DoString(string& str, size_t maxLength);
    inline LoadChunkHelper<T>& Do(BoolPack pack);

   private:
    T& t;
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

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::DoString(const string& str, size_t maxLength) {
    t.PutString(str, maxLength);

    return *this;
}

template <typename T>
SaveChunkHelper<T>& SaveChunkHelper<T>::Do(typename SaveChunkHelper<T>::BoolPack pack) {
    return Do32(pack.value);
}

template <typename T>
LoadChunkHelper<T>::LoadChunkHelper(T& t) : t(t) {}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do8(uint8& value) {
    value = t.Get8();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do8(uint8& v1, uint8& v2, uint8& v3, uint8& v4) {
    uint32 v = t.Get32();

    v1 = v & 0xff;
    v2 = (v >> 8) & 0xff;
    v3 = (v >> 16) & 0xff;
    v4 = (v >> 24) & 0xff;

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do16(uint16& value) {
    value = t.Get16();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do16(uint16& v1, uint16& v2) {
    uint32 v = t.Get32();

    v1 = v & 0xffff;
    v2 = (v >> 16) & 0xffff;

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do32(uint32& value) {
    value = t.Get32();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do64(uint64& value) {
    value = t.Get64();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do8(int8& value) {
    value = t.Get8();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do8(int8& v1, int8& v2, int8& v3, int8& v4) {
    uint32 v = t.Get32();

    v1 = v & 0xff;
    v2 = (v >> 8) & 0xff;
    v3 = (v >> 16) & 0xff;
    v4 = (v >> 24) & 0xff;

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do16(int16& value) {
    value = t.Get16();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do16(int16& v1, int16& v2) {
    uint32 v = t.Get32();

    v1 = v & 0xffff;
    v2 = (v >> 16) & 0xffff;

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do32(int32& value) {
    value = t.Get32();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do64(int64& value) {
    value = t.Get64();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::DoBool(bool& value) {
    value = t.GetBool();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::DoDouble(double& value) {
    value = t.GetDouble();

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::DoBuffer(void* buffer, size_t size) {
    t.GetBuffer(buffer, size);

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::DoString(string& str, size_t maxLength) {
    str = std::move(t.GetString(maxLength));

    return *this;
}

template <typename T>
LoadChunkHelper<T>& LoadChunkHelper<T>::Do(typename LoadChunkHelper<T>::BoolPack pack) {
    uint32 value;
    Do32(value);

    for (ssize_t i = pack.values.size() - 1; i >= 0; i--) {
        *pack.values[i] = static_cast<bool>(value & 0x01);
        value >>= 1;
    }

    return *this;
}

#endif  // _CHUNK_HELPER_H_
