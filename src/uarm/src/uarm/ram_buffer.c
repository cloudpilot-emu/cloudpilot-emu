#include "ram_buffer.h"

#include <string.h>

void ramBufferAllocate(struct RamBuffer* ramBuffer, size_t size) {
    size_t pageCount = size / 512;
    if (size * 512 < size) pageCount++;

    ramBuffer->size = pageCount * 512;
    ramBuffer->buffer = malloc(ramBuffer->size);
    memset(ramBuffer->buffer, 0, ramBuffer->size);

    size_t dirtyPageCount4 = pageCount / 32;
    if (dirtyPageCount4 * 32 < pageCount) dirtyPageCount4++;

    ramBuffer->dirtyPagesSize = dirtyPageCount4 * 4;
    ramBuffer->dirtyPages = malloc(ramBuffer->dirtyPagesSize);
    memset(ramBuffer->dirtyPages, 0, ramBuffer->dirtyPagesSize);
}

void ramBufferRelease(struct RamBuffer* ramBuffer) {
    free(ramBuffer->buffer);
    free(ramBuffer->dirtyPages);
}
