#include "ram_buffer.h"

void ramBufferAllocate(struct RamBuffer* ramBuffer, size_t size) {
    ramBuffer->size = size;

    size_t pageCount = size / 512;
    if (size * 512 < size) pageCount++;

    ramBuffer->buffer = malloc(pageCount * 512);

    size_t dirtyPageCount4 = pageCount / 32;
    if (dirtyPageCount4 * 32 < pageCount) dirtyPageCount4++;

    ramBuffer->dirtyPages = malloc(dirtyPageCount4 * 4);
}

void ramBufferRelease(struct RamBuffer* ramBuffer) {
    free(ramBuffer->buffer);
    free(ramBuffer->dirtyPages);
}
