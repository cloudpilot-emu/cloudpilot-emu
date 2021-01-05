#include "ChunkHelper.h"

LoadChunkHelper::LoadChunkHelper(Chunk& t) : t(t) {}

LoadChunkHelper& LoadChunkHelper::Do8(uint8& value) {
    value = t.Get8();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do8(uint8& v1, uint8& v2, uint8& v3, uint8& v4) {
    uint32 v = t.Get32();

    v1 = v & 0xff;
    v2 = (v >> 8) & 0xff;
    v3 = (v >> 16) & 0xff;
    v4 = (v >> 24) & 0xff;

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do16(uint16& value) {
    value = t.Get16();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do16(uint16& v1, uint16& v2) {
    uint32 v = t.Get32();

    v1 = v & 0xffff;
    v2 = (v >> 16) & 0xffff;

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do32(uint32& value) {
    value = t.Get32();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do64(uint64& value) {
    value = t.Get64();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do8(int8& value) {
    value = t.Get8();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do8(int8& v1, int8& v2, int8& v3, int8& v4) {
    uint32 v = t.Get32();

    v1 = v & 0xff;
    v2 = (v >> 8) & 0xff;
    v3 = (v >> 16) & 0xff;
    v4 = (v >> 24) & 0xff;

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do16(int16& value) {
    value = t.Get16();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do16(int16& v1, int16& v2) {
    uint32 v = t.Get32();

    v1 = v & 0xffff;
    v2 = (v >> 16) & 0xffff;

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do32(int32& value) {
    value = t.Get32();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::Do64(int64& value) {
    value = t.Get64();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::DoBool(bool& value) {
    value = t.GetBool();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::DoDouble(double& value) {
    value = t.GetDouble();

    return *this;
}

LoadChunkHelper& LoadChunkHelper::DoBuffer(void* buffer, size_t size) {
    t.GetBuffer(buffer, size);

    return *this;
}
