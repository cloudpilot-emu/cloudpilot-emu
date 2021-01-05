#include "SavestateProbe.h"

ChunkProbe* SavestateProbe::GetChunk(ChunkType type) {
    if (chunkMap.find(type) == chunkMap.end())
        chunkMap.insert(std::pair<ChunkType, ChunkProbe>(type, ChunkProbe()));

    return &(chunkMap[type]);
}

const SavestateProbe::chunkMapT& SavestateProbe::GetChunkMap() const { return chunkMap; }
