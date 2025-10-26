#include "memory_buffer.h"

#include <string.h>

bool memoryBufferAllocate(struct MemoryBuffer* memoryBuffer, size_t size) {
    if (size % MEMORY_BUFFER_GRANULARITY) return false;

    size_t pageCount = size / 1024;

    memoryBuffer->size = pageCount * 1024;
    memoryBuffer->buffer = malloc(memoryBuffer->size);
    memset(memoryBuffer->buffer, 0, memoryBuffer->size);

    size_t dirtyPageCount4 = pageCount / 32;
    if (dirtyPageCount4 * 32 < pageCount) dirtyPageCount4++;

    memoryBuffer->dirtyPagesSize = dirtyPageCount4 * 4;
    memoryBuffer->dirtyPages = malloc(memoryBuffer->dirtyPagesSize);
    memset(memoryBuffer->dirtyPages, 0, memoryBuffer->dirtyPagesSize);

    return true;
}

void memoryBufferRelease(struct MemoryBuffer* memoryBuffer) {
    if (memoryBuffer->isSubBuffer) return;

    free(memoryBuffer->buffer);
    free(memoryBuffer->dirtyPages);
}

bool memoryBufferGetSubBuffer(struct MemoryBuffer* memoryBuffer, struct MemoryBuffer* subBuffer,
                              size_t offset, size_t size) {
    if (size % MEMORY_BUFFER_GRANULARITY || offset % MEMORY_BUFFER_GRANULARITY ||
        offset + size > memoryBuffer->size)
        return false;

    subBuffer->size = size;
    subBuffer->dirtyPagesSize = size / (1024 * 8);
    subBuffer->buffer = memoryBuffer->buffer + offset;
    subBuffer->dirtyPages = memoryBuffer->dirtyPages + offset / (1024 * 32);
    subBuffer->isSubBuffer = true;

    return true;
}

void memoryBufferMarkRangeDirty(struct MemoryBuffer* memoryBuffer, size_t address, size_t size) {
    if (size == 0) return;

    for (size_t page = address >> 10; page <= (address + size - 1) >> 10; page++)
        memoryBuffer->dirtyPages[page >> 5] |= (1 << (page & 0x1f));
}
