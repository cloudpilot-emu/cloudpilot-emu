#ifndef _SAVESTATE_PROBE_H_
#define _SAVESTATE_PROBE_H_

#include <map>

#include "ChunkProbe.h"
#include "Logging.h"

template <typename ChunkType>
class SavestateProbe {
   public:
    using chunkMapT = std::map<ChunkType, ChunkProbe>;
    using chunkT = ChunkProbe;

   public:
    SavestateProbe() = default;

    ChunkProbe* GetChunk(ChunkType type);

    const chunkMapT& GetChunkMap() const;

    bool HasError() const;

    void NotifyError();

   private:
    chunkMapT chunkMap;
    bool error{false};
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <typename ChunkType>
ChunkProbe* SavestateProbe<ChunkType>::GetChunk(ChunkType type) {
    if (chunkMap.find(type) == chunkMap.end())
        chunkMap.insert(std::pair<ChunkType, ChunkProbe>(type, ChunkProbe()));
    else {
        logPrintf("serialization error: chunk 0x%08x requested twice", type);

        error = true;
        return nullptr;
    }

    return &(chunkMap[type]);
}

template <typename ChunkType>
bool SavestateProbe<ChunkType>::HasError() const {
    return error;
}

template <typename ChunkType>
const typename SavestateProbe<ChunkType>::chunkMapT& SavestateProbe<ChunkType>::GetChunkMap()
    const {
    return chunkMap;
}

template <typename ChunkType>
void SavestateProbe<ChunkType>::NotifyError() {
    error = true;
}

#endif  // _SAVESTATE_PROBE_H_
