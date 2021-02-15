#ifndef _SAVESTATE_H_
#define _SAVESTATE_H_

#include <memory>

#include "Byteswapping.h"
#include "Chunk.h"
#include "ChunkType.h"
#include "EmCommon.h"
#include "Logging.h"
#include "Platform.h"
#include "SavestateProbe.h"

class Savestate {
   public:
    using chunkMapT = std::map<ChunkType, Chunk>;
    using chunkT = Chunk;

   public:
    Savestate() = default;

    template <typename T>
    bool Save(T& t);

    Chunk* GetChunk(ChunkType type);

    void NotifyError();

    void* GetBuffer() { return buffer.get(); }
    size_t GetSize() const { return size; }

    void Reset();

   private:
    template <typename T>
    bool AllocateBuffer(T& t);

   private:
    unique_ptr<uint32[]> buffer;
    size_t size{0};

    bool error{false};

    chunkMapT chunkMap;

   private:
    Savestate(const Savestate&) = delete;
    Savestate(Savestate&&) = delete;
    Savestate& operator=(const Savestate&) = delete;
    Savestate& operator=(Savestate&&) = delete;
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Savestate::Save(T& target) {
    if (!buffer && !AllocateBuffer(target)) {
        error = true;
        return false;
    }

    error = false;
    for (auto& [chunkType, chunk] : chunkMap) chunk.Reset();

    target.Save(*this);

    for (auto& [chunkType, chunk] : chunkMap) error = error || chunk.HasError();

    return !error;
}

template <typename T>
bool Savestate::AllocateBuffer(T& target) {
    SavestateProbe probe;

    target.Save(probe);

    if (probe.HasError()) {
        logging::printf("failed to determine savestate layout");
        return false;
    }

    const SavestateProbe::chunkMapT& probeMap(probe.GetChunkMap());
    size_t chunkCount = probeMap.size();
    size = 4;

    for (auto& [chunkType, chunk] : probeMap) size = size + chunk.GetSize() * 4 + 8;

    buffer = make_unique<uint32[]>(size);

    uint32* nextTocEntry = buffer.get();
    *(nextTocEntry++) = chunkCount;

    uint32* nextChunk = buffer.get() + 1 + 2 * chunkCount;

    for (auto& [chunkType, chunk] : probeMap) {
        *nextTocEntry = static_cast<uint32>(chunkType);
        *(nextTocEntry + 1) = chunk.GetSize();

#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
        Byteswap(*nextTocEntry);
        Byteswap(*nextTocEntry + 1);
#endif

        nextTocEntry += 2;

        chunkMap.emplace(chunkType, Chunk(chunk.GetSize(), nextChunk));
        nextChunk += chunk.GetSize();
    }

    return true;
}

#endif  // _SAVESTATE_H_
