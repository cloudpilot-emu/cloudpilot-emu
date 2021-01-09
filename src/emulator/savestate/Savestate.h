#ifndef _SAVESTATE_H_
#define _SAVESTATE_H_

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
    ~Savestate();

    template <typename T>
    bool Save(T& t);

    Chunk* GetChunk(ChunkType type);

    void NotifyError();

    void* GetBuffer() { return buffer; }
    size_t GetSize() const { return size; }

   private:
    template <typename T>
    bool AllocateBuffer(T& t);

   private:
    void* buffer{nullptr};
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

    for (auto& [chunkType, chunk] : probeMap) size = size + chunk.GetSize() + 8;

    buffer = Platform::AllocateMemory(size);

    uint32* nextTocEntry = static_cast<uint32*>(buffer);
    *(nextTocEntry++) = chunkCount;

    uint8* nextChunk = static_cast<uint8*>(buffer) + 4 + 8 * chunkCount;

    for (auto& [chunkType, chunk] : probeMap) {
        *nextTocEntry = static_cast<uint32>(chunkType);
        *(nextTocEntry + 1) = chunk.GetSize();

#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
        Byteswap(*nextTocEntry);
        Byteswap(*nextTocEntry + 1);
#endif

        nextTocEntry += 2;

        chunkMap.emplace(chunkType, Chunk(chunk.GetSize(), static_cast<void*>(nextChunk)));
        nextChunk += chunk.GetSize();
    }

    return true;
}

#endif  // _SAVESTATE_H_
