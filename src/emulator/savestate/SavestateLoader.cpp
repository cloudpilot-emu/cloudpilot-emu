#include "SavestateLoader.h"

#include "Byteswapping.h"
#include "Logging.h"

bool SavestateLoader::ParseSavestate(void* buffer, size_t size) {
    chunkMap.clear();

    if (size < 4) {
        logging::printf("buffer is not a valid savestate: too small for header\n");
        return false;
    }

    uint32* nextTocEntry = static_cast<uint32*>(buffer);
    size_t chunkCount = *(nextTocEntry++);

    if (size < 4 + chunkCount * 8) {
        logging::printf("buffer is not a valid savestate: too small for TOC\n");
        return false;
    }

    uint8* nextChunk = static_cast<uint8*>(buffer) + 4 + 8 * chunkCount;

    for (size_t i = 0; i < chunkCount; i++) {
        ChunkType type = static_cast<ChunkType>(*nextTocEntry);
        size_t chunkSize = *(nextTocEntry + 1);

#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
        Byteswap(*nextTocEntry);
        Byteswap(*nextTocEntry + 1);
#endif

        nextTocEntry += 2;

        if (size - (nextChunk - static_cast<uint8*>(buffer)) < chunkSize) {
            logging::printf("buffer is not a valid savestate: too small for chunk %ul of %ul\n", i,
                            chunkCount);
            return false;
        }

        chunkMap.emplace(type, Chunk(chunkSize, static_cast<void*>(nextChunk)));

        nextChunk += chunkSize;
    }

    if ((nextChunk - static_cast<uint8*>(buffer)) != static_cast<ssize_t>(size)) {
        logging::printf("buffer is not a valid savestate: too large\n");
        return false;
    }

    return true;
}

Chunk* SavestateLoader::GetChunk(ChunkType type) {
    if (chunkMap.find(type) == chunkMap.end()) {
        logging::printf("chunk type 0x%04x not in map\n", type);
        return nullptr;
    }

    Chunk& chunk = chunkMap.at(type);

    error = error || chunk.HasError();
    chunk.Reset();

    return &chunk;
}

void SavestateLoader::NotifyError() { error = true; }
