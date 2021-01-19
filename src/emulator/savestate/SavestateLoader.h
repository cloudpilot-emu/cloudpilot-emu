#ifndef _SAVESTATE_LOADER_H_
#define _SAVESTATE_LOADER_H_

#include <map>
#include <memory>

#include "Chunk.h"
#include "ChunkType.h"
#include "EmCommon.h"

class SavestateLoader {
   public:
    using chunkMapT = std::map<ChunkType, Chunk>;

   public:
    SavestateLoader() = default;

    template <typename T>
    bool Load(void* buffer, size_t size, T& tartget);

    Chunk* GetChunk(ChunkType type);

    void NotifyError();

   private:
    bool ParseSavestate(void* buffer, size_t size);

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

template <typename T>
bool SavestateLoader::Load(void* buffer, size_t size, T& target) {
    auto alignedBuffer = make_unique<uint32[]>((size & ~0x03) + ((size & 0x03) ? 1 : 0));
    memcpy(alignedBuffer.get(), buffer, size);

    if (!ParseSavestate(alignedBuffer.get(), size)) return false;

    error = false;
    for (auto& [chunkType, chunk] : chunkMap) chunk.Reset();

    target.Load(*this);

    for (auto& [chunkType, chunk] : chunkMap) error = error || chunk.HasError();

    return !error;
}

#endif  // _SAVESTATE_LOADER_H_
