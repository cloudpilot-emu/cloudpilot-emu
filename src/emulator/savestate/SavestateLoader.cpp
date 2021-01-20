#include "SavestateLoader.h"

#include "Byteswapping.h"
#include "Logging.h"

bool SavestateLoader::ParseSavestate(uint32* buffer, size_t size) {
    chunkMap.clear();

    if (size < 4) {
        logging::printf("buffer is not a valid savestate: too small for header");
        return false;
    }

    if (size % 4) {
        logging::printf("buffer size is not a multiple of four");
        return false;
    }

    size /= 4;

    uint32* nextTocEntry = buffer;
    size_t chunkCount = *(nextTocEntry++);

    if (size < 4 + chunkCount * 2) {
        logging::printf("buffer is not a valid savestate: too small for TOC");
        return false;
    }

    uint32* nextChunk = buffer + 1 + 2 * chunkCount;

    for (size_t i = 0; i < chunkCount; i++) {
#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
        Byteswap(*nextTocEntry);
        Byteswap(*nextTocEntry + 1);
#endif

        ChunkType type = static_cast<ChunkType>(*nextTocEntry);
        size_t chunkSize = *(nextTocEntry + 1);

        nextTocEntry += 2;

        if (size - (nextChunk - buffer) < chunkSize) {
            logging::printf("buffer is not a valid savestate: too small for chunk %lu of %lu", i,
                            chunkCount);
            return false;
        }

        chunkMap.emplace(type, Chunk(chunkSize, nextChunk));

        nextChunk += chunkSize;
    }

    if ((nextChunk - buffer) != static_cast<ssize_t>(size)) {
        logging::printf("buffer is not a valid savestate: too large");
        return false;
    }

    return true;
}

Chunk* SavestateLoader::GetChunk(ChunkType type) {
    if (chunkMap.find(type) == chunkMap.end()) {
        logging::printf("chunk type 0x%04x not in map", type);
        return nullptr;
    }

    Chunk& chunk = chunkMap.at(type);

    error = error || chunk.HasError();
    chunk.Reset();

    return &chunk;
}

void SavestateLoader::NotifyError() { error = true; }
