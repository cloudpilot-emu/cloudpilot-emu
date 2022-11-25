#include "SavestateProbe.h"

#include "Logging.h"

ChunkProbe* SavestateProbe::GetChunk(ChunkType type) {
    if (chunkMap.find(type) == chunkMap.end())
        chunkMap.insert(std::pair<ChunkType, ChunkProbe>(type, ChunkProbe()));
    else {
        logging::printf("serialization error: chunk 0x%08x requested twice", type);

        error = true;
        return nullptr;
    }

    return &(chunkMap[type]);
}

bool SavestateProbe::HasError() const { return error; }

const SavestateProbe::chunkMapT& SavestateProbe::GetChunkMap() const { return chunkMap; }

void SavestateProbe::NotifyError() { error = true; }
