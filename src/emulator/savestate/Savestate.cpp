#include "Savestate.h"

Chunk* Savestate::GetChunk(ChunkType type) {
    if (!buffer) {
        logging::printf("tried to request chunk before probing layout");
        return nullptr;
    }

    if (chunkMap.find(type) == chunkMap.end()) {
        logging::printf("chunk type 0x%04x not in map", type);
        return nullptr;
    }

    Chunk& chunk = chunkMap.at(type);

    error = error || chunk.HasError();
    chunk.Reset();

    return &chunk;
}

void Savestate::NotifyError() { error = true; }
