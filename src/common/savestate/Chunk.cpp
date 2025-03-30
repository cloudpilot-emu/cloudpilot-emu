#include "Chunk.h"

#include <cstring>

#include "CPEndian.h"
#include "Logging.h"

#if (__BYTE_ORDER == __BIG_ENDIAN)
    #define SWAP_IF_REQUIRED(x) Byteswap(x);
#else
    #define SWAP_IF_REQUIRED(x) ;
#endif

using namespace std;

Chunk::Chunk(size_t size, uint32_t* buffer) : chunkSize(size), buffer(buffer), next(buffer) {}

void Chunk::Reset() {
    next = buffer;
    error = false;
}

bool Chunk::AssertOkForSize(size_t size) {
    if (error) return false;

    error = chunkSize - (next - buffer) < size;

    return !error;
}

void Chunk::Put8(uint8_t value) { Put32(value); }

void Chunk::Put16(uint16_t value) { Put32(value); }

void Chunk::Put32(uint32_t value) {
    if (!AssertOkForSize(1)) return;

    SWAP_IF_REQUIRED(value);

    *(next++) = value;
}

void Chunk::Put64(uint64_t value) {
    Put32(value & 0xffffffff);
    Put32((value >> 32) & 0xffffffff);
}

void Chunk::PutBool(bool value) { Put32(static_cast<uint32_t>(value)); }

void Chunk::PutDouble(double value) {
    union {
        uint64_t ival;
        double dval;
    } loc;

    loc.dval = value;
    Put64(loc.ival);
}

void Chunk::PutBuffer(void* buffer, size_t size) {
    size_t wordSize = size / 4 + ((size % 4) ? 1 : 0);

    if (!AssertOkForSize(wordSize)) return;

    std::memcpy(next, buffer, size);

    next += wordSize;
}

void Chunk::PutString(const string& str, size_t maxLength) {
    if (str.size() > maxLength) {
        logPrintf("string %s exceeds length", str.c_str());
        error = true;

        return;
    }

    char* buffer = static_cast<char*>(alloca(maxLength + 1));
    memset(buffer, 0, maxLength + 1);
    strcpy(buffer, str.c_str());

    PutBuffer(buffer, maxLength + 1);
}

uint8_t Chunk::Get8() { return Get32() & 0xff; }

uint16_t Chunk::Get16() { return Get32() & 0xffff; }

uint32_t Chunk::Get32() {
    if (!AssertOkForSize(1)) return 0;

    uint32_t value = *(next++);
    SWAP_IF_REQUIRED(value);

    return value;
}

uint64_t Chunk::Get64() {
    uint64_t val = Get32();
    val |= (static_cast<uint64_t>(Get32()) << 32);

    return val;
}

bool Chunk::GetBool() { return Get32(); }

void Chunk::GetBuffer(void* buffer, size_t size) {
    size_t wordSize = size / 4 + ((size % 4) ? 1 : 0);

    if (!AssertOkForSize(wordSize)) return;

    std::memcpy(buffer, next, size);

    next += wordSize;
}

double Chunk::GetDouble() {
    union {
        uint64_t ival;
        double dval;
    } loc;

    loc.ival = Get64();

    return loc.dval;
}

string Chunk::GetString(size_t maxLength) {
    char* buffer = static_cast<char*>(alloca(maxLength + 1));
    GetBuffer(buffer, maxLength + 1);

    return buffer;
}

bool Chunk::HasError() const { return error; }
