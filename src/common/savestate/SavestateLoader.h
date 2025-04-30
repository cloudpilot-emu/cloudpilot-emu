#ifndef _SAVESTATE_LOADER_H_
#define _SAVESTATE_LOADER_H_

#include <cstdint>
#include <cstring>
#include <memory>
#include <unordered_map>

#include "CPEndian.h"
#include "Chunk.h"
#include "Logging.h"

template <typename ChunkType>
class SavestateLoader {
   public:
    using chunkMapT = std::unordered_map<ChunkType, Chunk>;

   public:
    SavestateLoader() = default;

    template <typename T>
    bool Load(void* buffer, size_t size, T& target);

    Chunk* GetChunk(ChunkType type);
    Chunk* GetChunk(ChunkType type, uint32_t version, const char* component);
    Chunk* GetChunk(ChunkType type, uint32_t version, const char* component, uint32_t& gotVersion);

    bool HasChunk(ChunkType type);

    void NotifyError();

   private:
    bool ParseSavestate(uint32_t* buffer, size_t size);

   private:
    bool error{false};

    chunkMapT chunkMap;

   private:
    SavestateLoader(const SavestateLoader&) = delete;
    SavestateLoader(SavestateLoader&&) = delete;
    SavestateLoader& operator=(const SavestateLoader&) = delete;
    SavestateLoader& operator=(SavestateLoader&) = delete;
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <typename ChunkType>
template <typename T>
bool SavestateLoader<ChunkType>::Load(void* buffer, size_t size, T& target) {
    std::unique_ptr<uint32_t[]> alignedBuffer;

    if (reinterpret_cast<uintptr_t>(buffer) & 0x03) {
        alignedBuffer = std::make_unique<uint32_t[]>(size / 4 + ((size % 4) ? 1 : 0));
        memcpy(alignedBuffer.get(), buffer, size);

        buffer = alignedBuffer.get();
    }

    if (!ParseSavestate(reinterpret_cast<uint32_t*>(buffer), size)) return false;

    error = false;
    for (auto& [chunkType, chunk] : chunkMap) chunk.Reset();

    target.Load(*this);

    for (auto& [chunkType, chunk] : chunkMap) error = error || chunk.HasError();

    return !error;
}

template <typename ChunkType>
bool SavestateLoader<ChunkType>::ParseSavestate(uint32_t* buffer, size_t size) {
    chunkMap.clear();

    if (size < 4) {
        logPrintf("buffer is not a valid savestate: too small for header");
        return false;
    }

    if (size % 4) {
        logPrintf("buffer size is not a multiple of four");
        return false;
    }

    size /= 4;

    uint32_t* nextTocEntry = buffer;
    size_t chunkCount = *(nextTocEntry++);

    if (size < 1 + chunkCount * 2) {
        logPrintf("buffer is not a valid savestate: too small for TOC");
        return false;
    }

    uint32_t* nextChunk = buffer + 1 + 2 * chunkCount;

    for (size_t i = 0; i < chunkCount; i++) {
#if (__BYTE_ORDER == __BIG_ENDIAN)
        *nextTocEntry = htole32(*nextTocEntry);
        *(nextTocEntry + 1) = htole32(*(nextTocEntry + 1));
#endif

        ChunkType type = static_cast<ChunkType>(*nextTocEntry);
        size_t chunkSize = *(nextTocEntry + 1);

        nextTocEntry += 2;

        if (size - (nextChunk - buffer) < chunkSize) {
            logPrintf("buffer is not a valid savestate: too small for chunk %lu of %lu", i,
                      chunkCount);
            return false;
        }

        chunkMap.emplace(type, Chunk(chunkSize, nextChunk));

        nextChunk += chunkSize;
    }

    if ((nextChunk - buffer) != static_cast<ssize_t>(size)) {
        logPrintf("buffer is not a valid savestate: too large");
        return false;
    }

    return true;
}

template <typename ChunkType>
Chunk* SavestateLoader<ChunkType>::GetChunk(ChunkType type) {
    if (chunkMap.find(type) == chunkMap.end()) {
        logPrintf("chunk type 0x%04x not in map", type);
        return nullptr;
    }

    Chunk& chunk = chunkMap.at(type);

    error = error || chunk.HasError();
    chunk.Reset();

    return &chunk;
}

template <typename ChunkType>
Chunk* SavestateLoader<ChunkType>::GetChunk(ChunkType type, uint32_t version,
                                            const char* component) {
    uint32_t gotVersion;
    return GetChunk(type, version, component, gotVersion);
}

template <typename ChunkType>
Chunk* SavestateLoader<ChunkType>::GetChunk(ChunkType type, uint32_t version, const char* component,
                                            uint32_t& gotVersion) {
    Chunk* chunk = GetChunk(type);
    if (!chunk) {
        logPrintf("failed to restore %s: missing savestate\n", component);
        NotifyError();

        return nullptr;
    }

    gotVersion = chunk->Get32();
    if (gotVersion > version) {
        logPrintf("failed to restore %s: unsupported savestate version %u\n", component, version);
        NotifyError();

        return nullptr;
    }

    return chunk;
}

template <typename ChunkType>
bool SavestateLoader<ChunkType>::HasChunk(ChunkType type) {
    return chunkMap.find(type) != chunkMap.end();
}

template <typename ChunkType>
void SavestateLoader<ChunkType>::NotifyError() {
    error = true;
}

#endif  // _SAVESTATE_LOADER_H_
